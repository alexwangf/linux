/*
 * Copyright 2012-14 Advanced Micro Devices, Inc.
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
#ifndef __DAL_EDID_14_H__
#define __DAL_EDID_14_H__

#include "edid13.h"

struct edid_base *dal_edid14_create(
	struct timing_service *ts,
	uint32_t len,
	const uint8_t *buf);

bool dal_edid14_is_v_14(uint32_t len, const uint8_t *buff);

struct cvt_3byte_timing;
struct dcs_override_mode_timing_list;

bool dal_edid14_add_cvt_3byte_timing_from_descr(
	struct edid_base *edid,
	struct dcs_mode_timing_list *list,
	const struct cvt_3byte_timing *descr);

#endif /* __DAL_EDID_14_H__ */
