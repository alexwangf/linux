/*
 * Copyright 2009 Jerome Glisse.
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sub license, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL
 * THE COPYRIGHT HOLDERS, AUTHORS AND/OR ITS SUPPLIERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
 * USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial portions
 * of the Software.
 *
 */
/*
 * Authors:
 *    Jerome Glisse <glisse@freedesktop.org>
 *    Thomas Hellstrom <thomas-at-tungstengraphics-dot-com>
 *    Dave Airlie
 */
#include <linux/list.h>
#include <linux/slab.h>
#include <drm/drmP.h>
#include <drm/amdgpu_drm.h>
#include "amdgpu.h"
#include "amdgpu_trace.h"


int amdgpu_ttm_init(struct amdgpu_device *adev);
void amdgpu_ttm_fini(struct amdgpu_device *adev);

static u64 amdgpu_get_vis_part_size(struct amdgpu_device *adev,
						struct ttm_mem_reg * mem)
{
	u64 ret = 0;
	if (mem->start << PAGE_SHIFT < adev->mc.visible_vram_size) {
		ret = (u64)((mem->start << PAGE_SHIFT) + mem->size) >
			   adev->mc.visible_vram_size ?
			   adev->mc.visible_vram_size - (mem->start << PAGE_SHIFT):
			   mem->size;
	}
	return ret;
}

static void amdgpu_update_memory_usage(struct amdgpu_device *adev,
		       struct ttm_mem_reg *old_mem,
		       struct ttm_mem_reg *new_mem)
{
	u64 vis_size;
	if (!adev)
		return;

	if (new_mem) {
		switch (new_mem->mem_type) {
		case TTM_PL_TT:
			atomic64_add(new_mem->size, &adev->gtt_usage);
			break;
		case TTM_PL_VRAM:
			atomic64_add(new_mem->size, &adev->vram_usage);
			vis_size = amdgpu_get_vis_part_size(adev, new_mem);
			atomic64_add(vis_size, &adev->vram_vis_usage);
			break;
		}
	}

	if (old_mem) {
		switch (old_mem->mem_type) {
		case TTM_PL_TT:
			atomic64_sub(old_mem->size, &adev->gtt_usage);
			break;
		case TTM_PL_VRAM:
			atomic64_sub(old_mem->size, &adev->vram_usage);
			vis_size = amdgpu_get_vis_part_size(adev, old_mem);
			atomic64_sub(vis_size, &adev->vram_vis_usage);
			break;
		}
	}
}

static void amdgpu_ttm_bo_destroy(struct ttm_buffer_object *tbo)
{
	struct amdgpu_bo *bo;

	bo = container_of(tbo, struct amdgpu_bo, tbo);

	amdgpu_update_memory_usage(bo->adev, &bo->tbo.mem, NULL);
	amdgpu_mn_unregister(bo);

	mutex_lock(&bo->adev->gem.mutex);
	list_del_init(&bo->list);
	mutex_unlock(&bo->adev->gem.mutex);
	drm_gem_object_release(&bo->gem_base);
	kfree(bo->metadata);
	kfree(bo);
}

bool amdgpu_ttm_bo_is_amdgpu_bo(struct ttm_buffer_object *bo)
{
	if (bo->destroy == &amdgpu_ttm_bo_destroy)
		return true;
	return false;
}

void amdgpu_ttm_placement_from_domain(struct amdgpu_bo *rbo, u32 domain)
{
	u32 c = 0, i;
	rbo->placement.placement = rbo->placements;
	rbo->placement.busy_placement = rbo->placements;

	if (domain & AMDGPU_GEM_DOMAIN_VRAM) {
		if (rbo->flags & AMDGPU_GEM_CREATE_NO_CPU_ACCESS &&
			rbo->adev->mc.visible_vram_size < rbo->adev->mc.real_vram_size) {
			rbo->placements[c].fpfn =
				rbo->adev->mc.visible_vram_size >> PAGE_SHIFT;
			rbo->placements[c++].flags = TTM_PL_FLAG_WC | TTM_PL_FLAG_UNCACHED |
										 TTM_PL_FLAG_VRAM;
		}
		rbo->placements[c].fpfn = 0;
		rbo->placements[c++].flags = TTM_PL_FLAG_WC | TTM_PL_FLAG_UNCACHED |
									 TTM_PL_FLAG_VRAM;
	}

	if (domain & AMDGPU_GEM_DOMAIN_GTT) {
		if (rbo->flags & AMDGPU_GEM_CREATE_CPU_GTT_USWC) {
			rbo->placements[c].fpfn = 0;
			rbo->placements[c++].flags = TTM_PL_FLAG_WC | TTM_PL_FLAG_TT |
										 TTM_PL_FLAG_UNCACHED;
		} else {
			rbo->placements[c].fpfn = 0;
			rbo->placements[c++].flags = TTM_PL_FLAG_CACHED | TTM_PL_FLAG_TT;
		}
	}

	if (domain & AMDGPU_GEM_DOMAIN_CPU) {
		if (rbo->flags & AMDGPU_GEM_CREATE_CPU_GTT_USWC) {
			rbo->placements[c].fpfn = 0;
			rbo->placements[c++].flags = TTM_PL_FLAG_WC | TTM_PL_FLAG_SYSTEM |
										 TTM_PL_FLAG_UNCACHED;
		} else {
			rbo->placements[c].fpfn = 0;
			rbo->placements[c++].flags =  TTM_PL_FLAG_CACHED | TTM_PL_FLAG_SYSTEM;
		}
	}

	if (domain & AMDGPU_GEM_DOMAIN_GDS) {
		rbo->placements[c++].flags = TTM_PL_FLAG_UNCACHED |
					AMDGPU_PL_FLAG_GDS;
	}
	if (domain & AMDGPU_GEM_DOMAIN_GWS) {
		rbo->placements[c++].flags = TTM_PL_FLAG_UNCACHED |
					AMDGPU_PL_FLAG_GWS;
	}
	if (domain & AMDGPU_GEM_DOMAIN_OA) {
		rbo->placements[c++].flags = TTM_PL_FLAG_UNCACHED |
					AMDGPU_PL_FLAG_OA;
	}

	if (!c) {
		rbo->placements[c].fpfn = 0;
		rbo->placements[c++].flags = TTM_PL_MASK_CACHING |
					TTM_PL_FLAG_SYSTEM;
	}
	rbo->placement.num_placement = c;
	rbo->placement.num_busy_placement = c;

	for (i = 0; i < c; i++) {
		if ((rbo->flags & AMDGPU_GEM_CREATE_CPU_ACCESS_REQUIRED) &&
			(rbo->placements[i].flags & TTM_PL_FLAG_VRAM) &&
			!rbo->placements[i].fpfn)
			rbo->placements[i].lpfn =
				rbo->adev->mc.visible_vram_size >> PAGE_SHIFT;
		else
			rbo->placements[i].lpfn = 0;
	}
}

int amdgpu_bo_create(struct amdgpu_device *adev,
		     unsigned long size, int byte_align, bool kernel, u32 domain, u64 flags,
		     struct sg_table *sg, struct amdgpu_bo **bo_ptr)
{
	struct amdgpu_bo *bo;
	enum ttm_bo_type type;
	unsigned long page_align;
	size_t acc_size;
	int r;

	/* VI has a hw bug where VM PTEs have to be allocated in groups of 8.
	 * do this as a temporary workaround
	 */
	if (!(domain & (AMDGPU_GEM_DOMAIN_GDS | AMDGPU_GEM_DOMAIN_GWS | AMDGPU_GEM_DOMAIN_OA))) {
		if (adev->asic_type >= CHIP_TOPAZ) {
			if (byte_align & 0x7fff)
				byte_align = ALIGN(byte_align, 0x8000);
			if (size & 0x7fff)
				size = ALIGN(size, 0x8000);
		}
	}

	page_align = roundup(byte_align, PAGE_SIZE) >> PAGE_SHIFT;
	size = ALIGN(size, PAGE_SIZE);

	if (kernel) {
		type = ttm_bo_type_kernel;
	} else if (sg) {
		type = ttm_bo_type_sg;
	} else {
		type = ttm_bo_type_device;
	}
	*bo_ptr = NULL;

	acc_size = ttm_bo_dma_acc_size(&adev->mman.bdev, size,
				       sizeof(struct amdgpu_bo));

	bo = kzalloc(sizeof(struct amdgpu_bo), GFP_KERNEL);
	if (bo == NULL)
		return -ENOMEM;
	r = drm_gem_object_init(adev->ddev, &bo->gem_base, size);
	if (unlikely(r)) {
		kfree(bo);
		return r;
	}
	bo->adev = adev;
	INIT_LIST_HEAD(&bo->list);
	INIT_LIST_HEAD(&bo->va);
	bo->initial_domain = domain & (AMDGPU_GEM_DOMAIN_VRAM |
				       AMDGPU_GEM_DOMAIN_GTT |
				       AMDGPU_GEM_DOMAIN_CPU |
				       AMDGPU_GEM_DOMAIN_GDS |
				       AMDGPU_GEM_DOMAIN_GWS |
				       AMDGPU_GEM_DOMAIN_OA);

	bo->flags = flags;
	amdgpu_ttm_placement_from_domain(bo, domain);
	/* Kernel allocation are uninterruptible */
	down_read(&adev->pm.mclk_lock);
	r = ttm_bo_init(&adev->mman.bdev, &bo->tbo, size, type,
			&bo->placement, page_align, !kernel, NULL,
			acc_size, sg, NULL, &amdgpu_ttm_bo_destroy);
	up_read(&adev->pm.mclk_lock);
	if (unlikely(r != 0)) {
		return r;
	}
	*bo_ptr = bo;

	trace_amdgpu_bo_create(bo);

	return 0;
}

int amdgpu_bo_kmap(struct amdgpu_bo *bo, void **ptr)
{
	bool is_iomem;
	int r;

	if (bo->flags & AMDGPU_GEM_CREATE_NO_CPU_ACCESS)
		return -EPERM;

	if (bo->kptr) {
		if (ptr) {
			*ptr = bo->kptr;
		}
		return 0;
	}
	r = ttm_bo_kmap(&bo->tbo, 0, bo->tbo.num_pages, &bo->kmap);
	if (r) {
		return r;
	}
	bo->kptr = ttm_kmap_obj_virtual(&bo->kmap, &is_iomem);
	if (ptr) {
		*ptr = bo->kptr;
	}
	return 0;
}

void amdgpu_bo_kunmap(struct amdgpu_bo *bo)
{
	if (bo->kptr == NULL)
		return;
	bo->kptr = NULL;
	ttm_bo_kunmap(&bo->kmap);
}

struct amdgpu_bo *amdgpu_bo_ref(struct amdgpu_bo *bo)
{
	if (bo == NULL)
		return NULL;

	ttm_bo_reference(&bo->tbo);
	return bo;
}

void amdgpu_bo_unref(struct amdgpu_bo **bo)
{
	struct ttm_buffer_object *tbo;

	if ((*bo) == NULL)
		return;

	tbo = &((*bo)->tbo);
	ttm_bo_unref(&tbo);
	if (tbo == NULL)
		*bo = NULL;
}

int amdgpu_bo_pin_restricted(struct amdgpu_bo *bo, u32 domain, u64 max_offset,
			     u64 *gpu_addr)
{
	int r, i;

	if (amdgpu_ttm_tt_has_userptr(bo->tbo.ttm))
		return -EPERM;

	if (bo->pin_count) {
		bo->pin_count++;
		if (gpu_addr)
			*gpu_addr = amdgpu_bo_gpu_offset(bo);

		if (max_offset != 0) {
			u64 domain_start;

			if (domain == AMDGPU_GEM_DOMAIN_VRAM)
				domain_start = bo->adev->mc.vram_start;
			else
				domain_start = bo->adev->mc.gtt_start;
			WARN_ON_ONCE(max_offset <
				     (amdgpu_bo_gpu_offset(bo) - domain_start));
		}

		return 0;
	}
	amdgpu_ttm_placement_from_domain(bo, domain);
	for (i = 0; i < bo->placement.num_placement; i++) {
		/* force to pin into visible video ram */
		if ((bo->placements[i].flags & TTM_PL_FLAG_VRAM) &&
			!(bo->flags & AMDGPU_GEM_CREATE_NO_CPU_ACCESS) &&
			(!max_offset || max_offset > bo->adev->mc.visible_vram_size))
			bo->placements[i].lpfn =
				bo->adev->mc.visible_vram_size >> PAGE_SHIFT;
		else
			bo->placements[i].lpfn = max_offset >> PAGE_SHIFT;

		bo->placements[i].flags |= TTM_PL_FLAG_NO_EVICT;
	}

	r = ttm_bo_validate(&bo->tbo, &bo->placement, false, false);
	if (likely(r == 0)) {
		bo->pin_count = 1;
		if (gpu_addr != NULL)
			*gpu_addr = amdgpu_bo_gpu_offset(bo);
		if (domain == AMDGPU_GEM_DOMAIN_VRAM)
			bo->adev->vram_pin_size += amdgpu_bo_size(bo);
		else
			bo->adev->gart_pin_size += amdgpu_bo_size(bo);
	} else {
		dev_err(bo->adev->dev, "%p pin failed\n", bo);
	}
	return r;
}

int amdgpu_bo_pin(struct amdgpu_bo *bo, u32 domain, u64 *gpu_addr)
{
	return amdgpu_bo_pin_restricted(bo, domain, 0, gpu_addr);
}

int amdgpu_bo_unpin(struct amdgpu_bo *bo)
{
	int r, i;

	if (!bo->pin_count) {
		dev_warn(bo->adev->dev, "%p unpin not necessary\n", bo);
		return 0;
	}
	bo->pin_count--;
	if (bo->pin_count)
		return 0;
	for (i = 0; i < bo->placement.num_placement; i++) {
		bo->placements[i].lpfn = 0;
		bo->placements[i].flags &= ~TTM_PL_FLAG_NO_EVICT;
	}
	r = ttm_bo_validate(&bo->tbo, &bo->placement, false, false);
	if (likely(r == 0)) {
		if (bo->tbo.mem.mem_type == TTM_PL_VRAM)
			bo->adev->vram_pin_size -= amdgpu_bo_size(bo);
		else
			bo->adev->gart_pin_size -= amdgpu_bo_size(bo);
	} else {
		dev_err(bo->adev->dev, "%p validate failed for unpin\n", bo);
	}
	return r;
}

int amdgpu_bo_evict_vram(struct amdgpu_device *adev)
{
	/* late 2.6.33 fix IGP hibernate - we need pm ops to do this correct */
	if (0 && (adev->flags & AMDGPU_IS_APU)) {
		/* Useless to evict on IGP chips */
		return 0;
	}
	return ttm_bo_evict_mm(&adev->mman.bdev, TTM_PL_VRAM);
}

void amdgpu_bo_force_delete(struct amdgpu_device *adev)
{
	struct amdgpu_bo *bo, *n;

	if (list_empty(&adev->gem.objects)) {
		return;
	}
	dev_err(adev->dev, "Userspace still has active objects !\n");
	list_for_each_entry_safe(bo, n, &adev->gem.objects, list) {
		mutex_lock(&adev->ddev->struct_mutex);
		dev_err(adev->dev, "%p %p %lu %lu force free\n",
			&bo->gem_base, bo, (unsigned long)bo->gem_base.size,
			*((unsigned long *)&bo->gem_base.refcount));
		mutex_lock(&bo->adev->gem.mutex);
		list_del_init(&bo->list);
		mutex_unlock(&bo->adev->gem.mutex);
		/* this should unref the ttm bo */
		drm_gem_object_unreference(&bo->gem_base);
		mutex_unlock(&adev->ddev->struct_mutex);
	}
}

int amdgpu_bo_init(struct amdgpu_device *adev)
{
	/* Add an MTRR for the VRAM */
	adev->mc.vram_mtrr = arch_phys_wc_add(adev->mc.aper_base,
					      adev->mc.aper_size);
	DRM_INFO("Detected VRAM RAM=%lluM, BAR=%lluM\n",
		adev->mc.mc_vram_size >> 20,
		(unsigned long long)adev->mc.aper_size >> 20);
	DRM_INFO("RAM width %dbits DDR\n",
			adev->mc.vram_width);
	return amdgpu_ttm_init(adev);
}

void amdgpu_bo_fini(struct amdgpu_device *adev)
{
	amdgpu_ttm_fini(adev);
	arch_phys_wc_del(adev->mc.vram_mtrr);
}

int amdgpu_bo_fbdev_mmap(struct amdgpu_bo *bo,
			     struct vm_area_struct *vma)
{
	return ttm_fbdev_mmap(vma, &bo->tbo);
}

int amdgpu_bo_set_tiling_flags(struct amdgpu_bo *bo, u64 tiling_flags)
{
	unsigned bankw, bankh, mtaspect, tilesplit, stilesplit;

	bankw = (tiling_flags >> AMDGPU_TILING_EG_BANKW_SHIFT) & AMDGPU_TILING_EG_BANKW_MASK;
	bankh = (tiling_flags >> AMDGPU_TILING_EG_BANKH_SHIFT) & AMDGPU_TILING_EG_BANKH_MASK;
	mtaspect = (tiling_flags >> AMDGPU_TILING_EG_MACRO_TILE_ASPECT_SHIFT) & AMDGPU_TILING_EG_MACRO_TILE_ASPECT_MASK;
	tilesplit = (tiling_flags >> AMDGPU_TILING_EG_TILE_SPLIT_SHIFT) & AMDGPU_TILING_EG_TILE_SPLIT_MASK;
	stilesplit = (tiling_flags >> AMDGPU_TILING_EG_STENCIL_TILE_SPLIT_SHIFT) & AMDGPU_TILING_EG_STENCIL_TILE_SPLIT_MASK;
	switch (bankw) {
	case 0:
	case 1:
	case 2:
	case 4:
	case 8:
		break;
	default:
		return -EINVAL;
	}
	switch (bankh) {
	case 0:
	case 1:
	case 2:
	case 4:
	case 8:
		break;
	default:
		return -EINVAL;
	}
	switch (mtaspect) {
	case 0:
	case 1:
	case 2:
	case 4:
	case 8:
		break;
	default:
		return -EINVAL;
	}
	if (tilesplit > 6) {
		return -EINVAL;
	}
	if (stilesplit > 6) {
		return -EINVAL;
	}

	bo->tiling_flags = tiling_flags;
	return 0;
}

void amdgpu_bo_get_tiling_flags(struct amdgpu_bo *bo, u64 *tiling_flags)
{
	lockdep_assert_held(&bo->tbo.resv->lock.base);

	if (tiling_flags)
		*tiling_flags = bo->tiling_flags;
}

int amdgpu_bo_set_metadata (struct amdgpu_bo *bo, void *metadata,
			    uint32_t metadata_size, uint64_t flags)
{
	void *buffer;

	if (!metadata_size) {
		if (bo->metadata_size) {
			kfree(bo->metadata);
			bo->metadata_size = 0;
		}
		return 0;
	}

	if (metadata == NULL)
		return -EINVAL;

	buffer = kzalloc(metadata_size, GFP_KERNEL);
	if (buffer == NULL)
		return -ENOMEM;

	memcpy(buffer, metadata, metadata_size);

	kfree(bo->metadata);
	bo->metadata_flags = flags;
	bo->metadata = buffer;
	bo->metadata_size = metadata_size;

	return 0;
}

int amdgpu_bo_get_metadata(struct amdgpu_bo *bo, void *buffer,
			   size_t buffer_size, uint32_t *metadata_size,
			   uint64_t *flags)
{
	if (!buffer && !metadata_size)
		return -EINVAL;

	if (buffer) {
		if (buffer_size < bo->metadata_size)
			return -EINVAL;

		if (bo->metadata_size)
			memcpy(buffer, bo->metadata, bo->metadata_size);
	}

	if (metadata_size)
		*metadata_size = bo->metadata_size;
	if (flags)
		*flags = bo->metadata_flags;

	return 0;
}

void amdgpu_bo_move_notify(struct ttm_buffer_object *bo,
			   struct ttm_mem_reg *new_mem)
{
	struct amdgpu_bo *rbo;

	if (!amdgpu_ttm_bo_is_amdgpu_bo(bo))
		return;

	rbo = container_of(bo, struct amdgpu_bo, tbo);
	amdgpu_vm_bo_invalidate(rbo->adev, rbo);

	/* update statistics */
	if (!new_mem)
		return;

	/* move_notify is called before move happens */
	amdgpu_update_memory_usage(rbo->adev, &bo->mem, new_mem);
}

int amdgpu_bo_fault_reserve_notify(struct ttm_buffer_object *bo)
{
	struct amdgpu_device *adev;
	struct amdgpu_bo *rbo;
	unsigned long offset, size;
	int r;

	if (!amdgpu_ttm_bo_is_amdgpu_bo(bo))
		return 0;
	rbo = container_of(bo, struct amdgpu_bo, tbo);
	adev = rbo->adev;
	if (bo->mem.mem_type == TTM_PL_VRAM) {
		size = bo->mem.num_pages << PAGE_SHIFT;
		offset = bo->mem.start << PAGE_SHIFT;
		if ((offset + size) > adev->mc.visible_vram_size) {
			/* hurrah the memory is not visible ! */
			amdgpu_ttm_placement_from_domain(rbo, AMDGPU_GEM_DOMAIN_VRAM);
			rbo->placements[0].lpfn = adev->mc.visible_vram_size >> PAGE_SHIFT;
			r = ttm_bo_validate(bo, &rbo->placement, false, false);
			if (unlikely(r != 0))
				return r;
			offset = bo->mem.start << PAGE_SHIFT;
			/* this should not happen */
			if ((offset + size) > adev->mc.visible_vram_size)
				return -EINVAL;
		}
	}
	return 0;
}

/**
 * amdgpu_bo_fence - add fence to buffer object
 *
 * @bo: buffer object in question
 * @fence: fence to add
 * @shared: true if fence should be added shared
 *
 */
void amdgpu_bo_fence(struct amdgpu_bo *bo, struct amdgpu_fence *fence,
		     bool shared)
{
	struct reservation_object *resv = bo->tbo.resv;

	if (shared)
		reservation_object_add_shared_fence(resv, &fence->base);
	else
		reservation_object_add_excl_fence(resv, &fence->base);
}
