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

#ifndef __DAL_HW_SEQUENCER_TYPES_H__
#define __DAL_HW_SEQUENCER_TYPES_H__

#include "signal_types.h"
#include "grph_object_defs.h"
#include "link_service_types.h"
#include "plane_types.h"

enum {
	HW_MAX_NUM_VIEWPORTS = 2,
	HW_CURRENT_PIPE_INDEX = 0,
	HW_OTHER_PIPE_INDEX = 1
};

struct hw_view_port_adjustment {
	int32_t start_adjustment;
	int32_t width;

	enum controller_id controller_id;
};

struct hw_view_port_adjustments {
	uint32_t view_ports_num;
	struct hw_view_port_adjustment adjustments[HW_MAX_NUM_VIEWPORTS];
};

/* Color depth of a display */
enum hw_color_depth {
	HW_COLOR_DEPTH_UNKNOWN,
	HW_COLOR_DEPTH_666,
	HW_COLOR_DEPTH_888,
	HW_COLOR_DEPTH_101010,
	HW_COLOR_DEPTH_121212,
	HW_COLOR_DEPTH_141414,
	HW_COLOR_DEPTH_161616
};

/* Pixel encoding format of a display */
enum hw_pixel_encoding {
	HW_PIXEL_ENCODING_UNKNOWN,
	HW_PIXEL_ENCODING_RGB,
	HW_PIXEL_ENCODING_YCBCR422,
	HW_PIXEL_ENCODING_YCBCR444
};

/* Timing standard */
enum hw_timing_standard {
	HW_TIMING_STANDARD_UNDEFINED,
	HW_TIMING_STANDARD_DMT,
	HW_TIMING_STANDARD_GTF,
	HW_TIMING_STANDARD_CVT,
	HW_TIMING_STANDARD_CVT_RB,
	HW_TIMING_STANDARD_CEA770,
	HW_TIMING_STANDARD_CEA861,
	HW_TIMING_STANDARD_HDMI,
	HW_TIMING_STANDARD_TV_NTSC,
	HW_TIMING_STANDARD_TV_NTSC_J,
	HW_TIMING_STANDARD_TV_PAL,
	HW_TIMING_STANDARD_TV_PAL_M,
	HW_TIMING_STANDARD_TV_PAL_CN,
	HW_TIMING_STANDARD_TV_SECAM,
	/* for explicit timings from VBIOS, EDID etc. */
	HW_TIMING_STANDARD_EXPLICIT
};

/* identical to struct crtc_ranged_timing_control
 * defined in controller\timing_generator_types.h */
struct hw_ranged_timing_control {
	/* set to 1 to force dynamic counter V_COUNT
	 * to lock to constant rate counter V_COUNT_NOM
	 * on page flip event in dynamic refresh mode
	 * when switching from a low refresh rate to nominal refresh rate */
	bool force_lock_on_event;
	/* set to 1 to force CRTC2 (slave) to lock to CRTC1 (master) VSync
	 * in order to overlap their blank regions for MC clock changes */
	bool lock_to_master_vsync;

	/* set to 1 to program Static Screen Detection Masks
	 * without enabling dynamic refresh rate */
	bool program_static_screen_mask;
	/* set to 1 to program Dynamic Refresh Rate */
	bool program_dynamic_refresh_rate;
	/* set to 1 to force disable Dynamic Refresh Rate */
	bool force_disable_drr;

	/* event mask to enable/disable various trigger sources
	 * for static screen detection */
	struct static_screen_events event_mask;

	/* Number of consecutive static screen frames before static state is
	 * asserted. */
	uint32_t static_frame_count;
};

/* define the structure of Dynamic Refresh Mode */
struct hw_ranged_timing {
	/* defines the minimum possible vertical dimension of display timing
	 * for CRTC as supported by the panel */
	uint32_t vertical_total_min;
	/* defines the maximum possible vertical dimension of display timing
	 * for CRTC as supported by the panel */
	uint32_t vertical_total_max;

	struct hw_ranged_timing_control control;
};

/* Color depth */
enum crtc_color_depth {
	CRTC_COLOR_DEPTH_UNKNOWN = 0,
	CRTC_COLOR_DEPTH_666,
	CRTC_COLOR_DEPTH_888,
	CRTC_COLOR_DEPTH_101010,
	CRTC_COLOR_DEPTH_121212,
	CRTC_COLOR_DEPTH_141414,
	CRTC_COLOR_DEPTH_161616
};

/* CRTC timing structure */
struct hw_crtc_timing {
	uint32_t h_total;
	uint32_t h_addressable;
	uint32_t h_overscan_left;
	uint32_t h_overscan_right;
	uint32_t h_sync_start;
	uint32_t h_sync_width;

	uint32_t v_total;
	uint32_t v_addressable;
	uint32_t v_overscan_top;
	uint32_t v_overscan_bottom;
	uint32_t v_sync_start;
	uint32_t v_sync_width;

	struct hw_ranged_timing ranged_timing;

	/* in KHz */
	uint32_t pixel_clock;

	enum hw_timing_standard timing_standard;

	struct {
		uint32_t INTERLACED:1;
		uint32_t DOUBLESCAN:1;
		uint32_t PIXEL_REPETITION:4; /* 1...10 */
		uint32_t HSYNC_POSITIVE_POLARITY:1;
		uint32_t VSYNC_POSITIVE_POLARITY:1;
		/* frame should be packed for 3D
		 * (currently this refers to HDMI 1.4a FramePacking format */
		uint32_t HORZ_COUNT_BY_TWO:1;
		uint32_t PACK_3D_FRAME:1;
		/* 0 - left eye polarity, 1 - right eye polarity */
		uint32_t RIGHT_EYE_3D_POLARITY:1;
		/* DVI-DL High-Color mode */
		uint32_t HIGH_COLOR_DL_MODE:1;
		enum hw_color_depth COLOR_DEPTH:4;
		enum hw_pixel_encoding PIXEL_ENCODING:4;
		uint32_t Y_ONLY:1;
		/* HDMI 2.0 - Support scrambling for TMDS character
		 * rates less than or equal to 340Mcsc */
		uint32_t LTE_340MCSC_SCRAMBLE:1;
	} flags;

	enum crtc_color_depth crtc_color_depth;
};

struct hw_scaling_info {
	struct view src;
	struct view dst;
	enum signal_type signal;
};

enum hw_color_space {
	HW_COLOR_SPACE_UNKNOWN = 0,
	HW_COLOR_SPACE_SRGB_FULL_RANGE,
	HW_COLOR_SPACE_SRGB_LIMITED_RANGE,
	HW_COLOR_SPACE_YPBPR601,
	HW_COLOR_SPACE_YPBPR709,
	HW_COLOR_SPACE_YCBCR601,
	HW_COLOR_SPACE_YCBCR709,
	HW_COLOR_SPACE_YCBCR601_YONLY,
	HW_COLOR_SPACE_YCBCR709_YONLY,
	HW_COLOR_SPACE_NMVPU_SUPERAA,
};

enum hw_overlay_color_space {
	HW_OVERLAY_COLOR_SPACE_UNKNOWN,
	HW_OVERLAY_COLOR_SPACE_BT709,
	HW_OVERLAY_COLOR_SPACE_BT601,
	HW_OVERLAY_COLOR_SPACE_SMPTE240,
	HW_OVERLAY_COLOR_SPACE_RGB
};

enum hw_overlay_backend_bpp {
	HW_OVERLAY_BACKEND_BPP_UNKNOWN,
	HW_OVERLAY_BACKEND_BPP32_FULL_BANDWIDTH,
	HW_OVERLAY_BACKEND_BPP16_FULL_BANDWIDTH,
	HW_OVERLAY_BACKEND_BPP32_HALF_BANDWIDTH,
};
enum hw_overlay_format {
	HW_OVERLAY_FORMAT_UNKNOWN,
	HW_OVERLAY_FORMAT_YUY2,
	HW_OVERLAY_FORMAT_UYVY,
	HW_OVERLAY_FORMAT_RGB565,
	HW_OVERLAY_FORMAT_RGB555,
	HW_OVERLAY_FORMAT_RGB32,
	HW_OVERLAY_FORMAT_YUV444,
	HW_OVERLAY_FORMAT_RGB32_2101010
};

enum hw_tiling_mode {
	HW_TILING_MODE_INVALID,
	HW_TILING_MODE_LINEAR,
	HW_TILING_MODE_TILED

};

enum hw_scale_options {
	HW_SCALE_OPTION_UNKNOWN,
	HW_SCALE_OPTION_OVERSCAN, /* multimedia pass through mode */
	HW_SCALE_OPTION_UNDERSCAN
};

enum hw_stereo_format {
	HW_STEREO_FORMAT_NONE = 0,
	HW_STEREO_FORMAT_SIDE_BY_SIDE = 1,
	HW_STEREO_FORMAT_TOP_AND_BOTTOM = 2,
	HW_STEREO_FORMAT_FRAME_ALTERNATE = 3,
	HW_STEREO_FORMAT_ROW_INTERLEAVED = 5,
	HW_STEREO_FORMAT_COLUMN_INTERLEAVED = 6,
	HW_STEREO_FORMAT_CHECKER_BOARD = 7 /* the same as pixel interleave */
};

#define HW_STEREO_FORMAT_TO_MIXER_MODE(hw_format) \
	(hw_format == HW_STEREO_FORMAT_ROW_INTERLEAVED ? \
			HW_STEREO_MIXER_MODE_ROW_INTERLEAVE : \
	hw_format == HW_STEREO_FORMAT_COLUMN_INTERLEAVED ? \
			HW_STEREO_MIXER_MODE_COLUMN_INTERLEAVE : \
	hw_format == HW_STEREO_FORMAT_CHECKER_BOARD ? \
			HW_STEREO_MIXER_MODE_PIXEL_INTERLEAVE : \
	HW_STEREO_MIXER_MODE_INACTIVE);


enum hw_dithering_options {
	HW_DITHERING_OPTION_UNKNOWN,
	HW_DITHERING_OPTION_SKIP_PROGRAMMING,
	HW_DITHERING_OPTION_ENABLE,
	HW_DITHERING_OPTION_DISABLE
};

enum hw_rotation_angle {
	HW_ROTATION_ANGLE_0,
	HW_ROTATION_ANGLE_90,
	HW_ROTATION_ANGLE_180,
	HW_ROTATION_ANGLE_270,
	HW_ROTATION_ANGLE_INVALID
};

struct hw_stereo_mixer_params {
	bool sub_sampling;
	bool single_pipe;
};

struct hw_mode_info {
	struct view view;
	enum pixel_format pixel_format;
	uint32_t refresh_rate;
	struct hw_scaling_info scaling_info;
	struct hw_crtc_timing timing;
	enum hw_overlay_backend_bpp backend_bpp;
	enum hw_overlay_color_space ovl_color_space;
	enum hw_overlay_format ovl_surface;
	enum hw_color_space color_space;
	struct overscan_info overscan;
	enum hw_scale_options underscan_rule;
	bool fbc_enabled;
	bool lpt_enabled;
	enum hw_tiling_mode tiling_mode;
	struct hw_stereo_mixer_params stereo_mixer_params;
	enum hw_dithering_options dithering;

	/* output parameter */
	struct hw_view_port_adjustments *view_port_adjustments;

	struct /* HW Sequencer should not used this struct ds_info */
	{
		struct hw_crtc_timing original_timing;
		uint32_t position_x;
		uint32_t position_y;
		uint32_t DISPLAY_PREFERED_MODE:1;
		uint32_t TIMING_REDUCE_BLANK_PATCHED:1;
		uint32_t TIMING_UNDERSCAN_PATCHED:1;
		uint32_t TIMING_VH_SIZE_PATCHED:1;
		uint32_t TIMING_SCALING_PATCHED:1;
		uint32_t ALLOC_OVL_SIGNAL:1;
		uint32_t FREE_OVL_SIGNAL:1;
		uint32_t cea_vic;
	} ds_info;

	enum hw_rotation_angle rotation;
	bool is_tiling_rotated;
	/* 'stereo_format' is used in:
	 *	1. stereo mixer parameters
	 *	2. bandwidth manager parameters */
	enum hw_stereo_format stereo_format;

	struct scaling_tap_info taps_requested;
};

enum hw_path_action {
	HW_PATH_ACTION_UNDEFINED,
	HW_PATH_ACTION_SET, /* set given mode on the path */
	HW_PATH_ACTION_RESET, /* reset the path */
	/* path already set, for resource consideration only. no programming */
	HW_PATH_ACTION_EXISTING,
	/* set given adjustment on the path */
	HW_PATH_ACTION_SET_ADJUSTMENT,
	/* perform static validation */
	HW_PATH_ACTION_STATIC_VALIDATE
};

struct hw_action_flags {
	uint32_t RESYNC_PATH:1;
	uint32_t TIMING_CHANGED:1;
	uint32_t PIXEL_ENCODING_CHANGED:1;
	uint32_t GAMUT_CHANGED:1;
	uint32_t TURN_OFF_VCC:1;
};

enum hw_sync_request {
	HW_SYNC_REQUEST_NONE = 0,
	HW_SYNC_REQUEST_SET_INTERPATH,
	HW_SYNC_REQUEST_SET_GL_SYNC_GENLOCK,
	HW_SYNC_REQUEST_SET_GL_SYNC_FREE_RUN,
	HW_SYNC_REQUEST_SET_GL_SYNC_SHADOW,
	HW_SYNC_REQUEST_RESET_GLSYNC,
	HW_SYNC_REQUEST_RESYNC_GLSYNC,
	HW_SYNC_REQUEST_SET_STEREO3D
};

struct hw_sync_info {
	enum hw_sync_request sync_request;
	uint32_t target_pixel_clock; /* in KHz */
	enum sync_source sync_source;
};

/* TODO hw_info_frame and hw_info_packet structures are same as in encoder
 * merge it*/
struct hw_info_packet {
	bool valid;
	uint8_t hb0;
	uint8_t hb1;
	uint8_t hb2;
	uint8_t hb3;
	uint8_t sb[28];
};

struct hw_info_frame {
	/* Auxiliary Video Information */
	struct hw_info_packet avi_info_packet;
	struct hw_info_packet gamut_packet;
	struct hw_info_packet vendor_info_packet;
	/* Source Product Description */
	struct hw_info_packet spd_packet;
	/* Video Stream Configuration */
	struct hw_info_packet vsc_packet;
};

struct hw_path_mode {
	enum hw_path_action action;
	struct hw_action_flags action_flags;
	struct hw_mode_info mode;
	struct hw_sync_info sync_info;
	struct display_path *display_path;
	struct hw_adjustment_set *adjustment_set;
	struct hw_info_frame info_frame;
	/* existing path only, for clock state dependancy */
	struct link_settings link_settings;

	struct vector *plane_configs;
};

enum channel_command_type {
	CHANNEL_COMMAND_I2C,
	CHANNEL_COMMAND_I2C_OVER_AUX,
	CHANNEL_COMMAND_AUX
};


/* maximum TMDS transmitter pixel clock is 165 MHz. So it is KHz */
#define	TMDS_MAX_PIXEL_CLOCK_IN_KHZ 165000
#define	NATIVE_HDMI_MAX_PIXEL_CLOCK_IN_KHZ 297000

struct hw_adjustment_range {
	int32_t hw_default;
	int32_t min;
	int32_t max;
	int32_t step;
	uint32_t divider; /* (actually HW range is min/divider; divider !=0) */
};

#endif
