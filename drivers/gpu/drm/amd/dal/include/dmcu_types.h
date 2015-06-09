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

#ifndef __DAL_DMCU_TYPES_H__
#define __DAL_DMCU_TYPES_H__

/* Forward declaration */
struct dmcu;

/* Required information for creation and initialization of a controller */
struct dmcu_init_data {
	struct dal_context *dal_context;
	struct adapter_service *as;
	uint32_t max_engine_clock_in_khz;
};

/* Interface structure defines */

enum dmcu_action {
	DMCU_ACTION_PSR_ENABLE,
	DMCU_ACTION_PSR_EXIT,
	DMCU_ACTION_PSR_RFB_UPDATE,
	DMCU_ACTION_PSR_SET,
	DMCU_ACTION_PSR_CLEAR_COUNT,
	DMCU_ACTION_PSR_COUNT_REQUEST,
	DMCU_ACTION_PSR_STATE_REQUEST,
	DMCU_ACTION_PSR_SET_LEVEL,
	DMCU_ACTION_PSR_ADVANCE_STATE,
	DMCU_ACTION_PSR_SET_WAITLOOP
};

enum dmcu_output {
	DMCU_OUTPUT_PSR_ACK,
	DMCU_OUTPUT_PSR_NACK,
	DMCU_OUTPUT_PSR_AUX_ERR,
	DMCU_OUTPUT_PSR_COUNT_STATUS,
	DMCU_OUTPUT_PSR_STATE_STATUS,
	DMCU_OUTPUT_PSR_RFB_UPDATE_ERR,
	DMCU_OUTPUT_PSR_ERR,
	DMCU_OUTPUT_PSR_GET_REPLY,
	DMCU_OUTPUT_PSR_ENTRY_ERROR,
	DMCU_OUTPUT_PSR_LT_ERROR,
	DMCU_OUTPUT_PSR_FORCE_SR_ERROR,
	DMCU_OUTPUT_PSR_SDP_SEND_TIMEOUT
};

/* PSR states, based similarly on states defined in eDP specification. */
enum psr_state {
	STATE0,		/* PSR is disabled */
	STATE1,		/* PSR is enabled, but inactive */
	STATE1A,
	STATE2,		/* PSR is transitioning to active state */
	STATE2A,
	STATE3,		/* PSR is active; Display is in self refresh */
	STATE3INIT,
	STATE4,		/* RFB single frame update */
	STATE4A,
	STATE4B,
	STATE4C,
	STATE4D,
	STATE5,		/* Exiting from PSR active state */
	STATE5A,
	STATE5B,
	STATE5C
};

enum phy_type {
	PHY_TYPE_UNKNOWN = 1,
	PHY_TYPE_PCIE_PHY = 2,
	PHY_TYPE_UNIPHY = 3,
};

struct dmcu_context {
	enum channel_id channel;
	enum transmitter transmitter_id;
	enum engine_id engine_id;
	enum controller_id controller_id;
	enum phy_type phy_type;
	enum physical_phy_id smu_physical_phy_id;

	/* Vertical total pixels from crtc timing.
	 * This is used for static screen detection.
	 * ie. If we want to detect half a frame,
	 * we use this to determine the hyst lines.*/
	uint32_t crtc_timing_vertical_total;

	/* PSR supported from panel capabilities
	 * and current display configuration */
	bool psr_supported_display_config;

	/* Whether fast link training is supported by the panel */
	bool psr_exit_link_training_required;

	/* If RFB setup time is greater than the total VBLANK time, it is not
	 * possible for the sink to capture the video frame in the same frame
	 * the SDP is sent. In this case, the frame capture indication bit
	 * should be set and an extra static frame should be transmitted to
	 * the sink */
	bool psr_frame_capture_indication_required;

	/* Set the last possible line SDP may be transmitted without violating
	 * the RFB setup time */
	bool sdp_transmit_line_num_deadline;

	/* The VSync rate in Hz used to calculate the step size
	 * for smooth brightness feature */
	uint32_t vsync_rate_hz;
};

union dmcu_psr_level {
	struct {
		bool SKIP_CRC:1;
		bool SKIP_DP_VID_STREAM_DISABLE:1;
		bool SKIP_PHY_POWER_DOWN:1;
		bool SKIP_AUX_ACK_CHECK:1;
		bool SKIP_CRTC_DISABLE:1;
		bool SKIP_AUX_RFB_CAPTURE_CHECK:1;
		bool SKIP_SMU_NOTIFICATION:1;
		bool SKIP_AUTO_STATE_ADVANCE:1;
		bool DISABLE_PSR_ENTRY_ABORT:1;
	} bits;
	uint32_t u32all;
};

struct dmcu_config_data {
	/* Command sent to DMCU. */
	enum dmcu_action action;
	/* PSR Level controls which HW blocks to power down during PSR active,
	 * and also other sequence modifications. */
	union dmcu_psr_level psr_level;
	/* To indicate that first changed frame from active state should not
	 * result in exit to inactive state, but instead perform an automatic
	 * single frame RFB update. */
	bool rfb_update_auto_en;
	/* Number of consecutive static frames to detect before entering PSR
	 * active state. */
	uint32_t hyst_frames;
	/* Partial frames before entering PSR active. Note this parameter is in
	 * units of 100 lines. i.e. Wait a value of 5 means wait 500 additional
	 * lines. */
	uint32_t hyst_lines;
	/* Number of repeated AUX retries before indicating failure to driver.
	 * In a working case, first attempt to write/read AUX should pass. */
	uint32_t aux_repeat;
	/* Additional delay after remote frame capture before continuing to
	 * power down. This is mainly for debug purposes to identify timing
	 * issues. */
	uint32_t frame_delay;
	/* Controls how long the delay of a wait loop is. It should be tuned
	 * to 1 us, and needs to be reconfigured every time DISPCLK changes. */
	uint32_t wait_loop_num;
};

struct dmcu_output_data {
	/* DMCU reply */
	enum dmcu_output output;
	/* The current PSR state. */
	uint32_t psr_state;
	/* The number of frames during PSR active state. */
	uint32_t psr_count;
};

enum varibright_command {
	VARIBRIGHT_CMD_SET_VB_LEVEL = 0,
	VARIBRIGHT_CMD_USER_ENABLE,
	VARIBRIGHT_CMD_POST_DISPLAY_CONFIG,
	VARIBRIGHT_CMD_UNKNOWN
};

struct varibright_control {
	enum varibright_command command;
	uint8_t level;
	bool enable;
	bool activate;
};

#endif /* __DAL_DMCU_TYPES_H__ */
