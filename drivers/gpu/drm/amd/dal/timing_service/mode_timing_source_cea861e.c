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

#include "dal_services.h"
#include "mode_timing_source.h"

static const struct mode_timing_ex mode_timings[] = {
/* 640x480p, 60Hz, 4:3 */
{ {
	{640, 480, 60, TIMING_STANDARD_CEA861, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0, 1} },
	{800, 0, 640, 0, 16, 96, 525, 0, 480, 0, 10, 2, 25175, 1, 0,
		TIMING_STANDARD_CEA861, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
		{0, 0, 1, 0, 0, 0, 0} } },
	ASPECT_RATIO_4_3},
{ {
	{640, 480, 60, TIMING_STANDARD_CEA861, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0, 0} },
	{800, 0, 640, 0, 16, 96, 525, 0, 480, 0, 10, 2, 25200, 1, 0,
		TIMING_STANDARD_CEA861,	TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
		{0, 0, 1, 0, 0, 0, 0} } },
	ASPECT_RATIO_4_3},

/* 720x480p, 60Hz, 4:3 --> 16:9 */
{ {
	{720, 480, 60, TIMING_STANDARD_CEA861, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0, 1} },
	{858, 0, 720, 0, 16, 62, 525, 0, 480, 0, 9, 6, 27000, 2, 0,
		TIMING_STANDARD_CEA861,	TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
		{0, 0, 1, 0, 0, 0, 0} } },
	ASPECT_RATIO_4_3},
{ {
	{720, 480, 60, TIMING_STANDARD_CEA861,	TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0, 0} },
	{858, 0, 720, 0, 16, 62, 525, 0, 480, 0, 9, 6, 27027, 2, 0,
		TIMING_STANDARD_CEA861,	TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
	{0, 0, 1, 0, 0, 0, 0} } },
	ASPECT_RATIO_4_3},
{ {
	{720, 480, 60, TIMING_STANDARD_CEA861, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0, 1} },
	{858, 0, 720, 0, 16, 62, 525, 0, 480, 0, 9, 6, 27000, 3, 0,
		TIMING_STANDARD_CEA861, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
		{0, 0, 1, 0, 0, 0, 0} } },
	ASPECT_RATIO_16_9},
{ {
	{720, 480, 60, TIMING_STANDARD_CEA861, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0, 0} },
	{858, 0, 720, 0, 16, 62, 525, 0, 480, 0, 9, 6, 27027, 3, 0,
		TIMING_STANDARD_CEA861, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
		{0, 0, 1, 0, 0, 0, 0} } },
	ASPECT_RATIO_16_9},

/* 1440x480p, 60Hz, 4:3 --> 16:9 */
{ {
	{720, 480, 60, TIMING_STANDARD_CEA861, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0, 1} },
	{1716, 0, 1440, 0, 32, 124, 525, 0, 480, 0, 8, 6, 54000, 14, 0,
		TIMING_STANDARD_CEA861, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
		{0, 0, 2, 0, 0, 0, 0} } },
	ASPECT_RATIO_4_3},
{ {
	{720, 480, 60, TIMING_STANDARD_CEA861, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0, 0} },
	{1716, 0, 1440, 0, 32, 124, 525, 0, 480, 0, 8, 6, 54054, 14, 0,
		TIMING_STANDARD_CEA861, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
		{0, 0, 2, 0, 0, 0, 0} } },
	ASPECT_RATIO_4_3},
{ {
	{720, 480, 60, TIMING_STANDARD_CEA861, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0, 1} },
	{1716, 0, 1440, 0, 32, 124, 525, 0, 480, 0, 8, 6, 54000, 15, 0,
		TIMING_STANDARD_CEA861, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
		{0, 0, 2, 0, 0, 0, 0} } },
	ASPECT_RATIO_16_9},
{ {
	{720, 480, 60, TIMING_STANDARD_CEA861, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0, 0} },
	{1716, 0, 1440, 0, 32, 124, 525, 0, 480, 0, 8, 6, 54054, 15, 0,
		TIMING_STANDARD_CEA861, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
		{0, 0, 2, 0, 0, 0, 0} } },
	ASPECT_RATIO_16_9},

/* 2880x480p, 60Hz, 4:3 --> 16:9 */
{ {
	{720, 480, 60, TIMING_STANDARD_CEA861, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0, 1} },
	{3432, 0, 2880, 0, 64, 248, 525, 0, 480, 0, 8, 6, 108000, 35, 0,
		TIMING_STANDARD_CEA861, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
		{0, 0, 4, 0, 0, 0, 0} } },
	ASPECT_RATIO_4_3},
{ {
	{720, 480, 60, TIMING_STANDARD_CEA861, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0, 0} },
	{3432, 0, 2880, 0, 64, 248, 525, 0, 480, 0, 8, 6, 108108, 35, 0,
		TIMING_STANDARD_CEA861, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
		{0, 0, 4, 0, 0, 0, 0} } },
	ASPECT_RATIO_4_3},
{ {
	{720, 480, 60, TIMING_STANDARD_CEA861, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0, 1} },
	{3432, 0, 2880, 0, 64, 248, 525, 0, 480, 0, 8, 6, 108000, 36, 0,
		TIMING_STANDARD_CEA861, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
		{0, 0, 4, 0, 0, 0, 0} } },
	ASPECT_RATIO_16_9},
{ {
	{720, 480, 60, TIMING_STANDARD_CEA861, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0, 0} },
	{3432, 0, 2880, 0, 64, 248, 525, 0, 480, 0, 8, 6, 108108, 36, 0,
		TIMING_STANDARD_CEA861, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
		{0, 0, 4, 0, 0, 0, 0} } },
	ASPECT_RATIO_16_9},

/* 720(1440)x480i, 60Hz, 4:3 --> 16:9 */
{ {
	{720, 480, 60, TIMING_STANDARD_CEA861, TIMING_SOURCE_UNDEFINED,
		{1, 0, 0, 0, 1} },
	{1716, 0, 1440, 0, 38, 124, 525, 0, 480, 0, 9, 6, 27000, 6, 0,
		TIMING_STANDARD_CEA861, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
		{1, 0, 2, 0, 0, 0, 0} } },
	ASPECT_RATIO_4_3},
{ {
	{720, 480, 60, TIMING_STANDARD_CEA861, TIMING_SOURCE_UNDEFINED,
		{1, 0, 0, 0, 0} },
	{1716, 0, 1440, 0, 38, 124, 525, 0, 480, 0, 9, 6, 27027, 6, 0,
		TIMING_STANDARD_CEA861, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
		{1, 0, 2, 0, 0, 0, 0} } },
	ASPECT_RATIO_4_3},
{ {
	{720, 480, 60, TIMING_STANDARD_CEA861, TIMING_SOURCE_UNDEFINED,
		{1, 0, 0, 0, 1} },
	{1716, 0, 1440, 0, 38, 124, 525, 0, 480, 0, 9, 6, 27000, 7, 0,
		TIMING_STANDARD_CEA861, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
		{1, 0, 2, 0, 0, 0, 0} } },
	ASPECT_RATIO_16_9},
{ {
	{720, 480, 60, TIMING_STANDARD_CEA861, TIMING_SOURCE_UNDEFINED,
		{1, 0, 0, 0, 0} },
	{1716, 0, 1440, 0, 38, 124, 525, 0, 480, 0, 9, 6, 27027, 7, 0,
		TIMING_STANDARD_CEA861, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
		{1, 0, 2, 0, 0, 0, 0} } },
	ASPECT_RATIO_16_9},

/* 2880x480i, 60Hz, 4:3 --> 16:9 */
{ {
	{720, 480, 60, TIMING_STANDARD_CEA861, TIMING_SOURCE_UNDEFINED,
		{1, 0, 0, 0, 1} },
	{3432, 0, 2880, 0, 76, 248, 525, 0, 480, 0, 5, 6, 54000, 10, 0,
		TIMING_STANDARD_CEA861, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
		{1, 0, 4, 0, 0, 0, 0} } },
	ASPECT_RATIO_4_3},
{ {
	{720, 480, 60, TIMING_STANDARD_CEA861, TIMING_SOURCE_UNDEFINED,
		{1, 0, 0, 0, 0} },
	{3432, 0, 2880, 0, 76, 248, 525, 0, 480, 0, 9, 6, 54054, 10, 0,
		TIMING_STANDARD_CEA861, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
		{1, 0, 4, 0, 0, 0, 0} } },
	ASPECT_RATIO_4_3},
{ {
	{720, 480, 60, TIMING_STANDARD_CEA861, TIMING_SOURCE_UNDEFINED,
		{1, 0, 0, 0, 1} },
	{3432, 0, 2880, 0, 76, 248, 525, 0, 480, 0, 5, 6, 54000, 11, 0,
		TIMING_STANDARD_CEA861, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
		{1, 0, 4, 0, 0, 0, 0} } },
	ASPECT_RATIO_16_9},
{ {
	{720, 480, 60, TIMING_STANDARD_CEA861, TIMING_SOURCE_UNDEFINED,
		{1, 0,	0, 0, 0} },
	{3432, 0, 2880, 0, 76, 248, 525, 0, 480, 0, 9, 6, 54054, 11, 0,
		TIMING_STANDARD_CEA861, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
		{1, 0, 4, 0, 0, 0, 0} } },
	ASPECT_RATIO_16_9},

/* 720x480p, 4:3, 120Hz, 4:3 --> 16:9 */
{ {
	{ 720, 480, 120, TIMING_STANDARD_CEA861, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0, 1} },
	{858, 0, 720, 0, 16, 62, 525, 0, 480, 0, 9, 6, 54000, 48, 0,
		TIMING_STANDARD_CEA861, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
		{0, 0, 1, 0, 0, 0, 0} } },
	ASPECT_RATIO_4_3},
{ {
	{ 720, 480, 120, TIMING_STANDARD_CEA861, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0, 0} },
	{858, 0, 720, 0, 16, 62, 525, 0, 480, 0, 9, 6, 54054, 48, 0,
		TIMING_STANDARD_CEA861, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
		{0, 0, 1, 0, 0, 0, 0} } },
	ASPECT_RATIO_4_3},
{ {
	{ 720, 480, 120, TIMING_STANDARD_CEA861, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0, 1} },
	{858, 0, 720, 0, 16, 62, 525, 0, 480, 0, 9, 6, 54000, 49, 0,
		TIMING_STANDARD_CEA861, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
		{0, 0, 1, 0, 0, 0, 0} } },
	ASPECT_RATIO_16_9},
{ {
	{720, 480, 120,
		TIMING_STANDARD_CEA861, TIMING_SOURCE_UNDEFINED,
{0, 0, 0, 0, 0} }, {
		858, 0, 720, 0, 16, 62, 525, 0,
		480, 0, 9, 6, 54054, 49, 0,
		TIMING_STANDARD_CEA861, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
		{0, 0, 1, 0, 0, 0, 0} } },
	ASPECT_RATIO_16_9},

/* 720(1440)x480i, 120Hz, 4:3 --> 16:9 */
{ {
	{720, 480, 120, TIMING_STANDARD_CEA861, TIMING_SOURCE_UNDEFINED,
		{1, 0, 0, 0, 1} },
	{1716, 0, 1440, 0, 38, 124, 525,
		0, 480, 0, 9, 6, 54000, 50, 0,
		TIMING_STANDARD_CEA861, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
		{1, 0, 2, 0, 0, 0, 0} } },
	ASPECT_RATIO_4_3},
{ {
	{720, 480, 120, TIMING_STANDARD_CEA861, TIMING_SOURCE_UNDEFINED,
		{1, 0, 0, 0, 0} }, {
		1716, 0, 1440, 0, 38, 124, 525,
		0, 480, 0, 9, 6, 54054, 50, 0,
		TIMING_STANDARD_CEA861, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
		{1, 0, 2, 0, 0, 0, 0} } },
	ASPECT_RATIO_4_3},
{ {
	{720, 480, 120, TIMING_STANDARD_CEA861, TIMING_SOURCE_UNDEFINED,
		{1, 0, 0, 0, 1} },
	{1716, 0, 1440, 0, 38, 124, 525,
		0, 480, 0, 9, 6, 54000, 51, 0,
		TIMING_STANDARD_CEA861, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
		{1, 0, 2, 0, 0, 0, 0} } },
	ASPECT_RATIO_16_9},
{ {
	{720, 480, 120, TIMING_STANDARD_CEA861, TIMING_SOURCE_UNDEFINED,
		{1, 0, 0, 0, 0} },
	{1716, 0, 1440, 0, 38, 124, 525,
		0, 480, 0, 9, 6, 54054, 51, 0,
		TIMING_STANDARD_CEA861, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
		{1, 0, 2, 0, 0, 0, 0} } },
	ASPECT_RATIO_16_9},

/* 720x480p, 4:3, 240Hz, 4:3 --> 16:9 */
{ {
	{720, 480, 240, TIMING_STANDARD_CEA861, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0, 1} },
	{858, 0, 720, 0, 16, 62, 525, 0, 480, 0, 9, 6, 108000, 56, 0,
		TIMING_STANDARD_CEA861, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
		{0, 0, 1, 0, 0, 0, 0} } },
	ASPECT_RATIO_4_3},
{ {
	{720, 480, 240, TIMING_STANDARD_CEA861, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0, 0} }, {
		858, 0, 720, 0, 16, 62, 525, 0,
		480, 0, 9, 6, 108108, 56, 0,
		TIMING_STANDARD_CEA861, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
		{0, 0, 1, 0, 0, 0, 0} } },
	ASPECT_RATIO_4_3},
{ {
	{720, 480, 240, TIMING_STANDARD_CEA861, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0, 1} },
	{858, 0, 720, 0, 16, 62, 525, 0, 480, 0, 9, 6, 108000, 57, 0,
		TIMING_STANDARD_CEA861, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
		{0, 0, 1, 0, 0, 0, 0} } },
	ASPECT_RATIO_16_9},
{ {
	{720, 480, 240, TIMING_STANDARD_CEA861, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0, 0} },
	{858, 0, 720, 0, 16, 62, 525, 0, 480, 0, 9, 6, 108108, 57, 0,
		TIMING_STANDARD_CEA861, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
		{0, 0, 1, 0, 0, 0, 0} } },
	ASPECT_RATIO_16_9},

/* 720(1440)x480i, 240Hz, 4:3 --> 16:9 */
{ {
	{720, 480, 240, TIMING_STANDARD_CEA861, TIMING_SOURCE_UNDEFINED,
		{1, 0, 0, 0, 1} },
	{1716, 0, 1440, 0, 38, 124, 525,
		0, 480, 0, 9, 6, 108000, 58, 0,
		TIMING_STANDARD_CEA861, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
		{1, 0, 2, 0, 0, 0, 0} } },
	ASPECT_RATIO_4_3},
{ {
	{720, 480, 240, TIMING_STANDARD_CEA861, TIMING_SOURCE_UNDEFINED,
		{1, 0, 0, 0, 0} },
	{1716, 0, 1440, 0, 38, 124, 525, 0, 480, 0, 9, 6, 108108, 58, 0,
		TIMING_STANDARD_CEA861, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
		{1, 0, 2, 0, 0, 0, 0} } },
	ASPECT_RATIO_4_3},
{ {
	{720, 480, 240, TIMING_STANDARD_CEA861, TIMING_SOURCE_UNDEFINED,
		{1, 0, 0, 0, 1} },
	{1716, 0, 1440, 0, 38, 124, 525, 0, 480, 0, 9, 6, 108000, 59, 0,
		TIMING_STANDARD_CEA861, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
		{1, 0, 2, 0, 0, 0, 0} } },
	ASPECT_RATIO_16_9},
{ {
	{720, 480, 240, TIMING_STANDARD_CEA861, TIMING_SOURCE_UNDEFINED,
		{1, 0, 0, 0, 0} },
	{1716, 0, 1440, 0, 38, 124, 525, 0, 480, 0, 9, 6, 108108, 59, 0,
		TIMING_STANDARD_CEA861, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
		{1, 0, 2, 0, 0, 0, 0} } },
	ASPECT_RATIO_16_9},

/* 720x576p, 50Hz, 4:3 --> 16:9 */
{ {
	{720, 576, 50, TIMING_STANDARD_CEA861, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0, 0} },
	{864, 0, 720, 0, 12, 64, 625, 0, 576, 0, 5, 5, 27000, 17, 0,
		TIMING_STANDARD_CEA861, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
		{0, 0, 1, 0, 0, 0, 0} } },
	ASPECT_RATIO_4_3},
{ {
	{720,
		576, 50, TIMING_STANDARD_CEA861, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0, 0} },
	{864, 0, 720, 0, 12, 64, 625, 0, 576, 0, 5, 5, 27000, 18, 0,
		TIMING_STANDARD_CEA861, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
		{0, 0, 1, 0, 0, 0, 0} } },
	ASPECT_RATIO_16_9},

/* 1440x576p, 50Hz, 4:3 --> 16:9 */
{ {
	{720, 576, 50, TIMING_STANDARD_CEA861, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0, 0} },
	{1728, 0, 1440, 0, 24, 128, 625,
		0, 576, 0, 5, 5, 54000, 29, 0,
		TIMING_STANDARD_CEA861, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
		{0, 0, 2, 0, 0, 0, 0} } },
	ASPECT_RATIO_4_3},
{ {
	{720,
		576, 50, TIMING_STANDARD_CEA861, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0, 0} },
	{1728, 0, 1440, 0, 24, 128, 625,
		0, 576, 0, 5, 5, 54000, 30, 0,
		TIMING_STANDARD_CEA861, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
		{0, 0, 2, 0, 0, 0, 0} } },
	ASPECT_RATIO_16_9},

/* 2880x576p, 50Hz, 4:3 --> 16:9 */
{ {
	{720, 576, 50, TIMING_STANDARD_CEA861, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0, 0} },
	{3456, 0, 2880, 0, 48, 256, 625,
		0, 576, 0, 5, 5, 108000, 37, 0,
		TIMING_STANDARD_CEA861, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
		{0, 0, 4, 0, 0, 0, 0} } },
	ASPECT_RATIO_4_3},
{ {
	{720,
		576, 50, TIMING_STANDARD_CEA861, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0, 0} },
	{3456, 0, 2880, 0, 48, 256, 625,
		0, 576, 0, 5, 5, 108000, 38, 0,
		TIMING_STANDARD_CEA861, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
		{0, 0, 4, 0, 0, 0, 0} } },
	ASPECT_RATIO_16_9},

/* 720(1440)x576i, 50Hz, 4:3 --> 16:9 */
{ {
	{720, 576, 50, TIMING_STANDARD_CEA861, TIMING_SOURCE_UNDEFINED,
		{1, 0, 0, 0, 0} },
	{1728, 0, 1440, 0, 24, 126, 625,
		0, 576, 0, 5, 6, 27000, 21, 0,
		TIMING_STANDARD_CEA861, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
		{1, 0, 2, 0, 0, 0, 0} } },
	ASPECT_RATIO_4_3},
{ {
	{720,
		576, 50, TIMING_STANDARD_CEA861, TIMING_SOURCE_UNDEFINED,
		{1, 0, 0, 0, 0} },
	{1728, 0, 1440, 0, 24, 126, 625,
		0, 576, 0, 5, 6, 27000, 22, 0,
		TIMING_STANDARD_CEA861, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
		{1, 0, 2, 0, 0, 0, 0} } },
	ASPECT_RATIO_16_9},

/* 2880x576i, 50Hz, 4:3 --> 16:9 */
{ {
	{720, 576, 50, TIMING_STANDARD_CEA861, TIMING_SOURCE_UNDEFINED,
		{1, 0, 0, 0, 0} },
	{3456, 0, 2880, 0, 48, 252, 625,
		0, 576, 0, 5, 6, 54000, 25, 0,
		TIMING_STANDARD_CEA861, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
		{1, 0, 4, 0, 0, 0, 0} } },
	ASPECT_RATIO_4_3},
{ {
	{720,
		576, 50, TIMING_STANDARD_CEA861, TIMING_SOURCE_UNDEFINED,
		{1, 0, 0, 0, 0} },
	{3456, 0, 2880, 0, 48, 252, 625,
		0, 576, 0, 5, 6, 54000, 26, 0,
		TIMING_STANDARD_CEA861, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
		{1, 0, 4, 0, 0, 0, 0} } },
	ASPECT_RATIO_16_9},

/* 720x576p, 100Hz, 4:3 --> 16:9 */
{ {
	{720, 576, 100, TIMING_STANDARD_CEA861, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0, 0} },
	{864, 0, 720, 0, 12, 64, 625, 0, 576, 0, 5, 5, 54000, 42, 0,
		TIMING_STANDARD_CEA861, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
		{0, 0, 1, 0, 0, 0, 0} } },
	ASPECT_RATIO_4_3},
{ {
	{720, 576, 100, TIMING_STANDARD_CEA861, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0, 0} },
	{864, 0, 720, 0, 12, 64, 625, 0, 576, 0, 5, 5, 54000, 43, 0,
		TIMING_STANDARD_CEA861, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
		{0, 0, 1, 0, 0, 0, 0} } },
	ASPECT_RATIO_16_9},

/* 720(1440)x576i, 100Hz, 4:3 --> 16:9 */
{ {
	{720, 576, 100, TIMING_STANDARD_CEA861, TIMING_SOURCE_UNDEFINED,
		{1, 0, 0, 0, 0} },
	{1728, 0, 1440, 0, 24, 126, 625,
		0, 576, 0, 5, 6, 54000, 44, 0,
		TIMING_STANDARD_CEA861, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
		{1, 0, 2, 0, 0, 0, 0} } },
	ASPECT_RATIO_4_3},
{ {
	{720, 576, 100, TIMING_STANDARD_CEA861, TIMING_SOURCE_UNDEFINED,
		{1, 0, 0, 0, 0} },
	{1728, 0, 1440, 0, 24, 126, 625,
		0, 576, 0, 5, 6, 54000, 45, 0,
		TIMING_STANDARD_CEA861, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
		{1, 0, 2, 0, 0, 0, 0} } },
	ASPECT_RATIO_16_9},

/* 720x576p, 200Hz, 4:3 --> 16:9 */
{ {
	{720, 576, 200, TIMING_STANDARD_CEA861, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0, 0} },
	{864, 0, 720, 0, 12, 64, 625, 0, 576, 0, 5, 5, 108000, 52, 0,
		TIMING_STANDARD_CEA861, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
		{0, 0, 1, 0, 0, 0, 0} } },
	ASPECT_RATIO_4_3},
{ {
	{720, 576, 200, TIMING_STANDARD_CEA861, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0, 0} },
	{864, 0, 720, 0, 12, 64, 625, 0, 576, 0, 5, 5, 108000, 53, 0,
		TIMING_STANDARD_CEA861, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
		{0, 0, 1, 0, 0, 0, 0} } },
	ASPECT_RATIO_16_9},

/* 720(1440)x576i, 200Hz, 4:3 --> 16:9 */
{ {
	{720, 576, 200, TIMING_STANDARD_CEA861, TIMING_SOURCE_UNDEFINED,
		{1, 0, 0, 0, 0} },
	{1728, 0, 1440, 0, 24, 126, 625,
		0, 576, 0, 5, 6, 108000, 54, 0,
		TIMING_STANDARD_CEA861, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
		{1, 0, 2, 0, 0, 0, 0} } },
	ASPECT_RATIO_4_3},
{ {
	{720, 576, 200, TIMING_STANDARD_CEA861, TIMING_SOURCE_UNDEFINED,
		{1, 0, 0, 0, 0} },
	{1728, 0, 1440, 0, 24, 126, 625,
		0, 576, 0, 5, 6, 108000, 55, 0,
		TIMING_STANDARD_CEA861, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
		{1, 0, 2, 0, 0, 0, 0} } },
	ASPECT_RATIO_16_9},
/* 1280x720p, 24Hz, 16:9 */
{ {
	{1280, 720, 24, TIMING_STANDARD_CEA861, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0, 1} },
	{3300, 0, 1280, 0, 1760, 40, 750, 0, 720, 0, 5, 5, 59400, 60, 0,
		TIMING_STANDARD_CEA861, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
		{0, 0, 1, 1, 1, 0, 0} } },
	ASPECT_RATIO_16_9},
{ {
	{1280, 720, 24, TIMING_STANDARD_CEA861, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0, 0} },
	{ 3300, 0, 1280, 0, 1760, 40, 750, 0, 720, 0, 5, 5, 59400, 60, 0,
		TIMING_STANDARD_CEA861, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
		{0, 0, 1, 1, 1, 0, 0} } },
		ASPECT_RATIO_16_9},

/* 1280x720p, 25Hz, 16:9 */
{ {
	{ 1280, 720, 25, TIMING_STANDARD_CEA861, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0, 0} },
	{ 3960, 0, 1280, 0, 2420, 40, 750, 0, 720, 0, 5, 5, 74250, 61, 0,
		TIMING_STANDARD_CEA861, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
		{0, 0, 1, 1, 1, 0, 0} } },
	ASPECT_RATIO_16_9},

/* 1280x720p, 30Hz, 16:9 */
{ {
	{ 1280, 720, 30, TIMING_STANDARD_CEA861, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0, 1} },
	{ 3300, 0, 1280, 0, 1760, 40, 750, 0, 720, 0, 5, 5, 74176, 62, 0,
		TIMING_STANDARD_CEA861, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
		{0, 0, 1, 1, 1, 0, 0} } },
	ASPECT_RATIO_16_9},
{ {
	{1280, 720, 30, TIMING_STANDARD_CEA861, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0, 0} },
	{ 3300, 0, 1280, 0, 1760, 40, 750, 0, 720, 0, 5, 5, 74250, 62, 0,
		TIMING_STANDARD_CEA861, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
		{0, 0, 1, 1, 1, 0, 0} } },
	ASPECT_RATIO_16_9},

/* 1280x720p, 50Hz, 16:9 */
{ {
	{1280, 720, 50, TIMING_STANDARD_CEA861, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0, 0} },
	{1980, 0, 1280, 0, 440, 40, 750,
		0, 720, 0, 5, 5, 74250, 19, 0,
		TIMING_STANDARD_CEA861, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
		{0, 0, 1, 1, 1, 0, 0} } },
	ASPECT_RATIO_16_9},

/* 1280x720p, 60Hz, 16:9 */
{ {
	{1280, 720, 60, TIMING_STANDARD_CEA861, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0, 1} },
	{1650, 0, 1280, 0, 110, 40, 750,
		0, 720, 0, 5, 5, 74176, 4, 0,
		TIMING_STANDARD_CEA861, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
		{0, 0, 1, 1, 1, 0, 0} } },
	ASPECT_RATIO_16_9},
{ {
	{1280,
		720, 60, TIMING_STANDARD_CEA861, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0, 0} },
	{1650, 0, 1280, 0, 110, 40, 750,
		0, 720, 0, 5, 5, 74250, 4, 0,
		TIMING_STANDARD_CEA861, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
		{0, 0, 1, 1, 1, 0, 0} } },
	ASPECT_RATIO_16_9},

/* 1280x720p, 100Hz, 16:9 */
{ {
	{1280, 720, 100, TIMING_STANDARD_CEA861, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0, 0} },
	{1980, 0, 1280, 0, 440, 40, 750,
		0, 720, 0, 5, 5, 148500, 41, 0,
		TIMING_STANDARD_CEA861, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
		{0, 0, 1, 1, 1, 0, 0} } },
	ASPECT_RATIO_16_9},

/* 1280x720p, 120Hz, 16:9 */
{ {
	{1280, 720, 120, TIMING_STANDARD_CEA861, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0, 1} },
	{1650, 0, 1280, 0, 110, 40, 750,
		0, 720, 0, 5, 5, 148352, 47, 0,
		TIMING_STANDARD_CEA861, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
		{0, 0, 1, 1, 1, 0, 0} } },
	ASPECT_RATIO_16_9},
{ {
	{1280, 720, 120, TIMING_STANDARD_CEA861, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0, 0} },
	{1650, 0, 1280, 0, 110, 40, 750,
		0, 720, 0, 5, 5, 148500, 47, 0,
		TIMING_STANDARD_CEA861, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
		{0, 0, 1, 1, 1, 0, 0} } },
	ASPECT_RATIO_16_9},

/* 1920x1080p, 24Hz, 16:9 */
{ {
	{1920, 1080, 24, TIMING_STANDARD_CEA861, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0, 1} },
	{2750, 0, 1920, 0, 638, 44, 1125,
		0, 1080, 0, 4, 5, 74176, 32, 0,
		TIMING_STANDARD_CEA861, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
		{0, 0, 1, 1, 1, 0, 0} } },
	ASPECT_RATIO_16_9},
{ {
	{1920, 1080, 24, TIMING_STANDARD_CEA861, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0, 0} },
	{2750, 0, 1920, 0, 638, 44, 1125,
		0, 1080, 0, 4, 5, 74250, 32, 0,
		TIMING_STANDARD_CEA861, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
		{0, 0, 1, 1, 1, 0, 0} } },
	ASPECT_RATIO_16_9},

/* 1920x1080p, 25Hz, 16:9 */
{ {
	{1920, 1080, 25, TIMING_STANDARD_CEA861, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0, 0} },
	{2640, 0, 1920, 0, 528, 44, 1125,
		0, 1080, 0, 4, 5, 74250, 33, 0,
		TIMING_STANDARD_CEA861, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
		{0, 0, 1, 1, 1, 0, 0} } },
	ASPECT_RATIO_16_9},

/* 1920x1080p, 30Hz, 16:9 */
{ {
	{1920, 1080, 30, TIMING_STANDARD_CEA861, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0, 1} },
	{2200, 0, 1920, 0, 88, 44, 1125,
		0, 1080, 0, 4, 5, 74176, 34, 0,
		TIMING_STANDARD_CEA861, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
		{0, 0, 1, 1, 1, 0, 0} } },
	ASPECT_RATIO_16_9},
{ {
	{1920, 1080, 30, TIMING_STANDARD_CEA861, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0, 0} },
	{2200, 0, 1920, 0, 88, 44, 1125,
		0, 1080, 0, 4, 5, 74250, 34, 0,
		TIMING_STANDARD_CEA861, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
		{0, 0, 1, 1, 1, 0, 0} } },
	ASPECT_RATIO_16_9},

/* 1920x1080p/i, 50Hz, 16:9 */
{ {
	{1920, 1080, 50, TIMING_STANDARD_CEA861, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0, 0} },
	{2640, 0, 1920, 0, 528, 44, 1125,
		0, 1080, 0, 4, 5, 148500, 31, 0,
		TIMING_STANDARD_CEA861, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
		{0, 0, 1, 1, 1, 0, 0} } },
	ASPECT_RATIO_16_9},
{ {
	{1920, 1080, 50, TIMING_STANDARD_CEA861, TIMING_SOURCE_UNDEFINED,
		{1, 0, 0, 0, 0} },
	{2640, 0, 1920, 0, 528, 44, 1125, 0, 1080, 0, 5, 10, 74250, 20, 0,
		TIMING_STANDARD_CEA861, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
		{1, 0, 1, 1, 1, 0, 0} } },
	ASPECT_RATIO_16_9},
{ {
	{1920, 1080, 50, TIMING_STANDARD_CEA861, TIMING_SOURCE_UNDEFINED,
		{1, 0, 0, 0, 0} },
	{2304, 0, 1920, 0, 32, 168, 1250, 0, 1080, 0, 5, 10, 72000, 39, 0,
		TIMING_STANDARD_CEA861, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
		{1, 0, 1, 1, 0, 0, 0} } },
	ASPECT_RATIO_16_9},

/* 1920x1080p/i, 60Hz, 16:9 */
{ {
	{1920, 1080, 60, TIMING_STANDARD_CEA861, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0, 1} },
	{2200, 0, 1920, 0, 88, 44, 1125, 0, 1080, 0, 4, 5, 148352, 16, 0,
		TIMING_STANDARD_CEA861, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
		{0, 0, 1, 1, 1, 0, 0} } },
	ASPECT_RATIO_16_9},
{ {
	{1920, 1080, 60, TIMING_STANDARD_CEA861, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0, 0} },
	{2200, 0, 1920, 0, 88, 44, 1125, 0, 1080, 0, 4, 5, 148500, 16, 0,
		TIMING_STANDARD_CEA861, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
		{0, 0, 1, 1, 1, 0, 0} } },
	ASPECT_RATIO_16_9},
{ {
	{1920, 1080, 60, TIMING_STANDARD_CEA861, TIMING_SOURCE_UNDEFINED,
		{1, 0, 0, 0, 1} },
	{2200, 0, 1920, 0, 88, 44, 1125, 0, 1080, 0, 5, 10, 74176, 5, 0,
		TIMING_STANDARD_CEA861, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
		{1, 0, 1, 1, 1, 0, 0} } },
	ASPECT_RATIO_16_9},
{ {
	{1920, 1080, 60, TIMING_STANDARD_CEA861, TIMING_SOURCE_UNDEFINED,
		{1, 0, 0, 0, 0} },
	{2200, 0, 1920, 0, 88, 44, 1125, 0, 1080, 0, 5, 10, 74250, 5, 0,
		TIMING_STANDARD_CEA861, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
		{1, 0, 1, 1, 1, 0, 0} } },
	ASPECT_RATIO_16_9},

/* 1920x1080p/i, 100Hz, 16:9 */
{ {
	{1920, 1080, 100, TIMING_STANDARD_CEA861, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0, 0} },
	{2640, 0, 1920, 0, 528, 44, 1125, 0, 1080, 0, 4, 5, 297000, 64, 0,
		TIMING_STANDARD_CEA861, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
		{0, 0, 1, 1, 1, 0, 0} } },
	ASPECT_RATIO_16_9},
{ {
	{1920, 1080, 100, TIMING_STANDARD_CEA861, TIMING_SOURCE_UNDEFINED,
		{1, 0, 0, 0, 0} },
	{2640, 0, 1920, 0, 528, 44, 1125, 0, 1080, 0, 5, 10, 148500, 40, 0,
		TIMING_STANDARD_CEA861, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
		{1, 0, 1, 1, 1, 0, 0} } },
	ASPECT_RATIO_16_9},

/* 1920x1080p/i, 120Hz, 16:9 */
{ {
	{1920, 1080, 120, TIMING_STANDARD_CEA861, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0, 1} },
	{2200, 0, 1920, 0, 88, 44, 1125, 0, 1080, 0, 4, 5, 296704, 63, 0,
		TIMING_STANDARD_CEA861, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
		{0, 0, 1, 1, 1, 0, 0} } },
	ASPECT_RATIO_16_9},
{ {
	{1920, 1080, 120, TIMING_STANDARD_CEA861, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0, 0} },
	{2200, 0, 1920, 0, 88, 44, 1125, 0, 1080, 0, 4, 5, 297000, 63, 0,
		TIMING_STANDARD_CEA861, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
		{0, 0, 1, 1, 1, 0, 0} } },
	ASPECT_RATIO_16_9},
{ {
	{1920, 1080, 120, TIMING_STANDARD_CEA861, TIMING_SOURCE_UNDEFINED,
		{1, 0, 0, 0, 1} },
	{2200, 0, 1920, 0, 88, 44, 1125, 0, 1080, 0, 5, 10, 148352, 46, 0,
		TIMING_STANDARD_CEA861, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
		{1, 0, 1, 1, 1, 0, 0} } },
	ASPECT_RATIO_16_9},
{ {
	{1920, 1080, 120, TIMING_STANDARD_CEA861, TIMING_SOURCE_UNDEFINED,
		{1, 0, 0, 0, 0} },
	{2200, 0, 1920, 0, 88, 44, 1125, 0, 1080, 0, 5, 10, 148500, 46, 0,
		TIMING_STANDARD_CEA861, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
		{1, 0, 1, 1, 1, 0, 0} } },
	ASPECT_RATIO_16_9},};

#define MODE_COUNT	ARRAY_SIZE(mode_timings)

/**
 * Get the number of modes reported by this source
 *
 * @return the number of modes supported by the timing source, possibly zero
 */
static uint32_t get_mode_timing_count(void)
{
	return MODE_COUNT;
}

/**
 * Enumerate mode timing
 *
 * @param index the index of the mode timing to retrieve
 * @return pointer to timing if mode timing was found successfully, NULL otherwise
 */
static const struct mode_timing *get_mode_timing_by_index(uint32_t index)
{
	if (index < MODE_COUNT)
		return &(mode_timings[index].mode_timing);

	return NULL;
}

/**
 * Query the timing for a particular mode
 *
 * @param mode mode to retrieve timing for
 * @param crtc_timing the retrieved timing
 * @return true if the timing was found successfully, false otherwise
 */
static bool get_timing_for_mode(
		const struct mode_info *mode,
		struct crtc_timing *crtc_timing)
{
	uint32_t i;
	if (crtc_timing == NULL)
		return false;
	for (i = 0; i < MODE_COUNT; ++i)
		if (dal_mode_info_is_equal(
				mode,
				&(mode_timings[i].mode_timing.mode_info))) {
			*crtc_timing = mode_timings[i].mode_timing.crtc_timing;
			return true;
		}

	return false;
}

/**
 * Query used to determine if a timing is compatible with this source
 *
 * @param crtc_timing the timing to verify if it is compatible with this timing standard
 * @return true if the timing matches some timing within this standard, false if there is not match.
 */
static bool is_timing_in_standard(const struct crtc_timing *crtc_timing)
{
	uint32_t i;
	struct crtc_timing modified_timing = *crtc_timing;
	modified_timing.timing_3d_format = TIMING_3D_FORMAT_NONE;
	modified_timing.display_color_depth = DISPLAY_COLOR_DEPTH_UNDEFINED;
	modified_timing.pixel_encoding = PIXEL_ENCODING_UNDEFINED;
	modified_timing.flags.EXCLUSIVE_3D = false;
	modified_timing.flags.RIGHT_EYE_3D_POLARITY = false;
	modified_timing.flags.SUB_SAMPLE_3D = false;
	if (modified_timing.flags.PIXEL_REPETITION == 0)
		modified_timing.flags.PIXEL_REPETITION = 1;

	for (i = 0; i < MODE_COUNT; ++i)
		if (dal_crtc_timing_is_equal(
				&modified_timing,
				&(mode_timings[i].mode_timing.crtc_timing)))
			return true;

	return false;
}


/**
 * Query the aspect ratio for a particular timing
 *
 * @param crtc_timing the timing to query aspect ratio
 * @return the aspec ratio
 */
static enum aspect_ratio get_aspect_ratio_for_timing(
		const struct crtc_timing *crtc_timing)
{
	uint32_t index;
	for (index = 0; index < MODE_COUNT; index++) {
		if (crtc_timing->vic == mode_timings[index].mode_timing.
								crtc_timing.vic)
			return mode_timings[index].aspect_ratio;
	}
	return dal_mode_timing_source_get_aspect_ratio_for_timing(crtc_timing);
}

struct mode_timing_source_funcs *dal_mode_timing_source_cea861e_create(void)
{
	struct mode_timing_source_funcs *mts = dal_alloc(
			sizeof(struct mode_timing_source_funcs));
	if (mts == NULL)
		return NULL;

	if (!dal_mode_timing_source_construct(mts))
		goto out_free;
	mts->is_timing_in_standard = is_timing_in_standard;
	mts->get_timing_for_mode = get_timing_for_mode;
	mts->get_mode_timing_by_index = get_mode_timing_by_index;
	mts->get_mode_timing_count = get_mode_timing_count;
	mts->get_aspect_ratio_for_timing = get_aspect_ratio_for_timing;

	return mts;
out_free:
	dal_free(mts);
	return NULL;
}
