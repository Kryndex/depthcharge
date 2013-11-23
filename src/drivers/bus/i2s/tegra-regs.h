/*
 * This file is part of the depthcharge project.
 * Copyright 2013 Google Inc.  All rights reserved.
 *
 * tegra-regs.h - Definitions for Tegra124 I2S driver.
 * Note, some structures (ex, CIF) are different in Tegra114.
 *
 * Copyright (c) 2011-2013, NVIDIA CORPORATION.  All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __DRIVERS_BUS_I2S_TEGRA_REGS_H__
#define __DRIVERS_BUS_I2S_TEGRA_REGS_H__

#include <stdint.h>

typedef struct TegraI2sRegs {
	uint32_t ctrl;		/* I2S_CTRL_0, 0x00 */
	uint32_t timing;	/* I2S_TIMING_0, 0x04 */
	uint32_t offset;	/* I2S_OFFSET_0, 0x08 */
	uint32_t ch_ctrl;	/* I2S_CH_CTRL_0, 0x0C */
	uint32_t slot_ctrl;	/* I2S_SLOT_CTRL_0, 0x10 */
	uint32_t cif_tx_ctrl;	/* I2S_CIF_TX_CTRL_0, 0x14 */
	uint32_t cif_rx_ctrl;	/* I2S_CIF_RX_CTRL_0, 0x18 */
	uint32_t flowctl;	/* I2S_FLOWCTL_0, 0x1C */
	uint32_t tx_step;	/* I2S_TX_STEP_0, 0x20 */
	uint32_t flow_status;	/* I2S_FLOW_STATUS_0, 0x24 */
	uint32_t flow_total;	/* I2S_FLOW_TOTAL_0, 0x28 */
	uint32_t flow_over;	/* I2S_FLOW_OVER_0, 0x2C */
	uint32_t flow_under;	/* I2S_FLOW_UNDER_0, 0x30 */
	uint32_t reserved[12];	/* RESERVED, 0x34 - 0x60 */
	uint32_t slot_ctrl2;	/* I2S_SLOT_CTRL2_0, 0x64*/
} TegraI2sRegs;

enum {

	I2S_CTRL_XFER_EN_TX = 1 << 31,
	I2S_CTRL_XFER_EN_RX = 1 << 30,
	I2S_CTRL_CG_EN = 1 << 29,
	I2S_CTRL_SOFT_RESET = 1 << 28,
	I2S_CTRL_TX_FLOWCTL_EN = 1 << 27,

	I2S_CTRL_OBS_SEL_SHIFT = 24,
	I2S_CTRL_OBS_SEL_MASK = 7 << I2S_CTRL_OBS_SEL_SHIFT,

	I2S_FRAME_FORMAT_LRCK = 0,
	I2S_FRAME_FORMAT_FSYNC = 1,

	I2S_CTRL_FRAME_FORMAT_SHIFT = 12,
	I2S_CTRL_FRAME_FORMAT_MASK = 7 << I2S_CTRL_FRAME_FORMAT_SHIFT,
	I2S_CTRL_FRAME_FORMAT_LRCK = I2S_FRAME_FORMAT_LRCK <<
				     I2S_CTRL_FRAME_FORMAT_SHIFT,
	I2S_CTRL_FRAME_FORMAT_FSYNC = I2S_FRAME_FORMAT_FSYNC <<
				      I2S_CTRL_FRAME_FORMAT_SHIFT,

	I2S_CTRL_MASTER_ENABLE = 1 << 10,

	I2S_LRCK_LEFT_LOW = 0,
	I2S_LRCK_RIGHT_LOW = 1,

	I2S_CTRL_LRCK_SHIFT = 9,
	I2S_CTRL_LRCK_MASK = 1 << I2S_CTRL_LRCK_SHIFT,
	I2S_CTRL_LRCK_L_LOW = I2S_LRCK_LEFT_LOW  << I2S_CTRL_LRCK_SHIFT,
	I2S_CTRL_LRCK_R_LOW = I2S_LRCK_RIGHT_LOW << I2S_CTRL_LRCK_SHIFT,

	I2S_CTRL_LPBK_ENABLE = 1 << 8,

	I2S_BIT_CODE_LINEAR = 0,
	I2S_BIT_CODE_ULAW = 1,
	I2S_BIT_CODE_ALAW = 2,

	I2S_CTRL_BIT_CODE_SHIFT = 4,
	I2S_CTRL_BIT_CODE_MASK = 3 << I2S_CTRL_BIT_CODE_SHIFT,
	I2S_CTRL_BIT_CODE_LINEAR = I2S_BIT_CODE_LINEAR <<
				   I2S_CTRL_BIT_CODE_SHIFT,
	I2S_CTRL_BIT_CODE_ULAW = I2S_BIT_CODE_ULAW << I2S_CTRL_BIT_CODE_SHIFT,
	I2S_CTRL_BIT_CODE_ALAW = I2S_BIT_CODE_ALAW << I2S_CTRL_BIT_CODE_SHIFT,

	I2S_BITS_8 = 1,
	I2S_BITS_12 = 2,
	I2S_BITS_16 = 3,
	I2S_BITS_20 = 4,
	I2S_BITS_24 = 5,
	I2S_BITS_28 = 6,
	I2S_BITS_32 = 7,

	/* Sample container size; see {RX,TX}_MASK field in CH_CTRL below */
	I2S_CTRL_BIT_SIZE_SHIFT = 0,
	I2S_CTRL_BIT_SIZE_MASK = 7 << I2S_CTRL_BIT_SIZE_SHIFT,
	I2S_CTRL_BIT_SIZE_8 = I2S_BITS_8  << I2S_CTRL_BIT_SIZE_SHIFT,
	I2S_CTRL_BIT_SIZE_12 = I2S_BITS_12 << I2S_CTRL_BIT_SIZE_SHIFT,
	I2S_CTRL_BIT_SIZE_16 = I2S_BITS_16 << I2S_CTRL_BIT_SIZE_SHIFT,
	I2S_CTRL_BIT_SIZE_20 = I2S_BITS_20 << I2S_CTRL_BIT_SIZE_SHIFT,
	I2S_CTRL_BIT_SIZE_24 = I2S_BITS_24 << I2S_CTRL_BIT_SIZE_SHIFT,
	I2S_CTRL_BIT_SIZE_28 = I2S_BITS_28 << I2S_CTRL_BIT_SIZE_SHIFT,
	I2S_CTRL_BIT_SIZE_32 = I2S_BITS_32 << I2S_CTRL_BIT_SIZE_SHIFT,

	I2S_TIMING_NON_SYM_ENABLE = 1 << 12,
	I2S_TIMING_CHANNEL_BIT_COUNT_SHIFT = 0,
	I2S_TIMING_CHANNEL_BIT_COUNT_MASK_US = 0x7ff,
	I2S_TIMING_CHANNEL_BIT_COUNT_MASK =
			I2S_TIMING_CHANNEL_BIT_COUNT_MASK_US <<
			I2S_TIMING_CHANNEL_BIT_COUNT_SHIFT,

	I2S_OFFSET_RX_DATA_OFFSET_SHIFT = 16,
	I2S_OFFSET_RX_DATA_OFFSET_MASK_US = 0x7ff,
	I2S_OFFSET_RX_DATA_OFFSET_MASK = I2S_OFFSET_RX_DATA_OFFSET_MASK_US <<
					 I2S_OFFSET_RX_DATA_OFFSET_SHIFT,
	I2S_OFFSET_TX_DATA_OFFSET_SHIFT = 0,
	I2S_OFFSET_TX_DATA_OFFSET_MASK_US = 0x7ff,
	I2S_OFFSET_TX_DATA_OFFSET_MASK = I2S_OFFSET_TX_DATA_OFFSET_MASK_US <<
					 I2S_OFFSET_TX_DATA_OFFSET_SHIFT,

	/* FSYNC width - 1 in bit clocks */
	I2S_CH_CTRL_FSYNC_WIDTH_SHIFT = 24,
	I2S_CH_CTRL_FSYNC_WIDTH_MASK_US = 0xff,
	I2S_CH_CTRL_FSYNC_WIDTH_MASK = I2S_CH_CTRL_FSYNC_WIDTH_MASK_US <<
				       I2S_CH_CTRL_FSYNC_WIDTH_SHIFT,

	I2S_HIGHZ_NO = 0,
	I2S_HIGHZ_YES = 1,
	I2S_HIGHZ_ON_HALF_BIT_CLK = 2,

	I2S_CH_CTRL_HIGHZ_CTRL_SHIFT = 12,
	I2S_CH_CTRL_HIGHZ_CTRL_MASK = 3 << I2S_CH_CTRL_HIGHZ_CTRL_SHIFT,
	I2S_CH_CTRL_HIGHZ_CTRL_NO = I2S_HIGHZ_NO <<
				    I2S_CH_CTRL_HIGHZ_CTRL_SHIFT,
	I2S_CH_CTRL_HIGHZ_CTRL_YES = I2S_HIGHZ_YES <<
				     I2S_CH_CTRL_HIGHZ_CTRL_SHIFT,
	I2S_CH_CTRL_HIGHZ_CTRL_ON_HALF_BIT_CLK = I2S_HIGHZ_ON_HALF_BIT_CLK <<
						 I2S_CH_CTRL_HIGHZ_CTRL_SHIFT,

	I2S_MSB_FIRST = 0,
	I2S_LSB_FIRST = 1,

	I2S_CH_CTRL_RX_BIT_ORDER_SHIFT = 10,
	I2S_CH_CTRL_RX_BIT_ORDER_MASK = 1 << I2S_CH_CTRL_RX_BIT_ORDER_SHIFT,
	I2S_CH_CTRL_RX_BIT_ORDER_MSB_FIRST = I2S_MSB_FIRST <<
					     I2S_CH_CTRL_RX_BIT_ORDER_SHIFT,
	I2S_CH_CTRL_RX_BIT_ORDER_LSB_FIRST = I2S_LSB_FIRST <<
					     I2S_CH_CTRL_RX_BIT_ORDER_SHIFT,
	I2S_CH_CTRL_TX_BIT_ORDER_SHIFT = 9,
	I2S_CH_CTRL_TX_BIT_ORDER_MASK = 1 << I2S_CH_CTRL_TX_BIT_ORDER_SHIFT,
	I2S_CH_CTRL_TX_BIT_ORDER_MSB_FIRST = I2S_MSB_FIRST <<
					     I2S_CH_CTRL_TX_BIT_ORDER_SHIFT,
	I2S_CH_CTRL_TX_BIT_ORDER_LSB_FIRST = I2S_LSB_FIRST <<
					     I2S_CH_CTRL_TX_BIT_ORDER_SHIFT,

	I2S_POS_EDGE = 0,
	I2S_NEG_EDGE = 1,

	I2S_CH_CTRL_EGDE_CTRL_SHIFT = 8,
	I2S_CH_CTRL_EGDE_CTRL_MASK = 1 << I2S_CH_CTRL_EGDE_CTRL_SHIFT,
	I2S_CH_CTRL_EGDE_CTRL_POS_EDGE = I2S_POS_EDGE <<
					 I2S_CH_CTRL_EGDE_CTRL_SHIFT,
	I2S_CH_CTRL_EGDE_CTRL_NEG_EDGE = I2S_NEG_EDGE <<
					 I2S_CH_CTRL_EGDE_CTRL_SHIFT,

	/* Sample size is # bits from BIT_SIZE minus this field */
	I2S_CH_CTRL_RX_MASK_BITS_SHIFT = 4,
	I2S_CH_CTRL_RX_MASK_BITS_MASK_US = 7,
	I2S_CH_CTRL_RX_MASK_BITS_MASK = I2S_CH_CTRL_RX_MASK_BITS_MASK_US <<
					I2S_CH_CTRL_RX_MASK_BITS_SHIFT,

	I2S_CH_CTRL_TX_MASK_BITS_SHIFT = 0,
	I2S_CH_CTRL_TX_MASK_BITS_MASK_US = 7,
	I2S_CH_CTRL_TX_MASK_BITS_MASK = I2S_CH_CTRL_TX_MASK_BITS_MASK_US <<
					I2S_CH_CTRL_TX_MASK_BITS_SHIFT,

	/* Number of slots in frame, minus 1 */
	I2S_SLOT_CTRL_TOTAL_SLOTS_SHIFT = 16,
	I2S_SLOT_CTRL_TOTAL_SLOTS_MASK_US = 7,
	I2S_SLOT_CTRL_TOTAL_SLOTS_MASK = I2S_SLOT_CTRL_TOTAL_SLOTS_MASK_US <<
					 I2S_SLOT_CTRL_TOTAL_SLOTS_SHIFT,

	/* TDM mode slot enable bitmask */
	I2S_SLOT_CTRL_RX_SLOT_ENABLES_SHIFT = 8,
	I2S_SLOT_CTRL_RX_SLOT_ENABLES_MASK =
			0xff <<I2S_SLOT_CTRL_RX_SLOT_ENABLES_SHIFT,

	I2S_SLOT_CTRL_TX_SLOT_ENABLES_SHIFT = 0,
	I2S_SLOT_CTRL_TX_SLOT_ENABLES_MASK = 0xff <<
					   I2S_SLOT_CTRL_TX_SLOT_ENABLES_SHIFT,

	I2S_FILTER_LINEAR = 0,
	I2S_FILTER_QUAD = 1,

	I2S_FLOWCTL_FILTER_SHIFT = 31,
	I2S_FLOWCTL_FILTER_MASK = 1 << I2S_FLOWCTL_FILTER_SHIFT,
	I2S_FLOWCTL_FILTER_LINEAR = I2S_FILTER_LINEAR <<
				    I2S_FLOWCTL_FILTER_SHIFT,
	I2S_FLOWCTL_FILTER_QUAD = I2S_FILTER_QUAD << I2S_FLOWCTL_FILTER_SHIFT,

	I2S_TX_STEP_SHIFT = 0,
	I2S_TX_STEP_MASK_US = 0xffff,
	I2S_TX_STEP_MASK = I2S_TX_STEP_MASK_US << I2S_TX_STEP_SHIFT,

	I2S_FLOW_STATUS_UNDERFLOW = 1 << 31,
	I2S_FLOW_STATUS_OVERFLOW = 1 << 30,
	I2S_FLOW_STATUS_MONITOR_INT_EN = 1 << 4,
	I2S_FLOW_STATUS_COUNTER_CLR = 1 << 3,
	I2S_FLOW_STATUS_MONITOR_CLR = 1 << 2,
	I2S_FLOW_STATUS_COUNTER_EN = 1 << 1,
	I2S_FLOW_STATUS_MONITOR_EN = 1 << 0,
};

#endif // __DRIVERS_BUS_I2S_TEGRA_REGS_H__