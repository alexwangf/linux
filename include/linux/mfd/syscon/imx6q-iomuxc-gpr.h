/*
 * Copyright (C) 2012-2013 Freescale Semiconductor, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef __LINUX_IMX6Q_IOMUXC_GPR_H
#define __LINUX_IMX6Q_IOMUXC_GPR_H

#include <linux/bitops.h>

#define IOMUXC_GPR0	0x00
#define IOMUXC_GPR1	0x04
#define IOMUXC_GPR2	0x08
#define IOMUXC_GPR3	0x0c
#define IOMUXC_GPR4	0x10
#define IOMUXC_GPR5	0x14
#define IOMUXC_GPR6	0x18
#define IOMUXC_GPR7	0x1c
#define IOMUXC_GPR8	0x20
#define IOMUXC_GPR9	0x24
#define IOMUXC_GPR10	0x28
#define IOMUXC_GPR11	0x2c
#define IOMUXC_GPR12	0x30
#define IOMUXC_GPR13	0x34

#define IMX6Q_GPR0_CLOCK_8_MUX_SEL_MASK		(0x3 << 30)
#define IMX6Q_GPR0_CLOCK_8_MUX_SEL_AUDMUX_RXCLK_P7_MUXED	(0x0 << 30)
#define IMX6Q_GPR0_CLOCK_8_MUX_SEL_AUDMUX_RXCLK_P7	(0x1 << 30)
#define IMX6Q_GPR0_CLOCK_8_MUX_SEL_SSI3_SSI_SRCK	(0x2 << 30)
#define IMX6Q_GPR0_CLOCK_8_MUX_SEL_SSI3_RX_BIT_CLK	(0x3 << 30)
#define IMX6Q_GPR0_CLOCK_0_MUX_SEL_MASK		(0x3 << 28)
#define IMX6Q_GPR0_CLOCK_0_MUX_SEL_ESAI1_IPP_IND_SCKR_MUXED	(0x0 << 28)
#define IMX6Q_GPR0_CLOCK_0_MUX_SEL_ESAI1_IPP_IND_SCKR	(0x1 << 28)
#define IMX6Q_GPR0_CLOCK_0_MUX_SEL_ESAI1_IPP_DO_SCKR	(0x2 << 28)
#define IMX6Q_GPR0_CLOCK_B_MUX_SEL_MASK		(0x3 << 26)
#define IMX6Q_GPR0_CLOCK_B_MUX_SEL_AUDMUX_TXCLK_P7_MUXED	(0x0 << 26)
#define IMX6Q_GPR0_CLOCK_B_MUX_SEL_AUDMUX_TXCLK_P7	(0x1 << 26)
#define IMX6Q_GPR0_CLOCK_B_MUX_SEL_SSI3_SSI_STCK	(0x2 << 26)
#define IMX6Q_GPR0_CLOCK_B_MUX_SEL_SSI3_TX_BIT_CLK	(0x3 << 26)
#define IMX6Q_GPR0_CLOCK_3_MUX_SEL_MASK		(0x3 << 24)
#define IMX6Q_GPR0_CLOCK_3_MUX_SEL_AUDMUX_RXCLK_P7_MUXED	(0x3 << 24)
#define IMX6Q_GPR0_CLOCK_3_MUX_SEL_AUDMUX_RXCLK_P7	(0x3 << 24)
#define IMX6Q_GPR0_CLOCK_3_MUX_SEL_SSI3_SSI_SRCK	(0x3 << 24)
#define IMX6Q_GPR0_CLOCK_3_MUX_SEL_SSI3_RX_BIT_CLK	(0x3 << 24)
#define IMX6Q_GPR0_CLOCK_A_MUX_SEL_MASK		(0x3 << 22)
#define IMX6Q_GPR0_CLOCK_A_MUX_SEL_AUDMUX_TXCLK_P2_MUXED	(0x0 << 22)
#define IMX6Q_GPR0_CLOCK_A_MUX_SEL_AUDMUX_TXCLK_P2	(0x1 << 22)
#define IMX6Q_GPR0_CLOCK_A_MUX_SEL_SSI2_SSI_STCK	(0x2 << 22)
#define IMX6Q_GPR0_CLOCK_A_MUX_SEL_SSI2_TX_BIT_CLK	(0x3 << 22)
#define IMX6Q_GPR0_CLOCK_2_MUX_SEL_MASK		(0x3 << 20)
#define IMX6Q_GPR0_CLOCK_2_MUX_SEL_AUDMUX_RXCLK_P2_MUXED	(0x0 << 20)
#define IMX6Q_GPR0_CLOCK_2_MUX_SEL_AUDMUX_RXCLK_P2	(0x1 << 20)
#define IMX6Q_GPR0_CLOCK_2_MUX_SEL_SSI2_SSI_SRCK	(0x2 << 20)
#define IMX6Q_GPR0_CLOCK_2_MUX_SEL_SSI2_RX_BIT_CLK	(0x3 << 20)
#define IMX6Q_GPR0_CLOCK_9_MUX_SEL_MASK		(0x3 << 18)
#define IMX6Q_GPR0_CLOCK_9_MUX_SEL_AUDMUX_TXCLK_P1_MUXED	(0x0 << 18)
#define IMX6Q_GPR0_CLOCK_9_MUX_SEL_AUDMUX_TXCLK_P1	(0x1 << 18)
#define IMX6Q_GPR0_CLOCK_9_MUX_SEL_SSI1_SSI_STCK	(0x2 << 18)
#define IMX6Q_GPR0_CLOCK_9_MUX_SEL_SSI1_SSI_TX_BIT_CLK	(0x3 << 18)
#define IMX6Q_GPR0_CLOCK_1_MUX_SEL_MASK		(0x3 << 16)
#define IMX6Q_GPR0_CLOCK_1_MUX_SEL_AUDMUX_RXCLK_P1_MUXED	(0x0 << 16)
#define IMX6Q_GPR0_CLOCK_1_MUX_SEL_AUDMUX_RXCLK_P1	(0x1 << 16)
#define IMX6Q_GPR0_CLOCK_1_MUX_SEL_SSI1_SSI_SRCK	(0x2 << 16)
#define IMX6Q_GPR0_CLOCK_1_MUX_SEL_SSI1_SSI_RX_BIT_CLK	(0x3 << 16)
#define IMX6Q_GPR0_TX_CLK2_MUX_SEL_MASK		(0x3 << 14)
#define IMX6Q_GPR0_TX_CLK2_MUX_SEL_ASRCK_CLK1	(0x0 << 14)
#define IMX6Q_GPR0_TX_CLK2_MUX_SEL_ASRCK_CLK2	(0x1 << 14)
#define IMX6Q_GPR0_TX_CLK2_MUX_SEL_ASRCK_CLK3	(0x2 << 14)
#define IMX6Q_GPR0_DMAREQ_MUX_SEL7_MASK		BIT(7)
#define IMX6Q_GPR0_DMAREQ_MUX_SEL7_SPDIF	0x0
#define IMX6Q_GPR0_DMAREQ_MUX_SEL7_IOMUX	BIT(7)
#define IMX6Q_GPR0_DMAREQ_MUX_SEL6_MASK		BIT(6)
#define IMX6Q_GPR0_DMAREQ_MUX_SEL6_ESAI		0x0
#define IMX6Q_GPR0_DMAREQ_MUX_SEL6_I2C3		BIT(6)
#define IMX6Q_GPR0_DMAREQ_MUX_SEL5_MASK		BIT(5)
#define IMX6Q_GPR0_DMAREQ_MUX_SEL5_ECSPI4	0x0
#define IMX6Q_GPR0_DMAREQ_MUX_SEL5_EPIT2	BIT(5)
#define IMX6Q_GPR0_DMAREQ_MUX_SEL4_MASK		BIT(4)
#define IMX6Q_GPR0_DMAREQ_MUX_SEL4_ECSPI4	0x0
#define IMX6Q_GPR0_DMAREQ_MUX_SEL4_I2C1		BIT(4)
#define IMX6Q_GPR0_DMAREQ_MUX_SEL3_MASK		BIT(3)
#define IMX6Q_GPR0_DMAREQ_MUX_SEL3_ECSPI2	0x0
#define IMX6Q_GPR0_DMAREQ_MUX_SEL3_I2C1		BIT(3)
#define IMX6Q_GPR0_DMAREQ_MUX_SEL2_MASK		BIT(2)
#define IMX6Q_GPR0_DMAREQ_MUX_SEL2_ECSPI1	0x0
#define IMX6Q_GPR0_DMAREQ_MUX_SEL2_I2C2		BIT(2)
#define IMX6Q_GPR0_DMAREQ_MUX_SEL1_MASK		BIT(1)
#define IMX6Q_GPR0_DMAREQ_MUX_SEL1_ECSPI1	0x0
#define IMX6Q_GPR0_DMAREQ_MUX_SEL1_I2C3		BIT(1)
#define IMX6Q_GPR0_DMAREQ_MUX_SEL0_MASK		BIT(0)
#define IMX6Q_GPR0_DMAREQ_MUX_SEL0_IPU1		0x0
#define IMX6Q_GPR0_DMAREQ_MUX_SEL0_IOMUX	BIT(0)

#define IMX6Q_GPR1_PCIE_REQ_MASK		(0x3 << 30)
#define IMX6Q_GPR1_PCIE_EXIT_L1			BIT(28)
#define IMX6Q_GPR1_PCIE_RDY_L23			BIT(27)
#define IMX6Q_GPR1_PCIE_ENTER_L1		BIT(26)
#define IMX6Q_GPR1_MIPI_COLOR_SW		BIT(25)
#define IMX6Q_GPR1_DPI_OFF			BIT(24)
#define IMX6Q_GPR1_EXC_MON_MASK			BIT(22)
#define IMX6Q_GPR1_EXC_MON_OKAY			0x0
#define IMX6Q_GPR1_EXC_MON_SLVE			BIT(22)
#define IMX6Q_GPR1_ENET_CLK_SEL_MASK		BIT(21)
#define IMX6Q_GPR1_ENET_CLK_SEL_PAD		0
#define IMX6Q_GPR1_ENET_CLK_SEL_ANATOP		BIT(21)
#define IMX6Q_GPR1_MIPI_IPU2_MUX_MASK		BIT(20)
#define IMX6Q_GPR1_MIPI_IPU2_MUX_GASKET		0x0
#define IMX6Q_GPR1_MIPI_IPU2_MUX_IOMUX		BIT(20)
#define IMX6Q_GPR1_MIPI_IPU1_MUX_MASK		BIT(19)
#define IMX6Q_GPR1_MIPI_IPU1_MUX_GASKET		0x0
#define IMX6Q_GPR1_MIPI_IPU1_MUX_IOMUX		BIT(19)
#define IMX6Q_GPR1_PCIE_TEST_PD			BIT(18)
#define IMX6Q_GPR1_IPU_VPU_MUX_MASK		BIT(17)
#define IMX6Q_GPR1_IPU_VPU_MUX_IPU1		0x0
#define IMX6Q_GPR1_IPU_VPU_MUX_IPU2		BIT(17)
#define IMX6Q_GPR1_PCIE_REF_CLK_EN		BIT(16)
#define IMX6Q_GPR1_USB_EXP_MODE			BIT(15)
#define IMX6Q_GPR1_PCIE_INT			BIT(14)
#define IMX6Q_GPR1_USB_OTG_ID_SEL_MASK		BIT(13)
#define IMX6Q_GPR1_USB_OTG_ID_SEL_ENET_RX_ER	0x0
#define IMX6Q_GPR1_USB_OTG_ID_SEL_GPIO_1	BIT(13)
#define IMX6Q_GPR1_GINT_MASK			BIT(12)
#define IMX6Q_GPR1_GINT_CLEAR			0x0
#define IMX6Q_GPR1_GINT_ASSERT			BIT(12)
#define IMX6Q_GPR1_ADDRS3_MASK			(0x3 << 10)
#define IMX6Q_GPR1_ADDRS3_32MB			(0x0 << 10)
#define IMX6Q_GPR1_ADDRS3_64MB			(0x1 << 10)
#define IMX6Q_GPR1_ADDRS3_128MB			(0x2 << 10)
#define IMX6Q_GPR1_ACT_CS3			BIT(9)
#define IMX6Q_GPR1_ADDRS2_MASK			(0x3 << 7)
#define IMX6Q_GPR1_ACT_CS2			BIT(6)
#define IMX6Q_GPR1_ADDRS1_MASK			(0x3 << 4)
#define IMX6Q_GPR1_ACT_CS1			BIT(3)
#define IMX6Q_GPR1_ADDRS0_MASK			(0x3 << 1)
#define IMX6Q_GPR1_ACT_CS0			BIT(0)

#define IMX6Q_GPR2_COUNTER_RESET_VAL_MASK	(0x3 << 20)
#define IMX6Q_GPR2_COUNTER_RESET_VAL_5		(0x0 << 20)
#define IMX6Q_GPR2_COUNTER_RESET_VAL_3		(0x1 << 20)
#define IMX6Q_GPR2_COUNTER_RESET_VAL_4		(0x2 << 20)
#define IMX6Q_GPR2_COUNTER_RESET_VAL_6		(0x3 << 20)
#define IMX6Q_GPR2_LVDS_CLK_SHIFT_MASK		(0x7 << 16)
#define IMX6Q_GPR2_LVDS_CLK_SHIFT_0		(0x0 << 16)
#define IMX6Q_GPR2_LVDS_CLK_SHIFT_1		(0x1 << 16)
#define IMX6Q_GPR2_LVDS_CLK_SHIFT_2		(0x2 << 16)
#define IMX6Q_GPR2_LVDS_CLK_SHIFT_3		(0x3 << 16)
#define IMX6Q_GPR2_LVDS_CLK_SHIFT_4		(0x4 << 16)
#define IMX6Q_GPR2_LVDS_CLK_SHIFT_5		(0x5 << 16)
#define IMX6Q_GPR2_LVDS_CLK_SHIFT_6		(0x6 << 16)
#define IMX6Q_GPR2_LVDS_CLK_SHIFT_7		(0x7 << 16)
#define IMX6Q_GPR2_BGREF_RRMODE_MASK		BIT(15)
#define IMX6Q_GPR2_BGREF_RRMODE_EXT_RESISTOR	0x0
#define IMX6Q_GPR2_BGREF_RRMODE_INT_RESISTOR	BIT(15)
#define IMX6Q_GPR2_DI1_VS_POLARITY_MASK		BIT(10)
#define IMX6Q_GPR2_DI1_VS_POLARITY_ACTIVE_H	0x0
#define IMX6Q_GPR2_DI1_VS_POLARITY_ACTIVE_L	BIT(10)
#define IMX6Q_GPR2_DI0_VS_POLARITY_MASK		BIT(9)
#define IMX6Q_GPR2_DI0_VS_POLARITY_ACTIVE_H	0x0
#define IMX6Q_GPR2_DI0_VS_POLARITY_ACTIVE_L	BIT(9)
#define IMX6Q_GPR2_BIT_MAPPING_CH1_MASK		BIT(8)
#define IMX6Q_GPR2_BIT_MAPPING_CH1_SPWG		0x0
#define IMX6Q_GPR2_BIT_MAPPING_CH1_JEIDA	BIT(8)
#define IMX6Q_GPR2_DATA_WIDTH_CH1_MASK		BIT(7)
#define IMX6Q_GPR2_DATA_WIDTH_CH1_18BIT		0x0
#define IMX6Q_GPR2_DATA_WIDTH_CH1_24BIT		BIT(7)
#define IMX6Q_GPR2_BIT_MAPPING_CH0_MASK		BIT(6)
#define IMX6Q_GPR2_BIT_MAPPING_CH0_SPWG		0x0
#define IMX6Q_GPR2_BIT_MAPPING_CH0_JEIDA	BIT(6)
#define IMX6Q_GPR2_DATA_WIDTH_CH0_MASK		BIT(5)
#define IMX6Q_GPR2_DATA_WIDTH_CH0_18BIT		0x0
#define IMX6Q_GPR2_DATA_WIDTH_CH0_24BIT		BIT(5)
#define IMX6Q_GPR2_SPLIT_MODE_EN		BIT(4)
#define IMX6Q_GPR2_CH1_MODE_MASK		(0x3 << 2)
#define IMX6Q_GPR2_CH1_MODE_DISABLE		(0x0 << 2)
#define IMX6Q_GPR2_CH1_MODE_EN_ROUTE_DI0	(0x1 << 2)
#define IMX6Q_GPR2_CH1_MODE_EN_ROUTE_DI1	(0x3 << 2)
#define IMX6Q_GPR2_CH0_MODE_MASK		(0x3 << 0)
#define IMX6Q_GPR2_CH0_MODE_DISABLE		(0x0 << 0)
#define IMX6Q_GPR2_CH0_MODE_EN_ROUTE_DI0	(0x1 << 0)
#define IMX6Q_GPR2_CH0_MODE_EN_ROUTE_DI1	(0x3 << 0)

#define IMX6Q_GPR3_GPU_DBG_MASK			(0x3 << 29)
#define IMX6Q_GPR3_GPU_DBG_GPU3D		(0x0 << 29)
#define IMX6Q_GPR3_GPU_DBG_GPU2D		(0x1 << 29)
#define IMX6Q_GPR3_GPU_DBG_OPENVG		(0x2 << 29)
#define IMX6Q_GPR3_BCH_WR_CACHE_CTL		BIT(28)
#define IMX6Q_GPR3_BCH_RD_CACHE_CTL		BIT(27)
#define IMX6Q_GPR3_USDHCX_WR_CACHE_CTL		BIT(26)
#define IMX6Q_GPR3_USDHCX_RD_CACHE_CTL		BIT(25)
#define IMX6Q_GPR3_OCRAM_CTL_MASK		(0xf << 21)
#define IMX6Q_GPR3_OCRAM_STATUS_MASK		(0xf << 17)
#define IMX6Q_GPR3_CORE3_DBG_ACK_EN		BIT(16)
#define IMX6Q_GPR3_CORE2_DBG_ACK_EN		BIT(15)
#define IMX6Q_GPR3_CORE1_DBG_ACK_EN		BIT(14)
#define IMX6Q_GPR3_CORE0_DBG_ACK_EN		BIT(13)
#define IMX6Q_GPR3_TZASC2_BOOT_LOCK		BIT(12)
#define IMX6Q_GPR3_TZASC1_BOOT_LOCK		BIT(11)
#define IMX6Q_GPR3_IPU_DIAG_MASK		BIT(10)
#define IMX6Q_GPR3_LVDS1_MUX_CTL_MASK		(0x3 << 8)
#define IMX6Q_GPR3_LVDS1_MUX_CTL_IPU1_DI0	(0x0 << 8)
#define IMX6Q_GPR3_LVDS1_MUX_CTL_IPU1_DI1	(0x1 << 8)
#define IMX6Q_GPR3_LVDS1_MUX_CTL_IPU2_DI0	(0x2 << 8)
#define IMX6Q_GPR3_LVDS1_MUX_CTL_IPU2_DI1	(0x3 << 8)
#define IMX6Q_GPR3_LVDS0_MUX_CTL_MASK		(0x3 << 6)
#define IMX6Q_GPR3_LVDS0_MUX_CTL_IPU1_DI0	(0x0 << 6)
#define IMX6Q_GPR3_LVDS0_MUX_CTL_IPU1_DI1	(0x1 << 6)
#define IMX6Q_GPR3_LVDS0_MUX_CTL_IPU2_DI0	(0x2 << 6)
#define IMX6Q_GPR3_LVDS0_MUX_CTL_IPU2_DI1	(0x3 << 6)
#define IMX6Q_GPR3_MIPI_MUX_CTL_MASK		(0x3 << 4)
#define IMX6Q_GPR3_MIPI_MUX_CTL_IPU1_DI0	(0x0 << 4)
#define IMX6Q_GPR3_MIPI_MUX_CTL_IPU1_DI1	(0x1 << 4)
#define IMX6Q_GPR3_MIPI_MUX_CTL_IPU2_DI0	(0x2 << 4)
#define IMX6Q_GPR3_MIPI_MUX_CTL_IPU2_DI1	(0x3 << 4)
#define IMX6Q_GPR3_HDMI_MUX_CTL_MASK		(0x3 << 2)
#define IMX6Q_GPR3_HDMI_MUX_CTL_IPU1_DI0	(0x0 << 2)
#define IMX6Q_GPR3_HDMI_MUX_CTL_IPU1_DI1	(0x1 << 2)
#define IMX6Q_GPR3_HDMI_MUX_CTL_IPU2_DI0	(0x2 << 2)
#define IMX6Q_GPR3_HDMI_MUX_CTL_IPU2_DI1	(0x3 << 2)

#define IMX6Q_GPR4_VDOA_WR_CACHE_SEL		BIT(31)
#define IMX6Q_GPR4_VDOA_RD_CACHE_SEL		BIT(30)
#define IMX6Q_GPR4_VDOA_WR_CACHE_VAL		BIT(29)
#define IMX6Q_GPR4_VDOA_RD_CACHE_VAL		BIT(28)
#define IMX6Q_GPR4_PCIE_WR_CACHE_SEL		BIT(27)
#define IMX6Q_GPR4_PCIE_RD_CACHE_SEL		BIT(26)
#define IMX6Q_GPR4_PCIE_WR_CACHE_VAL		BIT(25)
#define IMX6Q_GPR4_PCIE_RD_CACHE_VAL		BIT(24)
#define IMX6Q_GPR4_SDMA_STOP_ACK		BIT(19)
#define IMX6Q_GPR4_CAN2_STOP_ACK		BIT(18)
#define IMX6Q_GPR4_CAN1_STOP_ACK		BIT(17)
#define IMX6Q_GPR4_ENET_STOP_ACK		BIT(16)
#define IMX6Q_GPR4_SOC_VERSION_MASK		(0xff << 8)
#define IMX6Q_GPR4_SOC_VERSION_OFF		0x8
#define IMX6Q_GPR4_VPU_WR_CACHE_SEL		BIT(7)
#define IMX6Q_GPR4_VPU_RD_CACHE_SEL		BIT(6)
#define IMX6Q_GPR4_VPU_P_WR_CACHE_VAL		BIT(3)
#define IMX6Q_GPR4_VPU_P_RD_CACHE_VAL_MASK	BIT(2)
#define IMX6Q_GPR4_IPU_WR_CACHE_CTL		BIT(1)
#define IMX6Q_GPR4_IPU_RD_CACHE_CTL		BIT(0)

#define IMX6Q_GPR5_L2_CLK_STOP			BIT(8)

#define IMX6Q_GPR9_TZASC2_BYP			BIT(1)
#define IMX6Q_GPR9_TZASC1_BYP			BIT(0)

#define IMX6Q_GPR10_LOCK_DBG_EN			BIT(29)
#define IMX6Q_GPR10_LOCK_DBG_CLK_EN		BIT(28)
#define IMX6Q_GPR10_LOCK_SEC_ERR_RESP		BIT(27)
#define IMX6Q_GPR10_LOCK_OCRAM_TZ_ADDR		(0x3f << 21)
#define IMX6Q_GPR10_LOCK_OCRAM_TZ_EN		BIT(20)
#define IMX6Q_GPR10_LOCK_DCIC2_MUX_MASK		(0x3 << 18)
#define IMX6Q_GPR10_LOCK_DCIC1_MUX_MASK		(0x3 << 16)
#define IMX6Q_GPR10_DBG_EN			BIT(13)
#define IMX6Q_GPR10_DBG_CLK_EN			BIT(12)
#define IMX6Q_GPR10_SEC_ERR_RESP_MASK		BIT(11)
#define IMX6Q_GPR10_SEC_ERR_RESP_OKEY		0x0
#define IMX6Q_GPR10_SEC_ERR_RESP_SLVE		BIT(11)
#define IMX6Q_GPR10_OCRAM_TZ_ADDR_MASK		(0x3f << 5)
#define IMX6Q_GPR10_OCRAM_TZ_EN_MASK		BIT(4)
#define IMX6Q_GPR10_DCIC2_MUX_CTL_MASK		(0x3 << 2)
#define IMX6Q_GPR10_DCIC2_MUX_CTL_IPU1_DI0	(0x0 << 2)
#define IMX6Q_GPR10_DCIC2_MUX_CTL_IPU1_DI1	(0x1 << 2)
#define IMX6Q_GPR10_DCIC2_MUX_CTL_IPU2_DI0	(0x2 << 2)
#define IMX6Q_GPR10_DCIC2_MUX_CTL_IPU2_DI1	(0x3 << 2)
#define IMX6Q_GPR10_DCIC1_MUX_CTL_MASK		(0x3 << 0)
#define IMX6Q_GPR10_DCIC1_MUX_CTL_IPU1_DI0	(0x0 << 0)
#define IMX6Q_GPR10_DCIC1_MUX_CTL_IPU1_DI1	(0x1 << 0)
#define IMX6Q_GPR10_DCIC1_MUX_CTL_IPU2_DI0	(0x2 << 0)
#define IMX6Q_GPR10_DCIC1_MUX_CTL_IPU2_DI1	(0x3 << 0)

#define IMX6Q_GPR12_ARMP_IPG_CLK_EN		BIT(27)
#define IMX6Q_GPR12_ARMP_AHB_CLK_EN		BIT(26)
#define IMX6Q_GPR12_ARMP_ATB_CLK_EN		BIT(25)
#define IMX6Q_GPR12_ARMP_APB_CLK_EN		BIT(24)
#define IMX6Q_GPR12_PCIE_CTL_2			BIT(10)

#define IMX6Q_GPR13_SDMA_STOP_REQ		BIT(30)
#define IMX6Q_GPR13_CAN2_STOP_REQ		BIT(29)
#define IMX6Q_GPR13_CAN1_STOP_REQ		BIT(28)
#define IMX6Q_GPR13_ENET_STOP_REQ		BIT(27)
#define IMX6Q_GPR13_SATA_PHY_8_MASK		(0x7 << 24)
#define IMX6Q_GPR13_SATA_PHY_8_0_5_DB		(0x0 << 24)
#define IMX6Q_GPR13_SATA_PHY_8_1_0_DB		(0x1 << 24)
#define IMX6Q_GPR13_SATA_PHY_8_1_5_DB		(0x2 << 24)
#define IMX6Q_GPR13_SATA_PHY_8_2_0_DB		(0x3 << 24)
#define IMX6Q_GPR13_SATA_PHY_8_2_5_DB		(0x4 << 24)
#define IMX6Q_GPR13_SATA_PHY_8_3_0_DB		(0x5 << 24)
#define IMX6Q_GPR13_SATA_PHY_8_3_5_DB		(0x6 << 24)
#define IMX6Q_GPR13_SATA_PHY_8_4_0_DB		(0x7 << 24)
#define IMX6Q_GPR13_SATA_PHY_7_MASK		(0x1f << 19)
#define IMX6Q_GPR13_SATA_PHY_7_SATA1I		(0x10 << 19)
#define IMX6Q_GPR13_SATA_PHY_7_SATA1M		(0x10 << 19)
#define IMX6Q_GPR13_SATA_PHY_7_SATA1X		(0x1a << 19)
#define IMX6Q_GPR13_SATA_PHY_7_SATA2I		(0x12 << 19)
#define IMX6Q_GPR13_SATA_PHY_7_SATA2M		(0x12 << 19)
#define IMX6Q_GPR13_SATA_PHY_7_SATA2X		(0x1a << 19)
#define IMX6Q_GPR13_SATA_PHY_6_MASK		(0x7 << 16)
#define IMX6Q_GPR13_SATA_SPEED_MASK		BIT(15)
#define IMX6Q_GPR13_SATA_SPEED_1P5G		0x0
#define IMX6Q_GPR13_SATA_SPEED_3P0G		BIT(15)
#define IMX6Q_GPR13_SATA_PHY_5			BIT(14)
#define IMX6Q_GPR13_SATA_PHY_4_MASK		(0x7 << 11)
#define IMX6Q_GPR13_SATA_PHY_4_16_16		(0x0 << 11)
#define IMX6Q_GPR13_SATA_PHY_4_14_16		(0x1 << 11)
#define IMX6Q_GPR13_SATA_PHY_4_12_16		(0x2 << 11)
#define IMX6Q_GPR13_SATA_PHY_4_10_16		(0x3 << 11)
#define IMX6Q_GPR13_SATA_PHY_4_9_16		(0x4 << 11)
#define IMX6Q_GPR13_SATA_PHY_4_8_16		(0x5 << 11)
#define IMX6Q_GPR13_SATA_PHY_3_MASK		(0xf << 7)
#define IMX6Q_GPR13_SATA_PHY_3_OFF		0x7
#define IMX6Q_GPR13_SATA_PHY_2_MASK		(0x1f << 2)
#define IMX6Q_GPR13_SATA_PHY_2_OFF		0x2
#define IMX6Q_GPR13_SATA_PHY_1_MASK		(0x3 << 0)
#define IMX6Q_GPR13_SATA_PHY_1_FAST		(0x0 << 0)
#define IMX6Q_GPR13_SATA_PHY_1_MED		(0x1 << 0)
#define IMX6Q_GPR13_SATA_PHY_1_SLOW		(0x2 << 0)

/* For imx6sl iomux gpr register field define */
#define IMX6SL_GPR1_FEC_CLOCK_MUX1_SEL_MASK    (0x3 << 17)
#define IMX6SL_GPR1_FEC_CLOCK_MUX2_SEL_MASK    (0x1 << 14)

#endif /* __LINUX_IMX6Q_IOMUXC_GPR_H */
