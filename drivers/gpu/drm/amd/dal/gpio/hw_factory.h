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

#ifndef __DAL_HW_FACTORY_H__
#define __DAL_HW_FACTORY_H__

struct hw_gpio_pin;

struct hw_factory {
	uint32_t number_of_pins[GPIO_ID_COUNT];
	const struct hw_factory_funcs {
		struct hw_gpio_pin *(*create_dvo)(
			struct dal_context *ctx,
			enum gpio_id id,
			uint32_t en);
		struct hw_gpio_pin *(*create_ddc_data)(
			struct dal_context *ctx,
			enum gpio_id id,
			uint32_t en);
		struct hw_gpio_pin *(*create_ddc_clock)(
			struct dal_context *ctx,
			enum gpio_id id,
			uint32_t en);
		struct hw_gpio_pin *(*create_generic)(
			struct dal_context *ctx,
			enum gpio_id id,
			uint32_t en);
		struct hw_gpio_pin *(*create_hpd)(
			struct dal_context *ctx,
			enum gpio_id id,
			uint32_t en);
		struct hw_gpio_pin *(*create_gpio_pad)(
			struct dal_context *ctx,
			enum gpio_id id,
			uint32_t en);
		struct hw_gpio_pin *(*create_sync)(
			struct dal_context *ctx,
			enum gpio_id id,
			uint32_t en);
		struct hw_gpio_pin *(*create_gsl)(
			struct dal_context *ctx,
			enum gpio_id id,
			uint32_t en);
	} *funcs;
};

bool dal_hw_factory_init(
	struct hw_factory *factory,
	enum dce_version dce_version);

#endif
