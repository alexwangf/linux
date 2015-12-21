/*
 * Copyright 2014 Advanced Micro Devices, Inc.
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
 */

#include <linux/device.h>
#include <linux/export.h>
#include <linux/err.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/compat.h>
#include <uapi/linux/kfd_ioctl.h>
#include <linux/time.h>
#include <linux/mm.h>
#include <uapi/asm-generic/mman-common.h>
#include <asm/processor.h>
#include "kfd_priv.h"
#include "kfd_device_queue_manager.h"
#include "kfd_dbgmgr.h"
#include "cik_regs.h"

static long kfd_ioctl(struct file *, unsigned int, unsigned long);
static int kfd_open(struct inode *, struct file *);
static int kfd_mmap(struct file *, struct vm_area_struct *);

static const char kfd_dev_name[] = "kfd";

static const struct file_operations kfd_fops = {
	.owner = THIS_MODULE,
	.unlocked_ioctl = kfd_ioctl,
	.compat_ioctl = kfd_ioctl,
	.open = kfd_open,
	.mmap = kfd_mmap,
};

static int kfd_char_dev_major = -1;
static struct class *kfd_class;
struct device *kfd_device;

int kfd_chardev_init(void)
{
	int err = 0;

	kfd_char_dev_major = register_chrdev(0, kfd_dev_name, &kfd_fops);
	err = kfd_char_dev_major;
	if (err < 0)
		goto err_register_chrdev;

	kfd_class = class_create(THIS_MODULE, kfd_dev_name);
	err = PTR_ERR(kfd_class);
	if (IS_ERR(kfd_class))
		goto err_class_create;

	kfd_device = device_create(kfd_class, NULL,
					MKDEV(kfd_char_dev_major, 0),
					NULL, kfd_dev_name);
	err = PTR_ERR(kfd_device);
	if (IS_ERR(kfd_device))
		goto err_device_create;

	return 0;

err_device_create:
	class_destroy(kfd_class);
err_class_create:
	unregister_chrdev(kfd_char_dev_major, kfd_dev_name);
err_register_chrdev:
	return err;
}

void kfd_chardev_exit(void)
{
	device_destroy(kfd_class, MKDEV(kfd_char_dev_major, 0));
	class_destroy(kfd_class);
	unregister_chrdev(kfd_char_dev_major, kfd_dev_name);
}

struct device *kfd_chardev(void)
{
	return kfd_device;
}


static int kfd_open(struct inode *inode, struct file *filep)
{
	struct kfd_process *process;
	bool is_32bit_user_mode;

	if (iminor(inode) != 0)
		return -ENODEV;

	is_32bit_user_mode = is_compat_task();

	if (is_32bit_user_mode == true) {
		dev_warn(kfd_device,
			"Process %d (32-bit) failed to open /dev/kfd\n"
			"32-bit processes are not supported by amdkfd\n",
			current->pid);
		return -EPERM;
	}

	process = kfd_create_process(current);
	if (IS_ERR(process))
		return PTR_ERR(process);

	dev_dbg(kfd_device, "process %d opened, compat mode (32 bit) - %d\n",
		process->pasid, process->is_32bit_user_mode);

	return 0;
}

static int kfd_ioctl_get_version(struct file *filep, struct kfd_process *p,
					void *data)
{
	struct kfd_ioctl_get_version_args *args = data;
	int err = 0;

	args->major_version = KFD_IOCTL_MAJOR_VERSION;
	args->minor_version = KFD_IOCTL_MINOR_VERSION;

	return err;
}

static int set_queue_properties_from_user(struct queue_properties *q_properties,
				struct kfd_ioctl_create_queue_args *args)
{
	if (args->queue_percentage > KFD_MAX_QUEUE_PERCENTAGE) {
		pr_err("kfd: queue percentage must be between 0 to KFD_MAX_QUEUE_PERCENTAGE\n");
		return -EINVAL;
	}

	if (args->queue_priority > KFD_MAX_QUEUE_PRIORITY) {
		pr_err("kfd: queue priority must be between 0 to KFD_MAX_QUEUE_PRIORITY\n");
		return -EINVAL;
	}

	if ((args->ring_base_address) &&
		(!access_ok(VERIFY_WRITE,
			(const void __user *) args->ring_base_address,
			sizeof(uint64_t)))) {
		pr_err("kfd: can't access ring base address\n");
		return -EFAULT;
	}

	if (!is_power_of_2(args->ring_size) && (args->ring_size != 0)) {
		pr_err("kfd: ring size must be a power of 2 or 0\n");
		return -EINVAL;
	}

	if (!access_ok(VERIFY_WRITE,
			(const void __user *) args->read_pointer_address,
			sizeof(uint32_t))) {
		pr_err("kfd: can't access read pointer\n");
		return -EFAULT;
	}

	if (!access_ok(VERIFY_WRITE,
			(const void __user *) args->write_pointer_address,
			sizeof(uint32_t))) {
		pr_err("kfd: can't access write pointer\n");
		return -EFAULT;
	}

	if (args->eop_buffer_address &&
		!access_ok(VERIFY_WRITE,
			(const void __user *) args->eop_buffer_address,
			sizeof(uint32_t))) {
		pr_debug("kfd: can't access eop buffer");
		return -EFAULT;
	}

	if (args->ctx_save_restore_address &&
		!access_ok(VERIFY_WRITE,
			(const void __user *) args->ctx_save_restore_address,
			sizeof(uint32_t))) {
		pr_debug("kfd: can't access ctx save restore buffer");
		return -EFAULT;
	}

	q_properties->is_interop = false;
	q_properties->queue_percent = args->queue_percentage;
	q_properties->priority = args->queue_priority;
	q_properties->queue_address = args->ring_base_address;
	q_properties->queue_size = args->ring_size;
	q_properties->read_ptr = (uint32_t *) args->read_pointer_address;
	q_properties->write_ptr = (uint32_t *) args->write_pointer_address;
	q_properties->eop_ring_buffer_address = args->eop_buffer_address;
	q_properties->eop_ring_buffer_size = args->eop_buffer_size;
	q_properties->ctx_save_restore_area_address =
			args->ctx_save_restore_address;
	q_properties->ctx_save_restore_area_size = args->ctx_save_restore_size;
	q_properties->ctl_stack_size = args->ctl_stack_size;
	if (args->queue_type == KFD_IOC_QUEUE_TYPE_COMPUTE ||
		args->queue_type == KFD_IOC_QUEUE_TYPE_COMPUTE_AQL)
		q_properties->type = KFD_QUEUE_TYPE_COMPUTE;
	else if (args->queue_type == KFD_IOC_QUEUE_TYPE_SDMA)
		q_properties->type = KFD_QUEUE_TYPE_SDMA;
	else
		return -ENOTSUPP;

	if (args->queue_type == KFD_IOC_QUEUE_TYPE_COMPUTE_AQL)
		q_properties->format = KFD_QUEUE_FORMAT_AQL;
	else
		q_properties->format = KFD_QUEUE_FORMAT_PM4;

	pr_debug("Queue Percentage (%d, %d)\n",
			q_properties->queue_percent, args->queue_percentage);

	pr_debug("Queue Priority (%d, %d)\n",
			q_properties->priority, args->queue_priority);

	pr_debug("Queue Address (0x%llX, 0x%llX)\n",
			q_properties->queue_address, args->ring_base_address);

	pr_debug("Queue Size (0x%llX, %u)\n",
			q_properties->queue_size, args->ring_size);

	pr_debug("Queue r/w Pointers (0x%llX, 0x%llX)\n",
			(uint64_t) q_properties->read_ptr,
			(uint64_t) q_properties->write_ptr);

	pr_debug("Queue Format (%d)\n", q_properties->format);

	pr_debug("Queue EOP (0x%llX)\n", q_properties->eop_ring_buffer_address);

	pr_debug("Queue CTX save arex (0x%llX)\n",
			q_properties->ctx_save_restore_area_address);

	return 0;
}

static int kfd_ioctl_create_queue(struct file *filep, struct kfd_process *p,
					void *data)
{
	struct kfd_ioctl_create_queue_args *args = data;
	struct kfd_dev *dev;
	int err = 0;
	unsigned int queue_id;
	struct kfd_process_device *pdd;
	struct queue_properties q_properties;

	unsigned long  offset;
	memset(&q_properties, 0, sizeof(struct queue_properties));

	pr_debug("kfd: creating queue ioctl\n");

	err = set_queue_properties_from_user(&q_properties, args);
	if (err)
		return err;

	pr_debug("kfd: looking for gpu id 0x%x\n", args->gpu_id);
	dev = kfd_device_by_id(args->gpu_id);
	if (dev == NULL) {
		pr_debug("kfd: gpu id 0x%x was not found\n", args->gpu_id);
		return -EINVAL;
	}

	mutex_lock(&p->mutex);

	pdd = kfd_bind_process_to_device(dev, p);
	if (IS_ERR(pdd)) {
		err = -ESRCH;
		goto err_bind_process;
	}

	pr_debug("kfd: creating queue for PASID %d on GPU 0x%x\n",
			p->pasid,
			dev->id);

	if (dev->cwsr_enabled && !p->pqm.tba_addr) {
		pr_debug("amdkfd:Start vm_mmap, file :0x%p.\n", filep);
		offset = (args->gpu_id | KFD_MMAP_TYPE_RESERVED_MEM)
				<< PAGE_SHIFT;
		p->pqm.tba_addr = (int64_t)vm_mmap(filep, 0,
			dev->cwsr_size,
			PROT_READ | PROT_EXEC,
			MAP_SHARED,
			offset);
		if (p->pqm.tba_addr < 0) {
			pr_err("Something wrong during vm_mmap. error -%d.\n",
				(int)-p->pqm.tba_addr);
			p->pqm.tba_addr = 0;
		} else
			p->pqm.tma_addr = p->pqm.tba_addr + dev->tma_offset;

		pr_debug("set tba :0x%llx, tma:0x%llx for pqm.\n",
			p->pqm.tba_addr, p->pqm.tma_addr);
	}

	err = pqm_create_queue(&p->pqm, dev, filep, &q_properties,
				0, q_properties.type, &queue_id);
	if (err != 0)
		goto err_create_queue;

	args->queue_id = queue_id;


	/* Return gpu_id as doorbell offset for mmap usage */
	args->doorbell_offset = (KFD_MMAP_TYPE_DOORBELL | args->gpu_id);
	args->doorbell_offset <<= PAGE_SHIFT;

	mutex_unlock(&p->mutex);

	pr_debug("kfd: queue id %d was created successfully\n", args->queue_id);

	pr_debug("ring buffer address == 0x%016llX\n",
			args->ring_base_address);

	pr_debug("read ptr address    == 0x%016llX\n",
			args->read_pointer_address);

	pr_debug("write ptr address   == 0x%016llX\n",
			args->write_pointer_address);

	return 0;

err_create_queue:
err_bind_process:
	mutex_unlock(&p->mutex);
	return err;
}

static int kfd_ioctl_destroy_queue(struct file *filp, struct kfd_process *p,
					void *data)
{
	int retval;
	struct kfd_ioctl_destroy_queue_args *args = data;

	pr_debug("kfd: destroying queue id %d for PASID %d\n",
				args->queue_id,
				p->pasid);

	mutex_lock(&p->mutex);

	retval = pqm_destroy_queue(&p->pqm, args->queue_id);

	mutex_unlock(&p->mutex);
	return retval;
}

static int kfd_ioctl_update_queue(struct file *filp, struct kfd_process *p,
					void *data)
{
	int retval;
	struct kfd_ioctl_update_queue_args *args = data;
	struct queue_properties properties;

	if (args->queue_percentage > KFD_MAX_QUEUE_PERCENTAGE) {
		pr_err("kfd: queue percentage must be between 0 to KFD_MAX_QUEUE_PERCENTAGE\n");
		return -EINVAL;
	}

	if (args->queue_priority > KFD_MAX_QUEUE_PRIORITY) {
		pr_err("kfd: queue priority must be between 0 to KFD_MAX_QUEUE_PRIORITY\n");
		return -EINVAL;
	}

	if ((args->ring_base_address) &&
		(!access_ok(VERIFY_WRITE,
			(const void __user *) args->ring_base_address,
			sizeof(uint64_t)))) {
		pr_err("kfd: can't access ring base address\n");
		return -EFAULT;
	}

	if (!is_power_of_2(args->ring_size) && (args->ring_size != 0)) {
		pr_err("kfd: ring size must be a power of 2 or 0\n");
		return -EINVAL;
	}

	properties.queue_address = args->ring_base_address;
	properties.queue_size = args->ring_size;
	properties.queue_percent = args->queue_percentage;
	properties.priority = args->queue_priority;

	pr_debug("kfd: updating queue id %d for PASID %d\n",
			args->queue_id, p->pasid);

	mutex_lock(&p->mutex);

	retval = pqm_update_queue(&p->pqm, args->queue_id, &properties);

	mutex_unlock(&p->mutex);

	return retval;
}

static int kfd_ioctl_set_cu_mask(struct file *filp, struct kfd_process *p,
					void *data)
{
	int retval;
	struct kfd_ioctl_set_cu_mask_args *args = data;
	struct queue_properties properties;
	uint32_t __user *cu_mask_ptr = (uint32_t __user *)args->cu_mask_ptr;

	if (get_user(properties.cu_mask, cu_mask_ptr))
		return -EFAULT;
	if (properties.cu_mask == 0)
		return 0;

	mutex_lock(&p->mutex);

	retval = pqm_set_cu_mask(&p->pqm, args->queue_id, &properties);

	mutex_unlock(&p->mutex);

	return retval;
}

static int kfd_ioctl_set_memory_policy(struct file *filep,
					struct kfd_process *p, void *data)
{
	struct kfd_ioctl_set_memory_policy_args *args = data;
	struct kfd_dev *dev;
	int err = 0;
	struct kfd_process_device *pdd;
	enum cache_policy default_policy, alternate_policy;

	if (args->default_policy != KFD_IOC_CACHE_POLICY_COHERENT
	    && args->default_policy != KFD_IOC_CACHE_POLICY_NONCOHERENT) {
		return -EINVAL;
	}

	if (args->alternate_policy != KFD_IOC_CACHE_POLICY_COHERENT
	    && args->alternate_policy != KFD_IOC_CACHE_POLICY_NONCOHERENT) {
		return -EINVAL;
	}

	dev = kfd_device_by_id(args->gpu_id);
	if (dev == NULL)
		return -EINVAL;

	mutex_lock(&p->mutex);

	pdd = kfd_bind_process_to_device(dev, p);
	if (IS_ERR(pdd)) {
		err = -ESRCH;
		goto out;
	}

	default_policy = (args->default_policy == KFD_IOC_CACHE_POLICY_COHERENT)
			 ? cache_policy_coherent : cache_policy_noncoherent;

	alternate_policy =
		(args->alternate_policy == KFD_IOC_CACHE_POLICY_COHERENT)
		   ? cache_policy_coherent : cache_policy_noncoherent;

	if (!dev->dqm->ops.set_cache_memory_policy(dev->dqm,
				&pdd->qpd,
				default_policy,
				alternate_policy,
				(void __user *)args->alternate_aperture_base,
				args->alternate_aperture_size))
		err = -EINVAL;

out:
	mutex_unlock(&p->mutex);

	return err;
}

static int kfd_ioctl_set_trap_handler(struct file *filep,
					struct kfd_process *p, void *data)
{
	struct kfd_ioctl_set_trap_handler_args *args = data;
	struct kfd_dev *dev;
	int err = 0;
	struct kfd_process_device *pdd;

	dev = kfd_device_by_id(args->gpu_id);
	if (dev == NULL)
		return -EINVAL;

	mutex_lock(&p->mutex);

	pdd = kfd_bind_process_to_device(dev, p);
	if (IS_ERR(pdd)) {
		err = -ESRCH;
		goto out;
	}

	if (dev->dqm->ops.set_trap_handler(dev->dqm,
					&pdd->qpd,
					args->tba_addr,
					args->tma_addr))
		err = -EINVAL;

out:
	mutex_unlock(&p->mutex);

	return err;
}

static int
kfd_ioctl_dbg_register(struct file *filep, struct kfd_process *p, void *data)
{
	long status = -EFAULT;
	struct kfd_ioctl_dbg_register_args *args = data;
	struct kfd_dev *dev;
	struct kfd_dbgmgr *dbgmgr_ptr;
	struct kfd_process_device *pdd;
	bool create_ok = false;

	pr_debug("kfd:dbg: %s\n", __func__);

	dev = kfd_device_by_id(args->gpu_id);
	if (!dev) {
		dev_info(NULL, "Error! kfd: In func %s >> getting device by id failed\n", __func__);
		return status;
	}

	mutex_lock(&p->mutex);
	mutex_lock(get_dbgmgr_mutex());

	/* make sure that we have pdd, if this the first queue created for this process */
	pdd = kfd_bind_process_to_device(dev, p);
	if (IS_ERR(pdd) < 0) {
		mutex_unlock(get_dbgmgr_mutex());
		mutex_unlock(&p->mutex);
		return PTR_ERR(pdd);
	}

	if (dev->dbgmgr == NULL) {
		/* In case of a legal call, we have no dbgmgr yet */

		create_ok = kfd_dbgmgr_create(&dbgmgr_ptr, dev);
		if (create_ok) {
			status = kfd_dbgmgr_register(dbgmgr_ptr, p);
			if (status != 0)
				kfd_dbgmgr_destroy(dbgmgr_ptr);
			else
				dev->dbgmgr = dbgmgr_ptr;
		}
	}

	mutex_unlock(get_dbgmgr_mutex());
	mutex_unlock(&p->mutex);

	return status;
}

/*
 * Unregister dbg IOCTL
 */

static int
kfd_ioctl_dbg_unrgesiter(struct file *filep, struct kfd_process *p, void *data)
{
	long status = -EFAULT;
	struct kfd_ioctl_dbg_unregister_args *args = data;
	struct kfd_dev *dev;

	dev = kfd_device_by_id(args->gpu_id);
	if (!dev) {
		dev_info(NULL, "Error! kfd: In func %s >> getting device by id failed\n", __func__);
		return status;
	}

	mutex_lock(get_dbgmgr_mutex());

	status = kfd_dbgmgr_unregister(dev->dbgmgr, p);
	if (status == 0) {
		kfd_dbgmgr_destroy(dev->dbgmgr);
		dev->dbgmgr = NULL;
	}

	mutex_unlock(get_dbgmgr_mutex());

	return status;
}

/*
 * Parse and generate variable size data structure for address watch.
 * Total size of the buffer and # watch points is limited in order
 * to prevent kernel abuse. (no bearing to the much smaller HW limitation
 * which is enforced by dbgdev module.
 * please also note that the watch address itself are not "copied from user",
 * since it be set into the HW in user mode values.
 *
 */

static int
kfd_ioctl_dbg_address_watch(struct file *filep,
		struct kfd_process *p,
		void *data)
{
	long status = -EFAULT;
	struct kfd_ioctl_dbg_address_watch_args *args = data;
	struct kfd_dev *dev;
	struct dbg_address_watch_info aw_info;
	unsigned char *args_buff = NULL;
	unsigned int args_idx = 0;
	uint64_t watch_mask_value = 0;

	memset((void *) &aw_info, 0, sizeof(struct dbg_address_watch_info));

	do {
		dev = kfd_device_by_id(args->gpu_id);
		if (!dev) {
			dev_info(NULL,
			"Error! kfd: In func %s >> get device by id failed\n",
			__func__);
			break;
		}

		if (args->buf_size_in_bytes > MAX_ALLOWED_AW_BUFF_SIZE) {
			status = -EINVAL;
			break;
		}

		if (args->buf_size_in_bytes <= sizeof(*args)) {
			status = -EINVAL;
			break;
		}

		/* this is the actual buffer to work with */

		args_buff = kzalloc(args->buf_size_in_bytes -
						sizeof(*args), GFP_KERNEL);
		if (args_buff == NULL) {
			status = -ENOMEM;
			break;
		}

		if (args->content_ptr == NULL) {
			status = -EINVAL;
			break;
		}

		if (copy_from_user(args_buff,
				(void __user *) args->content_ptr,
				args->buf_size_in_bytes - sizeof(*args))) {
			dev_info(NULL,
			"Error! kfd: In func %s >> copy_from_user failed\n",
			__func__);
			break;
		}


		aw_info.process = p;

		aw_info.num_watch_points = *((uint32_t *)(&args_buff[args_idx]));
		args_idx += sizeof(aw_info.num_watch_points);

		aw_info.watch_mode = (HSA_DBG_WATCH_MODE *) &args_buff[args_idx];
		args_idx += sizeof(HSA_DBG_WATCH_MODE) * aw_info.num_watch_points;

		/* set watch address base pointer to point on the array base within args_buff */

		aw_info.watch_address = (uint64_t *) &args_buff[args_idx];

		/*skip over the addresses buffer */
		args_idx += sizeof(aw_info.watch_address) * aw_info.num_watch_points;

		if (args_idx >= args->buf_size_in_bytes) {
			status = -EINVAL;
			break;
		}

		watch_mask_value = (uint64_t) args_buff[args_idx];

		if (watch_mask_value > 0) {
			/* there is an array of masks */

			/* set watch mask base pointer to point on the array base within args_buff */
			aw_info.watch_mask = (uint64_t *) &args_buff[args_idx];

			/*skip over the masks buffer */
			args_idx += sizeof(aw_info.watch_mask) * aw_info.num_watch_points;
		}

		else
			/* just the NULL mask, set to NULL and skip over it */
		{
			aw_info.watch_mask = NULL;
			args_idx += sizeof(aw_info.watch_mask);
		}

		if (args_idx > args->buf_size_in_bytes) {
			status = -EINVAL;
			break;
		}

		aw_info.watch_event = NULL;	/* Currently HSA Event is not supported for DBG */
		status = 0;

	} while (0);

	if (status == 0) {
		mutex_lock(get_dbgmgr_mutex());

		status = kfd_dbgmgr_address_watch(dev->dbgmgr, &aw_info);

		mutex_unlock(get_dbgmgr_mutex());

	}

	kfree(args_buff);

	return status;
}

/*
 * Parse and generate fixed size data structure for wave control.
 * Buffer is generated in a "packed" form, for avoiding structure packing/pending dependencies.
 */

static int
kfd_ioctl_dbg_wave_control(struct file *filep, struct kfd_process *p, void *data)
{
	long status = -EFAULT;
	struct kfd_ioctl_dbg_wave_control_args *args = data;
	struct kfd_dev *dev;
	struct dbg_wave_control_info wac_info;
	unsigned char *args_buff = NULL;
	unsigned int args_idx = 0;
	uint32_t computed_buff_size;

	memset((void *) &wac_info, 0, sizeof(struct dbg_wave_control_info));

	/* we use compact form, independent of the packing attribute value */

	computed_buff_size = sizeof(*args) +
				sizeof(wac_info.mode) +
				sizeof(wac_info.operand) +
				sizeof(wac_info.dbgWave_msg.DbgWaveMsg) +
				sizeof(wac_info.dbgWave_msg.MemoryVA) +
				sizeof(wac_info.trapId);


	dev_info(NULL, "kfd: In func %s - start\n", __func__);

	do {
		dev = kfd_device_by_id(args->gpu_id);
		if (!dev) {
			dev_info(NULL, "Error! kfd: In func %s >> getting device by id failed\n", __func__);
			break;
		}

		/* input size must match the computed "compact" size */

		if (args->buf_size_in_bytes != computed_buff_size) {
			dev_info(NULL,
					 "Error! kfd: In func %s >> size mismatch, computed : actual %u : %u\n",
					__func__, args->buf_size_in_bytes, computed_buff_size);
			status = -EINVAL;
			break;
		}

		/* this is the actual buffer to work with */

		args_buff = kzalloc(args->buf_size_in_bytes - sizeof(*args),
				GFP_KERNEL);

		if (args_buff == NULL) {
			status = -ENOMEM;
			break;
		}

		if (args->content_ptr == NULL) {
			status = -EINVAL;
			break;
		}


		/* Now copy the entire buffer from user */


		if (copy_from_user(args_buff,
				(void __user *) args->content_ptr,
				args->buf_size_in_bytes - sizeof(*args))) {
			dev_info(NULL,
			"Error! kfd: In func %s >> copy_from_user failed\n",
			 __func__);
			break;
		}

		/* move ptr to the start of the "pay-load" area */


		wac_info.process = p;

		wac_info.operand = (HSA_DBG_WAVEOP) *((HSA_DBG_WAVEOP *)(&args_buff[args_idx]));
		args_idx += sizeof(wac_info.operand);

		wac_info.mode = (HSA_DBG_WAVEMODE) *((HSA_DBG_WAVEMODE *)(&args_buff[args_idx]));
		args_idx += sizeof(wac_info.mode);

		wac_info.trapId = (uint32_t) *((uint32_t *)(&args_buff[args_idx]));
		args_idx += sizeof(wac_info.trapId);

		wac_info.dbgWave_msg.DbgWaveMsg.WaveMsgInfoGen2.Value = *((uint32_t *)(&args_buff[args_idx]));
		wac_info.dbgWave_msg.MemoryVA = NULL;


		status = 0;

	} while (0);
	if (status == 0) {
		mutex_lock(get_dbgmgr_mutex());

		dev_info(NULL,
				"kfd: In func %s >> calling dbg manager process %p, operand %u, mode %u, trapId %u, message %u\n",
				__func__, wac_info.process, wac_info.operand, wac_info.mode, wac_info.trapId,
				wac_info.dbgWave_msg.DbgWaveMsg.WaveMsgInfoGen2.Value);

		status = kfd_dbgmgr_wave_control(dev->dbgmgr, &wac_info);

		dev_info(NULL, "kfd: In func %s >> returned status of dbg manager is %ld\n", __func__, status);

		mutex_unlock(get_dbgmgr_mutex());

	}

	kfree(args_buff);

	return status;
}

static int kfd_ioctl_get_clock_counters(struct file *filep,
				struct kfd_process *p, void *data)
{
	struct kfd_ioctl_get_clock_counters_args *args = data;
	struct kfd_dev *dev;
	struct timespec time;

	dev = kfd_device_by_id(args->gpu_id);
	if (dev)
		/* Reading GPU clock counter from KGD */
		args->gpu_clock_counter =
			dev->kfd2kgd->get_gpu_clock_counter(dev->kgd);
	else
		/* Node without GPU resource */
		args->gpu_clock_counter = 0;

	/* No access to rdtsc. Using raw monotonic time */
	getrawmonotonic(&time);
	args->cpu_clock_counter = (uint64_t)timespec_to_ns(&time);

	get_monotonic_boottime(&time);
	args->system_clock_counter = (uint64_t)timespec_to_ns(&time);

	/* Since the counter is in nano-seconds we use 1GHz frequency */
	args->system_clock_freq = 1000000000;

	return 0;
}


static int kfd_ioctl_get_process_apertures(struct file *filp,
				struct kfd_process *p, void *data)
{
	struct kfd_ioctl_get_process_apertures_args *args = data;
	struct kfd_process_device_apertures *pAperture;
	struct kfd_process_device *pdd;

	dev_dbg(kfd_device, "get apertures for PASID %d", p->pasid);

	args->num_of_nodes = 0;

	mutex_lock(&p->mutex);

	/*if the process-device list isn't empty*/
	if (kfd_has_process_device_data(p)) {
		/* Run over all pdd of the process */
		pdd = kfd_get_first_process_device_data(p);
		do {
			pAperture =
				&args->process_apertures[args->num_of_nodes];
			pAperture->gpu_id = pdd->dev->id;
			pAperture->lds_base = pdd->lds_base;
			pAperture->lds_limit = pdd->lds_limit;
			pAperture->gpuvm_base = pdd->gpuvm_base;
			pAperture->gpuvm_limit = pdd->gpuvm_limit;
			pAperture->scratch_base = pdd->scratch_base;
			pAperture->scratch_limit = pdd->scratch_limit;

			dev_dbg(kfd_device,
				"node id %u\n", args->num_of_nodes);
			dev_dbg(kfd_device,
				"gpu id %u\n", pdd->dev->id);
			dev_dbg(kfd_device,
				"lds_base %llX\n", pdd->lds_base);
			dev_dbg(kfd_device,
				"lds_limit %llX\n", pdd->lds_limit);
			dev_dbg(kfd_device,
				"gpuvm_base %llX\n", pdd->gpuvm_base);
			dev_dbg(kfd_device,
				"gpuvm_limit %llX\n", pdd->gpuvm_limit);
			dev_dbg(kfd_device,
				"scratch_base %llX\n", pdd->scratch_base);
			dev_dbg(kfd_device,
				"scratch_limit %llX\n", pdd->scratch_limit);

			args->num_of_nodes++;
		} while ((pdd = kfd_get_next_process_device_data(p, pdd)) != NULL &&
				(args->num_of_nodes < NUM_OF_SUPPORTED_GPUS));
	}

	mutex_unlock(&p->mutex);

	return 0;
}

static int
kfd_ioctl_create_event(struct file *filp, struct kfd_process *p, void *data)
{
	struct kfd_ioctl_create_event_args *args = data;
	struct kfd_dev *kfd;
	struct kfd_process_device *pdd;
	int err = -EINVAL, i;
	void *mem, *kern_addr;

	/* Map dGPU gtt BO to kernel */
	if (args->event_page_offset != 0) {
		i = 0;
		while (kfd_topology_enum_kfd_devices(i, &kfd) == 0) {
			i++;
			if (!kfd)
				continue; /* Skip non GPU devices */
			if (KFD_IS_DGPU(kfd->device_info->asic_family)) {
				mutex_lock(&p->mutex);
				pdd = kfd_bind_process_to_device(kfd, p);
				if (IS_ERR(pdd) < 0) {
					err = PTR_ERR(pdd);
					mutex_unlock(&p->mutex);
					return -EFAULT;
				}
				mem = kfd_process_device_translate_handle(pdd,
					GET_IDR_HANDLE(args->event_page_offset));
				BUG_ON(!mem);
				kfd->kfd2kgd->map_gtt_bo_to_kernel(kfd->kgd,
						mem, &kern_addr);
				mutex_unlock(&p->mutex);
				err = kfd_event_create(filp, p,
						args->event_type,
						args->auto_reset != 0,
						args->node_id,
						&args->event_id,
						&args->event_trigger_data,
						&args->event_page_offset,
						&args->event_slot_index,
						kern_addr);
			}
		}
	} else {
		err = kfd_event_create(filp, p, args->event_type,
					args->auto_reset != 0,
					args->node_id,
					&args->event_id,
					&args->event_trigger_data,
					&args->event_page_offset,
					&args->event_slot_index,
					NULL);
	}

	return err;
}

static int
kfd_ioctl_destroy_event(struct file *filp, struct kfd_process *p, void *data)
{
	struct kfd_ioctl_destroy_event_args *args = data;

	return kfd_event_destroy(p, args->event_id);
}

static int
kfd_ioctl_set_event(struct file *filp, struct kfd_process *p, void *data)
{
	struct kfd_ioctl_set_event_args *args = data;

	return kfd_set_event(p, args->event_id);
}

static int
kfd_ioctl_reset_event(struct file *filp, struct kfd_process *p, void *data)
{
	struct kfd_ioctl_reset_event_args *args = data;

	return kfd_reset_event(p, args->event_id);
}

static int
kfd_ioctl_wait_events(struct file *filp, struct kfd_process *p, void *data)
{
	struct kfd_ioctl_wait_events_args *args = data;
	enum kfd_event_wait_result wait_result;
	int err;

	err = kfd_wait_on_events(p, args->num_events,
			(void __user *)args->events_ptr,
			(args->wait_for_all != 0),
			args->timeout, &wait_result);

	args->wait_result = wait_result;

	return err;
}
static int kfd_ioctl_alloc_scratch_memory(struct file *filep,
					struct kfd_process *p, void *data)
{
	struct kfd_ioctl_alloc_memory_of_gpu_args *args =
			(struct kfd_ioctl_alloc_memory_of_gpu_args *)data;
	struct kfd_process_device *pdd;
	struct kfd_dev *dev;
	long err;

	if (args->size == 0)
		return -EINVAL;

	dev = kfd_device_by_id(args->gpu_id);
	if (dev == NULL)
		return -EINVAL;

	mutex_lock(&p->mutex);

	pdd = kfd_bind_process_to_device(dev, p);
	if (IS_ERR(pdd) < 0) {
		err = PTR_ERR(pdd);
		goto bind_process_to_device_fail;
	}

	pdd->sh_hidden_private_base_vmid = args->va_addr;
	pdd->qpd.sh_hidden_private_base = args->va_addr;

	if (sched_policy == KFD_SCHED_POLICY_NO_HWS && pdd->qpd.vmid != 0) {
		err = dev->kfd2kgd->alloc_memory_of_scratch(
			dev->kgd, args->va_addr, pdd->qpd.vmid);
		if (err != 0)
			goto alloc_memory_of_scratch_failed;
	}

	mutex_unlock(&p->mutex);

	return 0;

alloc_memory_of_scratch_failed:
bind_process_to_device_fail:
	mutex_unlock(&p->mutex);
	return -EFAULT;
}

static int kfd_ioctl_alloc_memory_of_gpu(struct file *filep,
					struct kfd_process *p, void *data)
{
	struct kfd_ioctl_alloc_memory_of_gpu_args *args = data;
	struct kfd_process_device *pdd;
	void *mem;
	struct kfd_dev *dev;
	int idr_handle;
	long err;

	if (args->size == 0)
		return -EINVAL;

	dev = kfd_device_by_id(args->gpu_id);
	if (dev == NULL)
		return -EINVAL;

	if (dev->device_info->asic_family == CHIP_CARRIZO) {
		pr_debug("kfd_ioctl_alloc_memory_of_gpu not supported on CZ\n");
		return -EINVAL;
	}

	mutex_lock(&p->mutex);

	pdd = kfd_bind_process_to_device(dev, p);
	if (IS_ERR(pdd) < 0) {
		err = PTR_ERR(pdd);
		goto bind_process_to_device_failed;
	}

	err = dev->kfd2kgd->alloc_memory_of_gpu(
		dev->kgd, args->va_addr, args->size,
		pdd->vm, (struct kgd_mem **) &mem, NULL, NULL, 0);

	if (err != 0)
		goto alloc_memory_of_gpu_failed;

	idr_handle = kfd_process_device_create_obj_handle(pdd, mem);
	if (idr_handle < 0) {
		err = -EFAULT;
		goto handle_creation_failed;
	}

	args->handle = MAKE_HANDLE(args->gpu_id, idr_handle);

	mutex_unlock(&p->mutex);

	return 0;

handle_creation_failed:
	dev->kfd2kgd->free_memory_of_gpu(dev->kgd, (struct kgd_mem *) mem);
alloc_memory_of_gpu_failed:
bind_process_to_device_failed:
	mutex_unlock(&p->mutex);
	return err;
}

static int kfd_ioctl_alloc_memory_of_gpu_new(struct file *filep,
					struct kfd_process *p, void *data)
{
	struct kfd_ioctl_alloc_memory_of_gpu_new_args *args = data;
	struct kfd_process_device *pdd;
	void *mem;
	struct kfd_dev *dev;
	int idr_handle;
	long err;
	uint64_t offset;

	if (args->size == 0)
		return -EINVAL;

	dev = kfd_device_by_id(args->gpu_id);
	if (dev == NULL)
		return -EINVAL;

	if (dev->device_info->asic_family == CHIP_CARRIZO) {
		pr_debug("kfd_ioctl_alloc_memory_of_gpu not supported on CZ\n");
		return -EINVAL;
	}

	mutex_lock(&p->mutex);

	pdd = kfd_bind_process_to_device(dev, p);
	if (IS_ERR(pdd) < 0) {
		err = PTR_ERR(pdd);
		goto bind_process_to_device_failed;
	}

	err = dev->kfd2kgd->alloc_memory_of_gpu(
		dev->kgd, args->va_addr, args->size,
		pdd->vm, (struct kgd_mem **) &mem, &offset,
		NULL, args->flags);

	if (err != 0)
		goto alloc_memory_of_gpu_failed;

	idr_handle = kfd_process_device_create_obj_handle(pdd, mem);
	if (idr_handle < 0) {
		err = -EFAULT;
		goto handle_creation_failed;
	}

	args->handle = MAKE_HANDLE(args->gpu_id, idr_handle);
	if ((args->flags & KFD_IOC_ALLOC_MEM_FLAGS_DGPU_DEVICE) != 0) {
		args->mmap_offset = 0;
	} else {
		args->mmap_offset = KFD_MMAP_TYPE_MAP_BO;
		args->mmap_offset |= KFD_MMAP_GPU_ID(args->gpu_id);
		args->mmap_offset <<= PAGE_SHIFT;
		args->mmap_offset |= offset;
	}

	mutex_unlock(&p->mutex);

	return 0;

handle_creation_failed:
	dev->kfd2kgd->free_memory_of_gpu(dev->kgd, (struct kgd_mem *) mem);
alloc_memory_of_gpu_failed:
bind_process_to_device_failed:
	mutex_unlock(&p->mutex);
	return err;
}

static int kfd_ioctl_free_memory_of_gpu(struct file *filep,
					struct kfd_process *p, void *data)
{
	struct kfd_ioctl_free_memory_of_gpu_args *args = data;
	struct kfd_process_device *pdd;
	void *mem;
	struct kfd_dev *dev;

	dev = kfd_device_by_id(GET_GPU_ID(args->handle));
	if (dev == NULL)
		return -EINVAL;

	if (dev->device_info->asic_family == CHIP_CARRIZO) {
		pr_debug("kfd_ioctl_free_memory_of_gpu not supported on CZ\n");
		return -EINVAL;
	}

	mutex_lock(&p->mutex);

	pdd = kfd_get_process_device_data(dev, p);
	if (!pdd) {
		pr_err("Process device data doesn't exist\n");
		return -EINVAL;
	}

	mem = kfd_process_device_translate_handle(pdd,
						GET_IDR_HANDLE(args->handle));
	BUG_ON(mem == NULL);

	kfd_process_device_remove_obj_handle(pdd, GET_IDR_HANDLE(args->handle));

	dev->kfd2kgd->free_memory_of_gpu(dev->kgd, mem);

	mutex_unlock(&p->mutex);
	return 0;
}

static int kfd_ioctl_map_memory_to_gpu(struct file *filep,
					struct kfd_process *p, void *data)
{
	struct kfd_ioctl_map_memory_to_gpu_args *args = data;
	struct kfd_process_device *pdd;
	void *mem;
	struct kfd_dev *dev;
	long err;

	dev = kfd_device_by_id(GET_GPU_ID(args->handle));
	if (dev == NULL)
		return -EINVAL;

	if (dev->device_info->asic_family == CHIP_CARRIZO) {
		pr_debug("kfd_ioctl_map_memory_to_gpu not supported on CZ\n");
		return -EINVAL;
	}

	mutex_lock(&p->mutex);

	pdd = kfd_bind_process_to_device(dev, p);
	if (IS_ERR(pdd) < 0) {
		err = PTR_ERR(pdd);
		goto bind_process_to_device_failed;
	}

	mem = kfd_process_device_translate_handle(pdd,
						GET_IDR_HANDLE(args->handle));
	if (mem == NULL) {
		err = PTR_ERR(mem);
		goto get_mem_obj_from_handle_failed;
	}

	err = dev->kfd2kgd->map_memory_to_gpu(
		dev->kgd, (struct kgd_mem *) mem, NULL);

	if (err != 0)
		goto map_memory_to_gpu_failed;

	radeon_flush_tlb(dev, p->pasid);

	err = dev->dqm->ops.set_page_directory_base(dev->dqm, &pdd->qpd);
	if (err != 0)
		goto set_pd_base_failed;

	mutex_unlock(&p->mutex);

	return 0;

set_pd_base_failed:
	dev->kfd2kgd->unmap_memory_to_gpu(dev->kgd, (struct kgd_mem *) mem);
map_memory_to_gpu_failed:
get_mem_obj_from_handle_failed:
bind_process_to_device_failed:
	mutex_unlock(&p->mutex);
	return err;
}

static int kfd_ioctl_unmap_memory_from_gpu(struct file *filep,
					struct kfd_process *p, void *data)
{
	struct kfd_ioctl_unmap_memory_from_gpu_args *args = data;
	struct kfd_process_device *pdd;
	void *mem;
	struct kfd_dev *dev;
	long err;

	dev = kfd_device_by_id(GET_GPU_ID(args->handle));
	if (dev == NULL)
		return -EINVAL;

	if (dev->device_info->asic_family == CHIP_CARRIZO) {
		pr_debug("kfd_ioctl_unmap_memory_from_gpu not supported on CZ\n");
		return -EINVAL;
	}

	mutex_lock(&p->mutex);

	pdd = kfd_get_process_device_data(dev, p);
	if (!pdd) {
		pr_err("Process device data doesn't exist\n");
		err = PTR_ERR(pdd);
		goto bind_process_to_device_failed;
	}

	mem = kfd_process_device_translate_handle(pdd,
						GET_IDR_HANDLE(args->handle));
	if (mem == NULL) {
		err = PTR_ERR(mem);
		goto get_mem_obj_from_handle_failed;
	}

	dev->kfd2kgd->unmap_memory_to_gpu(dev->kgd, mem);

	radeon_flush_tlb(dev, p->pasid);

	mutex_unlock(&p->mutex);
	return 0;

get_mem_obj_from_handle_failed:
bind_process_to_device_failed:
	mutex_unlock(&p->mutex);
	return err;
}

static int kfd_ioctl_open_graphic_handle(struct file *filep,
					struct kfd_process *p,
					void *data)
{
	struct kfd_ioctl_open_graphic_handle_args *args = data;
	struct kfd_dev *dev;
	struct kfd_process_device *pdd;
	void *mem;
	int idr_handle;
	long err;

	dev = kfd_device_by_id(args->gpu_id);
	if (dev == NULL)
		return -EINVAL;

	if (dev->device_info->asic_family == CHIP_CARRIZO) {
		pr_debug("kfd_ioctl_open_graphic_handle not supported on CZ\n");
		return -EINVAL;
	}

	mutex_lock(&p->mutex);

	pdd = kfd_bind_process_to_device(dev, p);
	if (IS_ERR(pdd) < 0) {
		err = PTR_ERR(pdd);
		goto bind_process_to_device_failed;
	}

	err = dev->kfd2kgd->open_graphic_handle(dev->kgd,
			args->va_addr,
			(struct kgd_vm *) pdd->vm,
			args->graphic_device_fd,
			args->graphic_handle,
			(struct kgd_mem **) &mem);

	if (err != 0)
		goto gpuvm_alloc_failed;

	idr_handle = kfd_process_device_create_obj_handle(pdd, mem);
	if (idr_handle < 0)
		goto handle_creation_failed;

	args->handle = MAKE_HANDLE(args->gpu_id, idr_handle);

	mutex_unlock(&p->mutex);

	return 0;

handle_creation_failed:
	dev->kfd2kgd->destroy_process_gpumem(dev->kgd, mem);
gpuvm_alloc_failed:
bind_process_to_device_failed:
	mutex_unlock(&p->mutex);

	return err;
}

static int kfd_ioctl_set_process_dgpu_aperture(struct file *filep,
		struct kfd_process *p, void *data)
{
	struct kfd_ioctl_set_process_dgpu_aperture_args *args = data;

	kfd_set_process_dgpu_aperture(args->node_id, p, args->dgpu_base,
			args->dgpu_limit);

	return 0;
}

#define AMDKFD_IOCTL_DEF(ioctl, _func, _flags) \
	[_IOC_NR(ioctl)] = {.cmd = ioctl, .func = _func, .flags = _flags, .cmd_drv = 0, .name = #ioctl}

/** Ioctl table */
static const struct amdkfd_ioctl_desc amdkfd_ioctls[] = {
	AMDKFD_IOCTL_DEF(AMDKFD_IOC_GET_VERSION,
			kfd_ioctl_get_version, 0),

	AMDKFD_IOCTL_DEF(AMDKFD_IOC_CREATE_QUEUE,
			kfd_ioctl_create_queue, 0),

	AMDKFD_IOCTL_DEF(AMDKFD_IOC_DESTROY_QUEUE,
			kfd_ioctl_destroy_queue, 0),

	AMDKFD_IOCTL_DEF(AMDKFD_IOC_SET_MEMORY_POLICY,
			kfd_ioctl_set_memory_policy, 0),

	AMDKFD_IOCTL_DEF(AMDKFD_IOC_GET_CLOCK_COUNTERS,
			kfd_ioctl_get_clock_counters, 0),

	AMDKFD_IOCTL_DEF(AMDKFD_IOC_GET_PROCESS_APERTURES,
			kfd_ioctl_get_process_apertures, 0),

	AMDKFD_IOCTL_DEF(AMDKFD_IOC_UPDATE_QUEUE,
			kfd_ioctl_update_queue, 0),

	AMDKFD_IOCTL_DEF(AMDKFD_IOC_CREATE_EVENT,
			kfd_ioctl_create_event, 0),

	AMDKFD_IOCTL_DEF(AMDKFD_IOC_DESTROY_EVENT,
			kfd_ioctl_destroy_event, 0),

	AMDKFD_IOCTL_DEF(AMDKFD_IOC_SET_EVENT,
			kfd_ioctl_set_event, 0),

	AMDKFD_IOCTL_DEF(AMDKFD_IOC_RESET_EVENT,
			kfd_ioctl_reset_event, 0),

	AMDKFD_IOCTL_DEF(AMDKFD_IOC_WAIT_EVENTS,
			kfd_ioctl_wait_events, 0),

	AMDKFD_IOCTL_DEF(AMDKFD_IOC_DBG_REGISTER,
			kfd_ioctl_dbg_register, 0),

	AMDKFD_IOCTL_DEF(AMDKFD_IOC_DBG_UNREGISTER,
			kfd_ioctl_dbg_unrgesiter, 0),

	AMDKFD_IOCTL_DEF(AMDKFD_IOC_DBG_ADDRESS_WATCH,
			kfd_ioctl_dbg_address_watch, 0),

	AMDKFD_IOCTL_DEF(AMDKFD_IOC_DBG_WAVE_CONTROL,
			kfd_ioctl_dbg_wave_control, 0),

	AMDKFD_IOCTL_DEF(AMDKFD_IOC_ALLOC_MEMORY_OF_GPU,
			kfd_ioctl_alloc_memory_of_gpu, 0),

	AMDKFD_IOCTL_DEF(AMDKFD_IOC_FREE_MEMORY_OF_GPU,
			kfd_ioctl_free_memory_of_gpu, 0),

	AMDKFD_IOCTL_DEF(AMDKFD_IOC_MAP_MEMORY_TO_GPU,
			kfd_ioctl_map_memory_to_gpu, 0),

	AMDKFD_IOCTL_DEF(AMDKFD_IOC_UNMAP_MEMORY_FROM_GPU,
			kfd_ioctl_unmap_memory_from_gpu, 0),

	AMDKFD_IOCTL_DEF(AMDKFD_IOC_OPEN_GRAPHIC_HANDLE,
			kfd_ioctl_open_graphic_handle, 0),

	AMDKFD_IOCTL_DEF(AMDKFD_IOC_ALLOC_MEMORY_OF_SCRATCH,
			kfd_ioctl_alloc_scratch_memory, 0),

	AMDKFD_IOCTL_DEF(AMDKFD_IOC_SET_CU_MASK,
			kfd_ioctl_set_cu_mask, 0),

	AMDKFD_IOCTL_DEF(AMDKFD_IOC_SET_PROCESS_DGPU_APERTURE,
			kfd_ioctl_set_process_dgpu_aperture, 0),

	AMDKFD_IOCTL_DEF(AMDKFD_IOC_SET_TRAP_HANDLER,
			kfd_ioctl_set_trap_handler, 0),

	AMDKFD_IOCTL_DEF(AMDKFD_IOC_ALLOC_MEMORY_OF_GPU_NEW,
				kfd_ioctl_alloc_memory_of_gpu_new, 0),

};

#define AMDKFD_CORE_IOCTL_COUNT	ARRAY_SIZE(amdkfd_ioctls)

static long kfd_ioctl(struct file *filep, unsigned int cmd, unsigned long arg)
{
	struct kfd_process *process;
	amdkfd_ioctl_t *func;
	const struct amdkfd_ioctl_desc *ioctl = NULL;
	unsigned int nr = _IOC_NR(cmd);
	char stack_kdata[128];
	char *kdata = NULL;
	unsigned int usize, asize;
	int retcode = -EINVAL;

	if (nr >= AMDKFD_CORE_IOCTL_COUNT)
		goto err_i1;

	if ((nr >= AMDKFD_COMMAND_START) && (nr < AMDKFD_COMMAND_END)) {
		u32 amdkfd_size;

		ioctl = &amdkfd_ioctls[nr];

		amdkfd_size = _IOC_SIZE(ioctl->cmd);
		usize = asize = _IOC_SIZE(cmd);
		if (amdkfd_size > asize)
			asize = amdkfd_size;

		cmd = ioctl->cmd;
	} else
		goto err_i1;

	dev_dbg(kfd_device, "ioctl cmd 0x%x (#%d), arg 0x%lx\n", cmd, nr, arg);

	process = kfd_get_process(current);
	if (IS_ERR(process)) {
		dev_dbg(kfd_device, "no process\n");
		goto err_i1;
	}

	/* Do not trust userspace, use our own definition */
	func = ioctl->func;

	if (unlikely(!func)) {
		dev_dbg(kfd_device, "no function\n");
		retcode = -EINVAL;
		goto err_i1;
	}

	if (cmd & (IOC_IN | IOC_OUT)) {
		if (asize <= sizeof(stack_kdata)) {
			kdata = stack_kdata;
		} else {
			kdata = kmalloc(asize, GFP_KERNEL);
			if (!kdata) {
				retcode = -ENOMEM;
				goto err_i1;
			}
		}
		if (asize > usize)
			memset(kdata + usize, 0, asize - usize);
	}

	if (cmd & IOC_IN) {
		if (copy_from_user(kdata, (void __user *)arg, usize) != 0) {
			retcode = -EFAULT;
			goto err_i1;
		}
	} else if (cmd & IOC_OUT) {
		memset(kdata, 0, usize);
	}

	retcode = func(filep, process, kdata);

	if (cmd & IOC_OUT)
		if (copy_to_user((void __user *)arg, kdata, usize) != 0)
			retcode = -EFAULT;

err_i1:
	if (!ioctl)
		dev_dbg(kfd_device, "invalid ioctl: pid=%d, cmd=0x%02x, nr=0x%02x\n",
			  task_pid_nr(current), cmd, nr);

	if (kdata != stack_kdata)
		kfree(kdata);

	if (retcode)
		dev_dbg(kfd_device, "ret = %d\n", retcode);

	return retcode;
}

static int kfd_mmap(struct file *filp, struct vm_area_struct *vma)
{
	struct kfd_process *process;
	struct kfd_dev *kfd;
	unsigned long vm_pgoff;
	int retval;

	process = kfd_get_process(current);
	if (IS_ERR(process))
		return PTR_ERR(process);

	vm_pgoff = vma->vm_pgoff;
	vma->vm_pgoff = KFD_MMAP_OFFSET_VALUE_GET(vma->vm_pgoff);

	switch (vm_pgoff & KFD_MMAP_TYPE_MASK) {
	case KFD_MMAP_TYPE_DOORBELL:
		return kfd_doorbell_mmap(process, vma);

	case KFD_MMAP_TYPE_EVENTS:
		return kfd_event_mmap(process, vma);

	case KFD_MMAP_TYPE_MAP_BO:
		kfd = kfd_device_by_id(KFD_MMAP_GPU_ID_GET(vm_pgoff));
		if (!kfd)
			return -EFAULT;
		retval = kfd->kfd2kgd->mmap_bo(kfd->kgd, vma);
		return retval;

	case KFD_MMAP_TYPE_RESERVED_MEM:
		return kfd_reserved_mem_mmap(process, vma);

	}

	return -EFAULT;
}
