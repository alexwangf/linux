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

static const struct mode_timing mode_timings[] = {
{
	{640, 350, 85, TIMING_STANDARD_DMT, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0} },
	{832, 0, 640, 0, 32, 64, 445, 0, 350, 0, 32, 3, 31500, 0, 0,
		TIMING_STANDARD_DMT, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
			{0, 0, 0, 1, 0, 0, 0} } },
{
	{640, 400, 85, TIMING_STANDARD_DMT, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0} },
	{832, 0, 640, 0, 32, 64, 445, 0, 400, 0, 1, 3, 31500, 0, 0,
		TIMING_STANDARD_DMT, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
			{0, 0, 0, 0, 1, 0, 0} } },
{
	{640, 480, 60, TIMING_STANDARD_DMT, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0, 1} },
	{800, 0, 640, 0, 16, 96, 525, 0, 480, 0, 10, 2, 25175, 0, 0,
		TIMING_STANDARD_DMT, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
			{0, 0, 0, 0, 0, 0, 0} } },
{
	{640, 480, 72, TIMING_STANDARD_DMT, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0} },
	{832, 8, 640, 8, 16, 40, 520, 8, 480, 8, 1, 3, 31500, 0, 0,
		TIMING_STANDARD_DMT, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
			{0, 0, 0, 0, 0, 0, 0} } },
{
	{640, 480, 75, TIMING_STANDARD_DMT, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0} },
	{840, 0, 640, 0, 16, 64, 500, 0, 480, 0, 1, 3, 31500, 0, 0,
		TIMING_STANDARD_DMT, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
			{0, 0, 0, 0, 0, 0, 0} } },
{
	{640, 480, 85, TIMING_STANDARD_DMT, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0} },
	{832, 0, 640, 0, 56, 56, 509, 0, 480, 0, 1, 3, 36000, 0, 0,
		TIMING_STANDARD_DMT, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
			{0, 0, 0, 0, 0, 0, 0} } },
{
	{720, 400, 85, TIMING_STANDARD_DMT, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0} },
	{936, 0, 720, 0, 36, 72, 446, 0, 400, 0, 1, 3, 35500, 0, 0,
		TIMING_STANDARD_DMT, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
			{0, 0, 0, 0, 1, 0, 0} } },
{
	{800, 600, 56, TIMING_STANDARD_DMT, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0} },
	{1024, 0, 800, 0, 24, 72, 625, 0, 600, 0, 1, 2, 36000, 0, 0,
		TIMING_STANDARD_DMT, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
			{0, 0, 0, 1, 1, 0, 0} } },
{
	{800, 600, 60, TIMING_STANDARD_DMT, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0} },
	{1056, 0, 800, 0, 40, 128, 628, 0, 600, 0, 1, 4, 40000, 0, 0,
		TIMING_STANDARD_DMT, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
			{0, 0, 0, 1, 1, 0, 0} } },
{
	{800, 600, 72, TIMING_STANDARD_DMT, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0} },
	{1040, 0, 800, 0, 56, 120, 666, 0, 600, 0, 37, 6, 50000, 0, 0,
		TIMING_STANDARD_DMT, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
			{0, 0, 0, 1, 1, 0, 0} } },
{
	{800, 600, 75, TIMING_STANDARD_DMT, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0} },
	{1056, 0, 800, 0, 16, 80, 625, 0, 600, 0, 1, 3, 49500, 0, 0,
		TIMING_STANDARD_DMT, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
			{0, 0, 0, 1, 1, 0, 0} } },
{
	{800, 600, 85, TIMING_STANDARD_DMT, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0} },
	{1048, 0, 800, 0, 32, 64, 631, 0, 600, 0, 1, 3, 56250, 0, 0,
		TIMING_STANDARD_DMT, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
			{0, 0, 0, 1, 1, 0, 0} } },
{
	{800, 600, 120, TIMING_STANDARD_DMT, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 1} },
	{960, 0, 800, 0, 48, 32, 636, 0, 600, 0, 3, 4, 73250, 0, 0,
		TIMING_STANDARD_DMT, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
			{0, 0, 0, 1, 0, 0, 0} } },
{
	{848, 480, 60, TIMING_STANDARD_DMT, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0} },
	{1088, 0, 848, 0, 16, 112, 517, 0, 480, 0, 6, 8, 33750, 0, 0,
		TIMING_STANDARD_DMT, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
			{0, 0, 0, 1, 1, 0, 0} } },
{
	{1024, 768, 60, TIMING_STANDARD_DMT, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0} },
	{1344, 0, 1024, 0, 24, 136, 806, 0, 768, 0, 3, 6, 65000, 0, 0,
		TIMING_STANDARD_DMT, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
			{0, 0, 0, 0, 0, 0, 0} } },
{
	{1024, 768, 70, TIMING_STANDARD_DMT, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0} },
	{1328, 0, 1024, 0, 24, 136, 806, 0, 768, 0, 3, 6, 75000, 0, 0,
		TIMING_STANDARD_DMT, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
			{0, 0, 0, 0, 0, 0, 0} } },
{
	{1024, 768, 75, TIMING_STANDARD_DMT, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0} },
	{1312, 0, 1024, 0, 16, 96, 800, 0, 768, 0, 1, 3, 78750, 0, 0,
		TIMING_STANDARD_DMT, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
			{0, 0, 0, 1, 1, 0, 0} } },
{
	{1024, 768, 87, TIMING_STANDARD_DMT, TIMING_SOURCE_UNDEFINED,
		{1, 0, 0, 0} },
	{1264, 0, 1024, 0, 8, 176, 817, 0, 768, 0, 1, 8, 44900, 0, 0,
		TIMING_STANDARD_DMT, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
			{1, 0, 0, 1, 1, 0, 0} } },
{
	{1024, 768, 85, TIMING_STANDARD_DMT, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0} },
	{1376, 0, 1024, 0, 48, 96, 808, 0, 768, 0, 1, 3, 94500, 0, 0,
		TIMING_STANDARD_DMT, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
			{0, 0, 0, 1, 1, 0, 0} } },
{
	{1024, 768, 120, TIMING_STANDARD_DMT, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 1} },
	{1184, 0, 1024, 0, 48, 32, 813, 0, 768, 0, 3, 4, 115500, 0, 0,
		TIMING_STANDARD_DMT, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
			{0, 0, 0, 1, 0, 0, 0} } },
{
	{1152, 864, 75, TIMING_STANDARD_DMT, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0} },
	{1600, 0, 1152, 0, 64, 128, 900, 0, 864, 0, 1, 3, 108000, 0, 0,
		TIMING_STANDARD_DMT, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
			{0, 0, 0, 1, 1, 0, 0} } },
{
	{1280, 720, 60, TIMING_STANDARD_CEA861, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 1} },
	{1650, 0, 1280, 0, 110, 40, 750, 0, 720, 0, 5, 5, 74250, 4, 0,
		TIMING_STANDARD_CEA861, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
			{0, 0, 0, 1, 1, 0, 0} } },
{
	{1280, 768, 60, TIMING_STANDARD_DMT, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 1} },
	{1440, 0, 1280, 0, 48, 32, 790, 0, 768, 0, 3, 7, 68250, 0, 0,
		TIMING_STANDARD_DMT, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
			{0, 0, 0, 1, 0, 0, 0} } },
{
	{1280, 768, 60, TIMING_STANDARD_DMT, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0} },
	{1664, 0, 1280, 0, 64, 128, 798, 0, 768, 0, 3, 7, 79500, 0, 0,
		TIMING_STANDARD_DMT, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
			{0, 0, 0, 0, 1, 0, 0} } },
{
	{1280, 768, 75, TIMING_STANDARD_DMT, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0} },
	{1696, 0, 1280, 0, 80, 128, 805, 0, 768, 0, 3, 7, 102250, 0, 0,
		TIMING_STANDARD_DMT, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
			{0, 0, 0, 0, 1, 0, 0} } },
{
	{1280, 768, 85, TIMING_STANDARD_DMT, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0} },
	{1712, 0, 1280, 0, 80, 136, 809, 0, 768, 0, 3, 7, 117500, 0, 0,
		TIMING_STANDARD_DMT, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
			{0, 0, 0, 0, 1, 0, 0} } },
{
	{1280, 768, 120, TIMING_STANDARD_DMT, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 1} },
	{1440, 0, 1280, 0, 48, 32, 813, 0, 768, 0, 3, 7, 140250, 0, 0,
		TIMING_STANDARD_DMT, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
			{0, 0, 0, 1, 0, 0, 0} } },
{
	{1280, 800, 60, TIMING_STANDARD_DMT, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 1} },
	{1440, 0, 1280, 0, 48, 32, 823, 0, 800, 0, 3, 6, 71000, 0, 0,
		TIMING_STANDARD_DMT, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
			{0, 0, 0, 1, 0, 0, 0} } },
{
	{1280, 800, 60, TIMING_STANDARD_DMT, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0} },
	{1680, 0, 1280, 0, 72, 128, 831, 0, 800, 0, 3, 6, 83500, 0, 0,
		TIMING_STANDARD_DMT, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
			{0, 0, 0, 0, 1, 0, 0} } },
{
	{1280, 800, 75, TIMING_STANDARD_DMT, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0} },
	{1696, 0, 1280, 0, 80, 128, 838, 0, 800, 0, 3, 6, 106500, 0, 0,
		TIMING_STANDARD_DMT, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
			{0, 0, 0, 0, 1, 0, 0} } },
{
	{1280, 800, 85, TIMING_STANDARD_DMT, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0} },
	{1712, 0, 1280, 0, 80, 136, 843, 0, 800, 0, 3, 6, 122500, 0, 0,
		TIMING_STANDARD_DMT, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
			{0, 0, 0, 0, 1, 0, 0} } },
{
	{1280, 800, 120, TIMING_STANDARD_DMT, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 1} },
	{1440, 0, 1280, 0, 48, 32, 847, 0, 800, 0, 3, 6, 146250, 0, 0,
		TIMING_STANDARD_DMT, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
			{0, 0, 0, 1, 0, 0, 0} } },
{
	{1280, 960, 60, TIMING_STANDARD_DMT, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0} },
	{1800, 0, 1280, 0, 96, 112, 1000, 0, 960, 0, 1, 3, 108000, 0, 0,
		TIMING_STANDARD_DMT, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
			{0, 0, 0, 1, 1, 0, 0} } },
{
	{1280, 960, 85, TIMING_STANDARD_DMT, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0} },
	{1728, 0, 1280, 0, 64, 160, 1011, 0, 960, 0, 1, 3, 148500, 0, 0,
		TIMING_STANDARD_DMT, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
			{0, 0, 0, 1, 1, 0, 0} } },
{
	{1280, 960, 120, TIMING_STANDARD_DMT, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 1} },
	{1440, 0, 1280, 0, 48, 32, 1017, 0, 960, 0, 3, 4, 175500, 0, 0,
		TIMING_STANDARD_DMT, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
			{0, 0, 0, 1, 0, 0, 0} } },
{
	{1280, 1024, 60, TIMING_STANDARD_DMT, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0} },
	{1688, 0, 1280, 0, 48, 112, 1066, 0, 1024, 0, 1, 3, 108000, 0, 0,
		TIMING_STANDARD_DMT, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
			{0, 0, 0, 1, 1, 0, 0} } },
{
	{1280, 1024, 75, TIMING_STANDARD_DMT, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0} },
	{1688, 0, 1280, 0, 16, 144, 1066, 0, 1024, 0, 1, 3, 135000, 0, 0,
		TIMING_STANDARD_DMT, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
			{0, 0, 0, 1, 1, 0, 0} } },
{
	{1280, 1024, 85, TIMING_STANDARD_DMT, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0} },
	{1728, 0, 1280, 0, 64, 160, 1072, 0, 1024, 0, 1, 3, 157500, 0, 0,
		TIMING_STANDARD_DMT, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
			{0, 0, 0, 1, 1, 0, 0} } },
{
	{1280, 1024, 120, TIMING_STANDARD_DMT, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 1} },
	{1440, 0, 1280, 0, 48, 32, 1084, 0, 1024, 0, 3, 7, 187250, 0, 0,
		TIMING_STANDARD_DMT, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
			{0, 0, 0, 1, 0, 0, 0} } },
{
	{1360, 768, 60, TIMING_STANDARD_DMT, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0} },
	{1792, 0, 1360, 0, 64, 112, 795, 0, 768, 0, 3, 6, 85500, 0, 0,
		TIMING_STANDARD_DMT, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
			{0, 0, 0, 1, 1, 0, 0} } },
{
	{1360, 768, 120, TIMING_STANDARD_DMT, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 1} },
	{1520, 0, 1360, 0, 48, 32, 813, 0, 768, 0, 3, 5, 148250, 0, 0,
		TIMING_STANDARD_DMT, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
			{0, 0, 0, 1, 0, 0, 0} } },
{
	{1366, 768, 60, TIMING_STANDARD_DMT, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0} },
	{1792, 0, 1366, 0, 70, 143, 798, 0, 768, 0, 3, 3, 85500, 0, 0,
		TIMING_STANDARD_DMT, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
			{0, 0, 0, 1, 1, 0, 0} } },
{
	{1366, 768, 60, TIMING_STANDARD_DMT, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0} },
	{1500, 0, 1366, 0, 14, 56, 800, 0, 768, 0, 1, 3, 72000, 0, 0,
		TIMING_STANDARD_DMT, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
			{0, 0, 0, 1, 1, 0, 0} } },
{
	{1400, 1050, 60, TIMING_STANDARD_DMT, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 1} },
	{1560, 0, 1400, 0, 48, 32, 1080, 0, 1050, 0, 3, 4, 101000, 0, 0,
		TIMING_STANDARD_DMT, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
			{0, 0, 0, 1, 0, 0, 0} } },
{
	{1400, 1050, 60, TIMING_STANDARD_DMT, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0} },
	{1864, 0, 1400, 0, 88, 144, 1089, 0, 1050, 0, 3, 4, 121750, 0, 0,
		TIMING_STANDARD_DMT, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
			{0, 0, 0, 0, 1, 0, 0} } },
{
	{1400, 1050, 75, TIMING_STANDARD_DMT, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0} },
	{1896, 0, 1400, 0, 104, 144, 1099, 0, 1050, 0, 3, 4, 156000, 0, 0,
		TIMING_STANDARD_DMT, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
			{0, 0, 0, 0, 1, 0, 0} } },
{
	{1400, 1050, 85, TIMING_STANDARD_DMT, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0} },
	{1912, 0, 1400, 0, 104, 152, 1105, 0, 1050, 0, 3, 4, 179500, 0, 0,
		TIMING_STANDARD_DMT, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
			{0, 0, 0, 0, 1, 0, 0} } },
{
	{1400, 1050, 120, TIMING_STANDARD_DMT, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 1} },
	{1560, 0, 1400, 0, 48, 32, 1112, 0, 1050, 0, 3, 4, 208000, 0, 0,
		TIMING_STANDARD_DMT, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
			{0, 0, 0, 1, 0, 0, 0} } },
{
	{1440, 900, 60, TIMING_STANDARD_DMT, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 1} },
	{1600, 0, 1440, 0, 48, 32, 926, 0, 900, 0, 3, 6, 88750, 0, 0,
		TIMING_STANDARD_DMT, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
			{0, 0, 0, 1, 0, 0, 0} } },
{
	{1440, 900, 60, TIMING_STANDARD_DMT, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0} },
	{1904, 0, 1440, 0, 80, 152, 934, 0, 900, 0, 3, 6, 106500, 0, 0,
		TIMING_STANDARD_DMT, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
			{0, 0, 0, 0, 1, 0, 0} } },
{
	{1440, 900, 75, TIMING_STANDARD_DMT, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0} },
	{1936, 0, 1440, 0, 96, 152, 942, 0, 900, 0, 3, 6, 136750, 0, 0,
		TIMING_STANDARD_DMT, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
			{0, 0, 0, 0, 1, 0, 0} } },
{
	{1440, 900, 85, TIMING_STANDARD_DMT, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0} },
	{1952, 0, 1440, 0, 104, 152, 948, 0, 900, 0, 3, 6, 157000, 0, 0,
		TIMING_STANDARD_DMT, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
			{0, 0, 0, 0, 1, 0, 0} } },
{
	{1440, 900, 120, TIMING_STANDARD_DMT, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 1} },
	{1600, 0, 1440, 0, 48, 32, 953, 0, 900, 0, 3, 6, 182750, 0, 0,
		TIMING_STANDARD_DMT, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
			{0, 0, 0, 1, 0, 0, 0} } },
{
	{1600, 900, 60, TIMING_STANDARD_DMT, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0} },
	{1800, 0, 1600, 0, 24, 80, 1000, 0, 900, 0, 1, 3, 108000, 0, 0,
		TIMING_STANDARD_DMT, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
			{0, 0, 0, 1, 1, 0, 0} } },
{
	{1600, 1200, 60, TIMING_STANDARD_DMT, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0} },
	{2160, 0, 1600, 0, 64, 192, 1250, 0, 1200, 0, 1, 3, 162000, 0, 0,
		TIMING_STANDARD_DMT, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
			{0, 0, 0, 1, 1, 0, 0} } },
{
	{1600, 1200, 65, TIMING_STANDARD_DMT, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0} },
	{2160, 0, 1600, 0, 64, 192, 1250, 0, 1200, 0, 1, 3, 175500, 0, 0,
		TIMING_STANDARD_DMT, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
			{0, 0, 0, 1, 1, 0, 0} } },
{
	{1600, 1200, 70, TIMING_STANDARD_DMT, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0} },
	{2160, 0, 1600, 0, 64, 192, 1250, 0, 1200, 0, 1, 3, 189000, 0, 0,
		TIMING_STANDARD_DMT, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
			{0, 0, 0, 1, 1, 0, 0} } },
{
	{1600, 1200, 75, TIMING_STANDARD_DMT, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0} },
	{2160, 0, 1600, 0, 64, 192, 1250, 0, 1200, 0, 1, 3, 202500, 0, 0,
		TIMING_STANDARD_DMT, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
			{0, 0, 0, 1, 1, 0, 0} } },
{
	{1600, 1200, 85, TIMING_STANDARD_DMT, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0} },
	{2160, 0, 1600, 0, 64, 192, 1250, 0, 1200, 0, 1, 3, 229500, 0, 0,
		TIMING_STANDARD_DMT, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
			{0, 0, 0, 1, 1, 0, 0} } },
{
	{1600, 1200, 120, TIMING_STANDARD_DMT, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 1} },
	{1760, 0, 1600, 0, 48, 32, 1271, 0, 1200, 0, 3, 4, 268250, 0, 0,
		TIMING_STANDARD_DMT, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
			{0, 0, 0, 1, 0, 0, 0} } },
{
	{1680, 1050, 60, TIMING_STANDARD_DMT, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 1} },
	{1840, 0, 1680, 0, 48, 32, 1080, 0, 1050, 0, 3, 6, 119000, 0, 0,
		TIMING_STANDARD_DMT, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
			{0, 0, 0, 1, 0, 0, 0} } },
{
	{1680, 1050, 60, TIMING_STANDARD_DMT, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0} },
	{2240, 0, 1680, 0, 104, 176, 1089, 0, 1050, 0, 3, 6, 146250, 0, 0,
		TIMING_STANDARD_DMT, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
			{0, 0, 0, 0, 1, 0, 0} } },
{
	{1680, 1050, 75, TIMING_STANDARD_DMT, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0} },
	{2272, 0, 1680, 0, 120, 176, 1099, 0, 1050, 0, 3, 6, 187000, 0, 0,
		TIMING_STANDARD_DMT, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
			{0, 0, 0, 0, 1, 0, 0} } },
{
	{1680, 1050, 85, TIMING_STANDARD_DMT, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0} },
	{2288, 0, 1680, 0, 128, 176, 1105, 0, 1050, 0, 3, 6, 214750, 0, 0,
		TIMING_STANDARD_DMT, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
			{0, 0, 0, 0, 1, 0, 0} } },
{
	{1680, 1050,
		120, TIMING_STANDARD_DMT, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 1} },
	{1840, 0, 1680, 0, 48, 32, 1112, 0, 1050, 0,
		3, 6, 245500, 0, 0, TIMING_STANDARD_DMT, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
			{0, 0, 0,
		1, 0, 0, 0} } },
{
	{1792, 1344, 60, TIMING_STANDARD_DMT, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0} },
	{2448, 0, 1792, 0, 128, 200, 1394, 0, 1344, 0, 1, 3, 204750, 0, 0,
		TIMING_STANDARD_DMT, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
			{0, 0, 0, 0, 1, 0, 0} } },
{
	{1792, 1344, 75, TIMING_STANDARD_DMT, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0} },
	{2456, 0, 1792, 0, 96, 216, 1417, 0, 1344, 0, 1, 3, 261000, 0, 0,
		TIMING_STANDARD_DMT, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
			{0, 0, 0, 0, 1, 0, 0} } },
{
	{1792, 1344, 120, TIMING_STANDARD_DMT, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 1} },
	{1952, 0, 1792, 0, 48, 32, 1423, 0, 1344, 0, 3, 4, 333250, 0, 0,
		TIMING_STANDARD_DMT, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
			{0, 0, 0, 1, 0, 0, 0} } },
{
	{1856, 1392, 60, TIMING_STANDARD_DMT, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0} },
	{2528, 0, 1856, 0, 96, 224, 1439, 0, 1392, 0, 1, 3, 218250, 0, 0,
		TIMING_STANDARD_DMT, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
			{0, 0, 0, 0, 1, 0, 0} } },
{
	{1856, 1392, 75, TIMING_STANDARD_DMT, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0} },
	{2560, 0, 1856, 0, 128, 224, 1500, 0, 1392, 0, 1, 3, 288000, 0, 0,
		TIMING_STANDARD_DMT, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
			{0, 0, 0, 0, 1, 0, 0} } },
{
	{1856, 1392,
		120, TIMING_STANDARD_DMT, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0} },
	{2016, 0, 1856, 0, 48, 32, 1474, 0, 1392, 0, 3, 4, 356500, 0, 0,
		TIMING_STANDARD_DMT, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
			{0, 0, 0, 1, 0, 0, 0} } },
{
	{1920, 1080, 60, TIMING_STANDARD_CEA861, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0} },
	{2200, 0, 1920, 0, 88, 44, 1125, 0, 1080, 0, 4, 5, 148500, 16, 0,
		TIMING_STANDARD_CEA861, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
			{0, 0, 0, 1, 1, 0, 0} } },
{
	{1920, 1200, 60, TIMING_STANDARD_DMT, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 1} },
	{2080, 0, 1920, 0, 48, 32, 1235, 0, 1200, 0, 3, 6, 154000, 0, 0,
		TIMING_STANDARD_DMT, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
			{0, 0, 0, 1, 0, 0, 0} } },
{
	{1920, 1200, 60, TIMING_STANDARD_DMT, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0} },
	{2592, 0, 1920, 0, 136, 200, 1245, 0, 1200, 0, 3, 6, 193250, 0, 0,
		TIMING_STANDARD_DMT, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
			{0, 0, 0, 0, 1, 0, 0} } },
{
	{1920, 1200, 75, TIMING_STANDARD_DMT, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0} },
	{2608, 0, 1920, 0, 136, 208, 1255, 0, 1200, 0, 3, 6, 245250, 0, 0,
		TIMING_STANDARD_DMT, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
			{0, 0, 0, 0, 1, 0, 0} } },
{
	{1920, 1200, 85, TIMING_STANDARD_DMT, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0} },
	{2624, 0, 1920, 0, 144, 208, 1262, 0, 1200, 0, 3, 6, 281250, 0, 0,
		TIMING_STANDARD_DMT, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
			{0, 0, 0, 0, 1, 0, 0} } },
{
	{1920, 1200,
		120, TIMING_STANDARD_DMT, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 1} },
	{2080, 0, 1920, 0, 48, 32, 1271, 0, 1200, 0, 3, 6, 317000, 0, 0,
		TIMING_STANDARD_DMT, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
			{0, 0, 0, 1, 0, 0, 0} } },
{
	{1920, 1440, 60, TIMING_STANDARD_DMT, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0} },
	{2600, 0, 1920, 0, 128, 208, 1500, 0, 1440, 0, 1, 3, 234000, 0, 0,
		TIMING_STANDARD_DMT, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
			{0, 0, 0, 0, 1, 0, 0} } },
{
	{1920, 1440, 75, TIMING_STANDARD_DMT, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0} },
	{2640, 0, 1920, 0, 144, 224, 1500, 0, 1440, 0, 1, 3, 297000, 0, 0,
		TIMING_STANDARD_DMT, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
			{0, 0, 0, 0, 1, 0, 0} } },
{
	{1920, 1440, 120, TIMING_STANDARD_DMT, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 1} },
	{2080, 0, 1920, 0, 48, 32, 1525, 0, 1440, 0, 3, 4, 380500, 0, 0,
		TIMING_STANDARD_DMT, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
		{0, 0, 0, 1, 0, 0, 0} } },
{
	{2048, 1152, 60, TIMING_STANDARD_DMT, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0} },
	{2250, 0, 2048, 0, 26, 80, 1200, 0, 1152, 0, 1, 3, 162000, 0, 0,
			TIMING_STANDARD_DMT, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
			{0, 0, 0, 1, 1, 0, 0} } },
{
	{2560, 1600, 60, TIMING_STANDARD_DMT, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 1} },
	{2720, 0, 2560, 0, 48, 32, 1646, 0, 1600, 0, 3, 6, 268500, 0, 0,
		TIMING_STANDARD_DMT, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
			{0, 0, 0, 1, 0, 0, 0} } },
{
	{2560, 1600, 60, TIMING_STANDARD_DMT, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0} },
	{3504, 0, 2560, 0, 192, 280, 1658, 0, 1600, 0, 3, 6, 348500, 0, 0,
		TIMING_STANDARD_DMT, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
			{0, 0, 0, 0, 1, 0, 0} } },
{
	{2560, 1600, 75, TIMING_STANDARD_DMT, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0} },
	{3536, 0, 2560, 0, 208, 280, 1672, 0, 1600, 0, 3, 6, 443250, 0, 0,
		TIMING_STANDARD_DMT, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
			{0, 0, 0, 0, 1, 0, 0} } },
{
	{2560, 1600, 85, TIMING_STANDARD_DMT, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 0} },
	{3536, 0, 2560, 0, 208, 280, 1682, 0, 1600, 0, 3, 6, 505250, 0, 0,
		TIMING_STANDARD_DMT, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
			{0, 0, 0, 0, 1, 0, 0} } },
{
	{2560, 1600, 120, TIMING_STANDARD_DMT, TIMING_SOURCE_UNDEFINED,
		{0, 0, 0, 1} },
	{2720, 0, 2560, 0, 48, 32, 1694, 0, 1600, 0, 3, 6, 552750, 0, 0,
		TIMING_STANDARD_DMT, TIMING_3D_FORMAT_NONE,
		DISPLAY_COLOR_DEPTH_UNDEFINED, PIXEL_ENCODING_UNDEFINED,
		{0, 0, 0, 1, 0, 0, 0} } }
};

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
 * @return pointer to timing if mode timing was found successfully,
 *	NULL otherwise
 */
static const struct mode_timing *get_mode_timing_by_index(uint32_t index)
{
	if (index < MODE_COUNT)
		return &(mode_timings[index]);

	return NULL;
}

/**
 * This is the comparison function used
 * when trying to match a ModeInfo to find the timing.
 * The difference between this compare
 * and the ModeInfo::operator== is that this compares
 * the rounded refreshRate when trying to find a match.
 *
 * @param lhs left-hand side of the equation
 * @param rhs right-hand side of the equation
 * @return return true if left-hand side
 *	is equal to right-hand side, false otherwise
 */
static bool is_mode_info_equal(
		const struct mode_info *lhs,
		const struct mode_info *rhs)
{
	if (lhs->pixel_height != rhs->pixel_height)
		return false;
	if (lhs->pixel_width != rhs->pixel_width)
		return false;
	if (lhs->flags.INTERLACE != rhs->flags.INTERLACE)
		return false;
	if (lhs->field_rate != rhs->field_rate)
		return false;
	if (lhs->flags.REDUCED_BLANKING != rhs->flags.REDUCED_BLANKING)
		return false;

	return true;
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
		if (is_mode_info_equal(mode, &(mode_timings[i].mode_info))) {
			*crtc_timing = mode_timings[i].crtc_timing;
			return true;
		}

	return false;
}

/**
 * Query used to determine if a timing is compatible with this source
 *
 * @param timing the timing to verify if it is compatible with this timing standard
 * @return true if the timing matches some timing within this standard, false if there is not match.
 */
static bool is_timing_in_standard(const struct crtc_timing *timing)
{
	uint32_t i;

	for (i = 0; i < MODE_COUNT; ++i)
		if (dal_crtc_timing_is_equal(
				timing,
				&(mode_timings[i].crtc_timing)))
			return true;
	return false;
}

struct mode_timing_source_funcs *dal_mode_timing_source_dmt_create(void)
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

	return mts;
out_free:
	dal_free(mts);
	return NULL;
}
