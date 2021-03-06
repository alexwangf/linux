/*****************************************************************************
*
* Copyright (c) 2014, Advanced Micro Devices, Inc.
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * Neither the name of Advanced Micro Devices, Inc. nor the names of
*       its contributors may be used to endorse or promote products derived
*       from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL ADVANCED MICRO DEVICES, INC. BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*
***************************************************************************/
#include <linux/init.h>
#include <linux/module.h>
#include <linux/pci.h>
#include <linux/ioport.h>
#include <linux/platform_device.h>
#include <linux/uaccess.h>
#include <linux/io.h>
#include <linux/delay.h>
#include <linux/amd_imc.h>

static int imc_enabled;
static u16 imc_port_addr;
static u8 msg_reg_base_hi;
static u8 msg_reg_base_lo;
static u16 msg_reg_base;

static struct pci_dev *amd_imc_pci;
static struct platform_device *amd_imc_platform_device;

static DEFINE_PCI_DEVICE_TABLE(amd_lpc_pci_tbl) = {
	{PCI_VENDOR_ID_AMD, PCI_DEVICE_ID_AMD_LPC_BRIDGE, PCI_ANY_ID,
	 PCI_ANY_ID,},
	{}
};

void amd_imc_enter_scratch_ram(void)
{
	u8 byte;

	if (!imc_enabled)
		return;

	/* Instruct IMC to enter scratch RAM */
	outb(AMD_MSG_REG0, msg_reg_base + AMD_MSG_INDEX_REG_OFFSET);
	outb(0, msg_reg_base + AMD_MSG_DATA_REG_OFFSET);

	outb(AMD_MSG_REG1, msg_reg_base + AMD_MSG_INDEX_REG_OFFSET);
	outb(AMD_IMC_ENTER_SCRATCH_RAM, msg_reg_base + AMD_MSG_DATA_REG_OFFSET);

	outb(AMD_MSG_SYS_TO_IMC, msg_reg_base + AMD_MSG_INDEX_REG_OFFSET);
	outb(AMD_IMC_ROM_OWNERSHIP_SEM, msg_reg_base +
	     AMD_MSG_DATA_REG_OFFSET);

	/* As per the spec, the firmware may take up to 50ms */
	msleep(50);

	/* read message registger 0 to confirm function completion */
	outb(AMD_MSG_REG0, msg_reg_base + AMD_MSG_INDEX_REG_OFFSET);
	byte = inb(msg_reg_base + AMD_MSG_DATA_REG_OFFSET);

	if (byte == AMD_IMC_FUNC_NOT_SUPP)
		pr_info("amd_imc: %s not supported\n", __func__);
	else if (byte == AMD_IMC_FUNC_COMPLETED)
		pr_info("amd_imc: %s completed\n", __func__);
}
EXPORT_SYMBOL_GPL(amd_imc_enter_scratch_ram);

void amd_imc_exit_scratch_ram(void)
{
	u8 byte;

	if (!imc_enabled)
		return;

	/* Instruct IMC to exit scratch RAM */
	outb(AMD_MSG_REG0, msg_reg_base + AMD_MSG_INDEX_REG_OFFSET);
	outb(0, msg_reg_base + AMD_MSG_DATA_REG_OFFSET);

	outb(AMD_MSG_REG1, msg_reg_base + AMD_MSG_INDEX_REG_OFFSET);
	outb(AMD_IMC_ENTER_SCRATCH_RAM, msg_reg_base + AMD_MSG_DATA_REG_OFFSET);

	outb(AMD_MSG_SYS_TO_IMC, msg_reg_base + AMD_MSG_INDEX_REG_OFFSET);
	outb(AMD_IMC_ROM_OWNERSHIP_SEM, msg_reg_base +
	     AMD_MSG_DATA_REG_OFFSET);

	/* As per the spec, the firmware may take up to 50ms */
	msleep(50);

	/* read message registger 0 to confirm function completion */
	outb(AMD_MSG_REG0, msg_reg_base + AMD_MSG_INDEX_REG_OFFSET);
	byte = inb(msg_reg_base + AMD_MSG_DATA_REG_OFFSET);

	if (byte == AMD_IMC_FUNC_NOT_SUPP)
		pr_info("amd_imc: %s not supported\n", __func__);
	else if (byte == AMD_IMC_FUNC_COMPLETED)
		pr_info("amd_imc: %s completed\n", __func__);
}
EXPORT_SYMBOL_GPL(amd_imc_exit_scratch_ram);

/*
* The PCI Device ID table below is used to identify the platform
* the driver is supposed to work for. Since this is a platform
* driver, we need a way for us to be able to find the correct
* platform when the driver gets loaded, otherwise we should
* bail out.
*/
static DEFINE_PCI_DEVICE_TABLE(amd_imc_pci_tbl) = {
	{ PCI_VENDOR_ID_AMD, 0x790B, PCI_ANY_ID,
	  PCI_ANY_ID, },
	{ 0, },
};

static int amd_imc_init(struct platform_device *pdev)
{
	struct pci_dev *dev = NULL;
	static u32 imc_strap_status_phys;
	void __iomem *imcstrapstatus;
	u32 val;

	/* Match the PCI device */
	for_each_pci_dev(dev) {
		if (pci_match_id(amd_imc_pci_tbl, dev) != NULL) {
			amd_imc_pci = dev;
			break;
		}
	}

	if (!amd_imc_pci)
		return -ENODEV;


	/* ACPI MMIO Base Address */
	val = AMD_GPIO_ACPIMMIO_BASE;

	/* IMCStrapStatus is located at ACPI MMIO Base Address + 0xE80 */
	if (!request_mem_region_exclusive(val + AMD_IMC_STRAP_STATUS_OFFSET,
	    AMD_IMC_STRAP_STATUS_SIZE, "IMC Strap Status")) {
		pr_err("amd_imc: MMIO address 0x%04x already in use\n",
			val + AMD_IMC_STRAP_STATUS_OFFSET);
		goto exit;
	}

	imc_strap_status_phys = val + AMD_IMC_STRAP_STATUS_OFFSET;

	imcstrapstatus = ioremap(imc_strap_status_phys,
				 AMD_IMC_STRAP_STATUS_SIZE);
	if (!imcstrapstatus) {
		pr_err("amd_imc: failed to get IMC Strap Status address\n");
		goto unreg_imc_region;
	}

	/* Check if IMC is enabled */
	val = ioread32(imcstrapstatus);
	if ((val & AMD_IMC_ENABLED) == AMD_IMC_ENABLED) {
		struct pci_dev *pdev = NULL;

		pr_info("amd_imc: IMC is enabled\n");
		imc_enabled = 1;

		/*
		 * In case IMC is enabled, we need to find the IMC port address
		 * which will be used to send messages to the IMC. The IMC port
		 * address is stored in bits 1:15 of PCI device 20, function 3,
		 * offset 0xA4. PCI device 20, function 3 is actually the LPC
		 * ISA bridge.
		 */
		for_each_pci_dev(pdev) {
			if (pci_match_id(amd_lpc_pci_tbl, pdev) != NULL)
				break;
		}

		/* Match found. Get the IMC port address */
		if (pdev) {
			pci_read_config_word(pdev, AMD_PCI_IMC_PORT_ADDR_REG,
					     &imc_port_addr);

			/* The actual IMC port address has bit 0 masked out */
			imc_port_addr &= ~AMD_IMC_PORT_ACTIVE;
		}

		/* Put device into configuration state */
		outb(AMD_DEVICE_ENTER_CONFIG_STATE, imc_port_addr +
		     AMD_IMC_INDEX_REG_OFFSET);

		/* Select logical device number 9 */
		outb(AMD_SET_LOGICAL_DEVICE, imc_port_addr +
		     AMD_IMC_INDEX_REG_OFFSET);
		outb(AMD_SET_DEVICE_9, imc_port_addr +
		     AMD_IMC_DATA_REG_OFFSET);

		/* read high byte of message register base address */
		outb(AMD_MSG_REG_HIGH, imc_port_addr +
		     AMD_IMC_INDEX_REG_OFFSET);
		msg_reg_base_hi = inb(imc_port_addr + AMD_IMC_DATA_REG_OFFSET);

		/* read low byte of message register base address */
		outb(AMD_MSG_REG_LOW, imc_port_addr +
		     AMD_IMC_INDEX_REG_OFFSET);
		msg_reg_base_lo = inb(imc_port_addr + AMD_IMC_DATA_REG_OFFSET);

		msg_reg_base = msg_reg_base_lo | (msg_reg_base_hi << 8);

		/* Get device out of configuration state */
		outb(AMD_DEVICE_EXIT_CONFIG_STATE, imc_port_addr +
		     AMD_IMC_INDEX_REG_OFFSET);
	} else {
		pr_info("amd_imc: IMC is disabled\n");
		imc_enabled = 0;
	}

	/* Release the region occupied by IMC Strap Status register */
	iounmap(imcstrapstatus);
	release_mem_region(imc_strap_status_phys, AMD_IMC_STRAP_STATUS_SIZE);

	return 0;

unreg_imc_region:
	release_mem_region(imc_strap_status_phys, AMD_IMC_STRAP_STATUS_SIZE);
exit:
	return -ENODEV;
}

static struct platform_driver amd_imc_driver = {
	.probe		= amd_imc_init,
	.driver		= {
		.owner	= THIS_MODULE,
		.name	= IMC_MODULE_NAME,
	},
};

static int __init amd_imc_init_module(void)
{
	int err;

	pr_info("AMD IMC Driver v%s\n", IMC_VERSION);

	err = platform_driver_register(&amd_imc_driver);
	if (err)
		return err;

	amd_imc_platform_device = platform_device_register_simple(
					IMC_MODULE_NAME, -1, NULL, 0);
	if (IS_ERR(amd_imc_platform_device)) {
		err = PTR_ERR(amd_imc_platform_device);
		goto unreg_platform_driver;
	}

	return 0;

unreg_platform_driver:
	platform_driver_unregister(&amd_imc_driver);
	return err;
}

static void __exit amd_imc_cleanup_module(void)
{
	platform_device_unregister(amd_imc_platform_device);
	platform_driver_unregister(&amd_imc_driver);
	pr_info("AMD IMC Module Unloaded\n");
}

module_init(amd_imc_init_module);
module_exit(amd_imc_cleanup_module);

MODULE_AUTHOR("Arindam Nath <arindam.nath@amd.com>");
MODULE_DESCRIPTION("AMD IMC driver");
MODULE_LICENSE("Dual BSD/GPL");
