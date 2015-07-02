/*
 * Copyright 2015 Advanced Micro Devices, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE COPYRIGHT HOLDER(S) OR AUTHOR(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Authors: AMD
 *
 */

#include <drm/drmP.h>

#include "amdgpu_dal_types.h"
#include "include/dal_interface.h"

#include "amdgpu.h"
#include "amdgpu_dm.h"
#include "amdgpu_dm_irq.h"
#include "include/dal_interface.h"


/******************************************************************************
 * Private declarations.
 *****************************************************************************/

struct handler_common_data {
	struct list_head list;
	interrupt_handler handler;
	void *handler_arg;

	/* DM which this handler belongs to */
	struct amdgpu_display_manager *dm;
};

struct amdgpu_dm_irq_handler_data {
	struct handler_common_data hcd;
	/* DAL irq source which registered for this interrupt. */
	enum dal_irq_source irq_source;
};

struct amdgpu_dm_timer_handler_data {
	struct handler_common_data hcd;
	struct delayed_work d_work;
};


#define DM_IRQ_TABLE_LOCK(adev, flags) \
	spin_lock_irqsave(&adev->dm.irq_handler_list_table_lock, flags)

#define DM_IRQ_TABLE_UNLOCK(adev, flags) \
	spin_unlock_irqrestore(&adev->dm.irq_handler_list_table_lock, flags)

/******************************************************************************
 * Private functions.
 *****************************************************************************/

static void init_handler_common_data(
	struct handler_common_data *hcd,
	void (*ih)(void *),
	void *args,
	struct amdgpu_display_manager *dm)
{
	hcd->handler = ih;
	hcd->handler_arg = args;
	hcd->dm = dm;
}

/**
 * dm_irq_work_func - Handle an IRQ outside of the interrupt handler proper.
 *
 * @work: work struct
 */
static void dm_irq_work_func(struct work_struct *work)
{
	struct list_head *entry;
	struct irq_list_head *irq_list_head =
		container_of(work, struct irq_list_head, work);
	struct list_head *handler_list = &irq_list_head->head;
	struct amdgpu_dm_irq_handler_data *handler_data;

	list_for_each(entry, handler_list) {
		handler_data =
			list_entry(
				entry,
				struct amdgpu_dm_irq_handler_data,
				hcd.list);

		DRM_DEBUG_KMS("DM_IRQ: work_func: for dal_src=%d\n",
				handler_data->irq_source);

		DRM_DEBUG_KMS("DM_IRQ: schedule_work: for dal_src=%d\n",
			handler_data->irq_source);

		handler_data->hcd.handler(handler_data->hcd.handler_arg);
	}

	/* Call a DAL subcomponent which registered for interrupt notification
	 * at INTERRUPT_LOW_IRQ_CONTEXT.
	 * (The most common use is HPD interrupt) */
}

/**
 * Remove a handler and return a pointer to hander list from which the
 * handler was removed.
 */
static struct list_head *remove_irq_handler(
	struct amdgpu_device *adev,
	void *ih,
	const struct dal_interrupt_params *int_params)
{
	struct list_head *hnd_list;
	struct list_head *entry, *tmp;
	struct amdgpu_dm_irq_handler_data *handler;
	unsigned long irq_table_flags;
	bool handler_removed = false;
	enum dal_irq_source irq_source;

	DM_IRQ_TABLE_LOCK(adev, irq_table_flags);

	irq_source = int_params->irq_source;

	switch (int_params->int_context) {
	case INTERRUPT_HIGH_IRQ_CONTEXT:
		hnd_list = &adev->dm.irq_handler_list_high_tab[irq_source];
		break;
	case INTERRUPT_LOW_IRQ_CONTEXT:
	default:
		hnd_list = &adev->dm.irq_handler_list_low_tab[irq_source].head;
		break;
	}

	list_for_each_safe(entry, tmp, hnd_list) {

		handler = list_entry(entry, struct amdgpu_dm_irq_handler_data,
				hcd.list);

		if (ih == handler) {
			/* Found our handler. Remove it from the list. */
			list_del(&handler->hcd.list);
			handler_removed = true;
			break;
		}
	}

	DM_IRQ_TABLE_UNLOCK(adev, irq_table_flags);

	if (handler_removed == false) {
		/* Not necessarily an error - caller may not
		 * know the context. */
		return NULL;
	}

	kfree(handler);

	DRM_DEBUG_KMS(
	"DM_IRQ: removed irq handler: %p for: dal_src=%d, irq context=%d\n",
		ih, int_params->irq_source, int_params->int_context);

	return hnd_list;
}

/* If 'handler_in == NULL' then remove ALL handlers. */
static void remove_timer_handler(
	struct amdgpu_device *adev,
	struct amdgpu_dm_timer_handler_data *handler_in)
{
	struct amdgpu_dm_timer_handler_data *handler_temp;
	struct list_head *handler_list;
	struct list_head *entry, *tmp;
	unsigned long irq_table_flags;
	bool handler_removed = false;

	DM_IRQ_TABLE_LOCK(adev, irq_table_flags);

	handler_list = &adev->dm.timer_handler_list;

	list_for_each_safe(entry, tmp, handler_list) {
		/* Note that list_for_each_safe() guarantees that
		 * handler_temp is NOT null. */
		handler_temp = list_entry(entry,
				struct amdgpu_dm_timer_handler_data, hcd.list);

		if (handler_in == NULL || handler_in == handler_temp) {
			list_del(&handler_temp->hcd.list);
			DM_IRQ_TABLE_UNLOCK(adev, irq_table_flags);

			DRM_DEBUG_KMS("DM_IRQ: removing timer handler: %p\n",
					handler_temp);

			if (handler_in == NULL) {
				/* Since it is still in the queue, it must
				 * be cancelled. */
				cancel_delayed_work_sync(&handler_temp->d_work);
			}

			kfree(handler_temp);
			handler_removed = true;

			DM_IRQ_TABLE_LOCK(adev, irq_table_flags);
		}

		if (handler_in == NULL) {
			/* Remove ALL handlers. */
			continue;
		}

		if (handler_in == handler_temp) {
			/* Remove a SPECIFIC handler.
			 * Found our handler - we can stop here. */
			break;
		}
	}

	DM_IRQ_TABLE_UNLOCK(adev, irq_table_flags);

	if (handler_in != NULL && handler_removed == false) {
		DRM_ERROR("DM_IRQ: handler: %p is not in the list!\n",
				handler_in);
	}
}

/**
 * dm_timer_work_func - Handle a timer.
 *
 * @work: work struct
 */
static void dm_timer_work_func(
	struct work_struct *work)
{
	struct amdgpu_dm_timer_handler_data *handler_data =
		container_of(work, struct amdgpu_dm_timer_handler_data,
				d_work.work);

	DRM_DEBUG_KMS("DM_IRQ: work_func: handler_data=%p\n", handler_data);

	/* Call a DAL subcomponent which registered for timer notification. */
	handler_data->hcd.handler(handler_data->hcd.handler_arg);

	/* We support only "single shot" timers. That means we must delete
	 * the handler after it was called. */
	remove_timer_handler(handler_data->hcd.dm->adev, handler_data);
}

/******************************************************************************
 * Public functions.
 *
 * Note: caller is responsible for input validation.
 *****************************************************************************/

void *amdgpu_dm_irq_register_interrupt(
	struct amdgpu_device *adev,
	struct dal_interrupt_params *int_params,
	void (*ih)(void *),
	void *handler_args)
{
	struct list_head *hnd_list;
	struct amdgpu_dm_irq_handler_data *handler_data;
	unsigned long irq_table_flags;
	enum dal_irq_source irq_source;

	handler_data = kzalloc(sizeof(*handler_data), GFP_KERNEL);
	if (!handler_data) {
		DRM_ERROR("DM_IRQ: failed to allocate irq handler!\n");
		return DAL_INVALID_IRQ_HANDLER_IDX;
	}

	memset(handler_data, 0, sizeof(*handler_data));

	init_handler_common_data(&handler_data->hcd, ih, handler_args,
			&adev->dm);

	irq_source = int_params->irq_source;

	handler_data->irq_source = irq_source;

	/* Lock the list, add the handler. */
	DM_IRQ_TABLE_LOCK(adev, irq_table_flags);

	switch (int_params->int_context) {
	case INTERRUPT_HIGH_IRQ_CONTEXT:
		hnd_list = &adev->dm.irq_handler_list_high_tab[irq_source];
		break;
	case INTERRUPT_LOW_IRQ_CONTEXT:
	default:
		hnd_list = &adev->dm.irq_handler_list_low_tab[irq_source].head;
		break;
	}

	list_add_tail(&handler_data->hcd.list, hnd_list);

	DM_IRQ_TABLE_UNLOCK(adev, irq_table_flags);

	/* This pointer will be stored by code which requested interrupt
	 * registration.
	 * The same pointer will be needed in order to unregister the
	 * interrupt. */

	DRM_DEBUG_KMS(
		"DM_IRQ: added irq handler: %p for: dal_src=%d, irq context=%d\n",
		handler_data,
		irq_source,
		int_params->int_context);

	return handler_data;
}

void amdgpu_dm_irq_unregister_interrupt(
	struct amdgpu_device *adev,
	enum dal_irq_source irq_source,
	void *ih)
{
	struct list_head *handler_list;
	struct dal_interrupt_params int_params;
	int i;

	memset(&int_params, 0, sizeof(int_params));

	int_params.irq_source = irq_source;

	for (i = 0; i < INTERRUPT_CONTEXT_NUMBER; i++) {

		int_params.int_context = i;

		handler_list = remove_irq_handler(adev, ih, &int_params);

		if (handler_list != NULL)
			break;
	}

	if (handler_list == NULL) {
		/* If we got here, it means we searched all irq contexts
		 * for this irq source, but the handler was not found. */
		DRM_ERROR(
		"DM_IRQ: failed to find irq handler:%p for irq_source:%d!\n",
			ih, irq_source);
	}
}

int amdgpu_dm_irq_init(
	struct amdgpu_device *adev)
{
	int src;
	struct irq_list_head *lh;

	DRM_DEBUG_KMS("DM_IRQ\n");

	spin_lock_init(&adev->dm.irq_handler_list_table_lock);

	for (src = 0; src < DAL_IRQ_SOURCES_NUMBER; src++) {
		/* low context handler list init */
		lh = &adev->dm.irq_handler_list_low_tab[src];
		INIT_LIST_HEAD(&lh->head);
		INIT_WORK(&lh->work, dm_irq_work_func);

		/* high context handler init */
		INIT_LIST_HEAD(&adev->dm.irq_handler_list_high_tab[src]);
	}

	INIT_LIST_HEAD(&adev->dm.timer_handler_list);

	/* allocate and initialize the workqueue for DM timer */
	adev->dm.timer_workqueue = create_singlethread_workqueue(
			"dm_timer_queue");
	if (adev->dm.timer_workqueue == NULL) {
		DRM_ERROR("DM_IRQ: unable to create timer queue!\n");
		return -1;
	}

	return 0;
}

void amdgpu_dm_irq_register_timer(
	struct amdgpu_device *adev,
	struct dal_timer_interrupt_params *int_params,
	interrupt_handler ih,
	void *args)
{
	unsigned long jf_delay;
	struct list_head *handler_list;
	struct amdgpu_dm_timer_handler_data *handler_data;
	unsigned long irq_table_flags;

	handler_data = kzalloc(sizeof(*handler_data), GFP_KERNEL);
	if (!handler_data) {
		DRM_ERROR("DM_IRQ: failed to allocate timer handler!\n");
		return;
	}

	memset(handler_data, 0, sizeof(*handler_data));

	init_handler_common_data(&handler_data->hcd, ih, args, &adev->dm);

	INIT_DELAYED_WORK(&handler_data->d_work, dm_timer_work_func);

	/* Lock the list, add the handler. */
	DM_IRQ_TABLE_LOCK(adev, irq_table_flags);

	handler_list = &adev->dm.timer_handler_list;

	list_add_tail(&handler_data->hcd.list, handler_list);

	DM_IRQ_TABLE_UNLOCK(adev, irq_table_flags);

	jf_delay = usecs_to_jiffies(int_params->micro_sec_interval);

	queue_delayed_work(adev->dm.timer_workqueue, &handler_data->d_work,
			jf_delay);

	DRM_DEBUG_KMS("DM_IRQ: added handler:%p with micro_sec_interval=%llu\n",
			handler_data, int_params->micro_sec_interval);
	return;
}

/* DM IRQ and timer resource release */
void amdgpu_dm_irq_fini(
	struct amdgpu_device *adev)
{
	int src;
	struct irq_list_head *lh;
	DRM_DEBUG_KMS("DM_IRQ: releasing resources.\n");

	for (src = 0; src < DAL_IRQ_SOURCES_NUMBER; src++) {

		/* The handler was removed from the table,
		 * it means it is safe to flush all the 'work'
		 * (because no code can schedule a new one). */
		lh = &adev->dm.irq_handler_list_low_tab[src];
		flush_work(&lh->work);
	}

	/* Cancel ALL timers and release handlers (if any). */
	remove_timer_handler(adev, NULL);
	/* Release the queue itself. */
	destroy_workqueue(adev->dm.timer_workqueue);
}

int amdgpu_dm_irq_resume(
	struct amdgpu_device *adev)
{
	int src;
	struct list_head *hnd_list_h, *hnd_list_l;

	DRM_DEBUG_KMS("DM_IRQ: resume\n");

	/* re-enable HW interrupt */
	for (src = DAL_IRQ_SOURCE_HPD1; src < DAL_IRQ_SOURCES_NUMBER; src++) {
		hnd_list_l = &adev->dm.irq_handler_list_low_tab[src].head;
		hnd_list_h = &adev->dm.irq_handler_list_high_tab[src];
		if (!list_empty(hnd_list_l) || !list_empty(hnd_list_h))
			dal_interrupt_set(adev->dm.dal, src, true);
	}
	return 0;
}


/**
 * amdgpu_dm_irq_schedule_work - schedule all work items registered for the
 * "irq_source".
 */
static void amdgpu_dm_irq_schedule_work(
	struct amdgpu_device *adev,
	enum dal_irq_source irq_source)
{
	unsigned long irq_table_flags;

	DM_IRQ_TABLE_LOCK(adev, irq_table_flags);

	/* Since the caller is interested in 'work_struct' then
	 * the irq will be post-processed at "INTERRUPT_LOW_IRQ_CONTEXT". */

	schedule_work(&adev->dm.irq_handler_list_low_tab[irq_source].work);

	DM_IRQ_TABLE_UNLOCK(adev, irq_table_flags);
}

/** amdgpu_dm_irq_immediate_work
 *  Callback high irq work immediately, don't send to work queue
 */
static void amdgpu_dm_irq_immediate_work(
	struct amdgpu_device *adev,
	enum dal_irq_source irq_source)
{
	struct amdgpu_dm_irq_handler_data *handler_data;
	struct list_head *entry;
	unsigned long irq_table_flags;

	DM_IRQ_TABLE_LOCK(adev, irq_table_flags);

	list_for_each(
		entry,
		&adev->dm.irq_handler_list_high_tab[irq_source]) {

		handler_data =
			list_entry(
				entry,
				struct amdgpu_dm_irq_handler_data,
				hcd.list);

		/* Call a subcomponent which registered for immediate
		 * interrupt notification */
		handler_data->hcd.handler(handler_data->hcd.handler_arg);
	}

	DM_IRQ_TABLE_UNLOCK(adev, irq_table_flags);
}

/*
 * amdgpu_dm_irq_handler
 *
 * Generic IRQ handler, calls all registered high irq work immediately, and
 * schedules work for low irq
 */
int amdgpu_dm_irq_handler(
		struct amdgpu_device *adev,
		struct amdgpu_irq_src *source,
		struct amdgpu_iv_entry *entry)
{

	enum dal_irq_source src =
		dal_interrupt_to_irq_source(
			adev->dm.dal,
			entry->src_id,
			entry->src_data);

	dal_interrupt_ack(adev->dm.dal, src);

	/* Call high irq work immediately */
	amdgpu_dm_irq_immediate_work(adev, src);
	/*Schedule low_irq work */
	amdgpu_dm_irq_schedule_work(adev, src);

	return 0;
}

static enum dal_irq_source amdgpu_dm_hpd_to_dal_irq_source(unsigned type)
{
	switch (type) {
	case AMDGPU_HPD_1:
		return DAL_IRQ_SOURCE_HPD1;
	case AMDGPU_HPD_2:
		return DAL_IRQ_SOURCE_HPD2;
	case AMDGPU_HPD_3:
		return DAL_IRQ_SOURCE_HPD3;
	case AMDGPU_HPD_4:
		return DAL_IRQ_SOURCE_HPD4;
	case AMDGPU_HPD_5:
		return DAL_IRQ_SOURCE_HPD5;
	case AMDGPU_HPD_6:
		return DAL_IRQ_SOURCE_HPD6;
	default:
		return DAL_IRQ_SOURCE_INVALID;
	}
}

static enum dal_irq_source amdgpu_dm_crtc_to_dal_irq_source(
	unsigned type)
{
	switch (type) {
	case AMDGPU_CRTC_IRQ_VBLANK1:
		return DAL_IRQ_SOURCE_CRTC1VSYNC;
	case AMDGPU_CRTC_IRQ_VBLANK2:
		return DAL_IRQ_SOURCE_CRTC2VSYNC;
	case AMDGPU_CRTC_IRQ_VBLANK3:
		return DAL_IRQ_SOURCE_CRTC3VSYNC;
	case AMDGPU_CRTC_IRQ_VBLANK4:
		return DAL_IRQ_SOURCE_CRTC4VSYNC;
	case AMDGPU_CRTC_IRQ_VBLANK5:
		return DAL_IRQ_SOURCE_CRTC5VSYNC;
	case AMDGPU_CRTC_IRQ_VBLANK6:
		return DAL_IRQ_SOURCE_CRTC6VSYNC;
	default:
		return DAL_IRQ_SOURCE_INVALID;
	}
}

static int amdgpu_dm_set_hpd_irq_state(struct amdgpu_device *adev,
					struct amdgpu_irq_src *source,
					unsigned type,
					enum amdgpu_interrupt_state state)
{
	enum dal_irq_source src = amdgpu_dm_hpd_to_dal_irq_source(type);
	bool st = (state == AMDGPU_IRQ_STATE_ENABLE);

	dal_interrupt_set(adev->dm.dal, src, st);
	return 0;
}

static int amdgpu_dm_set_pflip_irq_state(struct amdgpu_device *adev,
					struct amdgpu_irq_src *source,
					unsigned type,
					enum amdgpu_interrupt_state state)
{
	enum dal_irq_source src = dal_get_pflip_irq_src_from_display_index(
			adev->dm.dal,
			type,	/* this is the display_index because passed
				 * via work->crtc_id*/
			0	/* plane_no */);
	bool st = (state == AMDGPU_IRQ_STATE_ENABLE);

	dal_interrupt_set(adev->dm.dal, src, st);
	return 0;
}

static int amdgpu_dm_set_crtc_irq_state(struct amdgpu_device *adev,
					struct amdgpu_irq_src *source,
					unsigned type,
					enum amdgpu_interrupt_state state)
{
	enum dal_irq_source src = amdgpu_dm_crtc_to_dal_irq_source(type);
	bool st = (state == AMDGPU_IRQ_STATE_ENABLE);

	dal_interrupt_set(adev->dm.dal, src, st);
	return 0;
}

static const struct amdgpu_irq_src_funcs dm_crtc_irq_funcs = {
	.set = amdgpu_dm_set_crtc_irq_state,
	.process = amdgpu_dm_irq_handler,
};

static const struct amdgpu_irq_src_funcs dm_pageflip_irq_funcs = {
	.set = amdgpu_dm_set_pflip_irq_state,
	.process = amdgpu_dm_irq_handler,
};

static const struct amdgpu_irq_src_funcs dm_hpd_irq_funcs = {
	.set = amdgpu_dm_set_hpd_irq_state,
	.process = amdgpu_dm_irq_handler,
};

void amdgpu_dm_set_irq_funcs(struct amdgpu_device *adev)
{
	adev->crtc_irq.num_types = AMDGPU_CRTC_IRQ_LAST;
	adev->crtc_irq.funcs = &dm_crtc_irq_funcs;

	adev->pageflip_irq.num_types = AMDGPU_PAGEFLIP_IRQ_LAST;
	adev->pageflip_irq.funcs = &dm_pageflip_irq_funcs;

	adev->hpd_irq.num_types = AMDGPU_HPD_LAST;
	adev->hpd_irq.funcs = &dm_hpd_irq_funcs;
}

/*
 * amdgpu_dm_hpd_init - hpd setup callback.
 *
 * @adev: amdgpu_device pointer
 *
 * Setup the hpd pins used by the card (evergreen+).
 * Enable the pin, set the polarity, and enable the hpd interrupts.
 */
void amdgpu_dm_hpd_init(struct amdgpu_device *adev)
{
	struct drm_device *dev = adev->ddev;
	struct drm_connector *connector;

	list_for_each_entry(connector, &dev->mode_config.connector_list, head) {
		struct amdgpu_connector *amdgpu_connector =
				to_amdgpu_connector(connector);
		enum dal_irq_source src =
			amdgpu_dm_hpd_to_dal_irq_source(
				amdgpu_connector->hpd.hpd);

		if (connector->connector_type == DRM_MODE_CONNECTOR_eDP ||
			connector->connector_type == DRM_MODE_CONNECTOR_LVDS) {
			/* don't try to enable hpd on eDP or LVDS avoid breaking
			 * the aux dp channel on imac and help (but not
			 * completely fix)
			 * https://bugzilla.redhat.com/show_bug.cgi?id=726143
			 * also avoid interrupt storms during dpms.
			 */
			continue;
		}

		dal_interrupt_set(adev->dm.dal, src, true);
		amdgpu_irq_get(adev, &adev->hpd_irq, amdgpu_connector->hpd.hpd);
	}
}

/**
 * amdgpu_dm_hpd_fini - hpd tear down callback.
 *
 * @adev: amdgpu_device pointer
 *
 * Tear down the hpd pins used by the card (evergreen+).
 * Disable the hpd interrupts.
 */
void amdgpu_dm_hpd_fini(struct amdgpu_device *adev)
{
	struct drm_device *dev = adev->ddev;
	struct drm_connector *connector;

	list_for_each_entry(connector, &dev->mode_config.connector_list, head) {
		struct amdgpu_connector *amdgpu_connector =
				to_amdgpu_connector(connector);
		enum dal_irq_source src =
			amdgpu_dm_hpd_to_dal_irq_source(
				amdgpu_connector->hpd.hpd);

		dal_interrupt_set(adev->dm.dal, src, false);
		amdgpu_irq_put(adev, &adev->hpd_irq, amdgpu_connector->hpd.hpd);
	}
}
