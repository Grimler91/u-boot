// SPDX-License-Identifier: GPL-2.0+
/*
 * (C) Copyright 2018 Samsung
 *
 *
 */
#include <common.h>
#include <dm.h>
#include <errno.h>
#include <malloc.h>
#include <dwc3-uboot.h>
#include <asm/armv8/mmu.h>
#include <asm/arch/dwmmc.h>
#include <usb.h>
#include <dwc3-uboot.h>
#include <asm/arch/cpu.h>
#include <asm/arch/power.h>
#include <asm/gpio.h>

DECLARE_GLOBAL_DATA_PTR;

int board_init(void)
{
	/* start Multi Core Timer to get ARM Architected Timers working */
	writel(1 << 8, 0x101c0240);
	return 0;
}

int dram_init(void)
{
	gd->ram_size = PHYS_SDRAM_1_SIZE;
	return 0;
}

int dram_init_banksize(void)
{
	gd->bd->bi_dram[0].start = PHYS_SDRAM_1;
	gd->bd->bi_dram[0].size = PHYS_SDRAM_1_SIZE;
	return 0;
}

unsigned long get_uart_clk(int id)
{
	return 200000000;
}

unsigned long get_mmc_clk(int id)
{
	return 100000000;
}

unsigned long set_mmc_clk(int id, int div)
{
	return 0;
}

unsigned long get_i2c_clk(int id)
{
	return 66666667;
}

#ifdef CONFIG_USB_DWC3
static struct dwc3_device dwc3_device_data = {
	.maximum_speed = USB_SPEED_SUPER,
	.base = 0x15400000,
	.dr_mode = USB_DR_MODE_PERIPHERAL,
	.index = 0,
};

int usb_gadget_handle_interrupts(void)
{
	dwc3_uboot_handle_interrupt(0);
	return 0;
}

int board_usb_init(int index, enum usb_init_type init)
{
	void *base = (void *)0x15500000; /* Exynos5433 DRD PHY */

	/* CMU TOP: set SCLK_USBDRD30_RATIO divider to 1 */
	writel(0x0007770b, (void *)0x10030634);

	/* CMU FSYS: set PHYCLK_USBDRD30_UDRD30_PHYCLOCK and
	   PHYCLK_USBDRD30_UDRD30_PIPE_PCLK muxes to 1 */
	writel(0x00000011, (void *)0x156e0208);

	/* CMU FSYS: enable MUX_SCLK_USBDRD30_USER gate and
	   keep other gates enabled */
	writel(0x01101001, (void *)0x156e0204);

	/* CMU: TOP: set MUX_SCLK_USBDRD30 mux source to pll */
	writel(0x00000101, (void *)0x10030234);

	/* PMU: bypass USB DEV PHY isolation */
	writel(0x00000001, (void *)0x105c0704);

	/* copied from Linux kernel register dump */
	writel(0x0, base + 0x14);
	writel(0x0, base + 0x34);
	writel(0x8000040, base + 0x4);
	writel(0x24d466e4, base + 0x1c);
	writel(0x4, base + 0x30);
	writel(0x24d466e4, base + 0x1c);
	writel(0x3fff81c, base + 0x20);
	writel(0x40, base + 0x8);
	writel(0x0, base + 0x28);
	writel(0xc41805bf, base + 0x10);
	udelay(10);
	writel(0xc41805bd, base + 0x10);
	writel(0x3fff81c, base + 0x20);
	writel(0x0, base + 0x28);
	writel(0xc41b40bf, base + 0x10);
	udelay(10);
	writel(0xc41b40bd, base + 0x10);

	return dwc3_uboot_init(&dwc3_device_data);
}
#endif

int checkboard(void)
{
	const char *board_info;

	board_info = fdt_getprop(gd->fdt_blob, 0, "model", NULL);
	printf("Board: %s\n", board_info ? board_info : "unknown");

	return 0;
}
