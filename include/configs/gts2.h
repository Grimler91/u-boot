// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (C) 2018 Samsung Electronics
 * Marek Szyprowski <m.szyprowski@samsung.com>
 *
 * Configuation settings for the Exynos5433 TM2 board.
 */

#ifndef __SAMSUNG_GTS2_H
#define __SAMSUNG_GTS2_H

#include <configs/exynos5-common.h>

/* Timer input clock frequency */
#define COUNTER_FREQUENCY		24000000

#define CONFIG_SYS_SDRAM_BASE		0x20000000

#define SDRAM_BANK_SIZE			(256UL << 20UL)	/* 256 MB */

/* Initial environment variables */
#define CONFIG_BOOTCOMMAND		"run modedetect"
#undef CONFIG_EXTRA_ENV_SETTINGS
#define CONFIG_EXTRA_ENV_SETTINGS \
	"dfu_alt_info=kernel part 0 9 offset 0x400\0" \
	"rootfs part 0 18\0" \
	"system-data part 0 19\0" \
	"user part 0 21\0" \
	"modedetect=if gpio input gpa21 || itest.l *0x105c080c == 0x12345671\0" \
	"then run download\0" \
	"else run bootkernel\0" \
	"fi\0" \
	"reset\0" \
	"fdt_high=0xffffffffffffffff\0" \
	"bootargs=console=ttySAC1,115200 earlycon=exynos4210,0x14C20000\0" \
	"bootkernel=echo Booting kernel\0" \
	"run boarddetect\0" \
	"run loadkernel\0" \
	"bootm 0x30080000#$board\0" \
	"boarddetect=if itest.l *0x138000b4 == 0x0063f9ff\0" \
	"then setenv board tm2e\0" \
	"elif itest.l *0x138000b4 == 0x0059f9ff\0" \
	"then setenv board tm2\0" \
	"else setenv board unknown\0" \
	"fi\0" \
	"echo Detected $board board\0" \
	"loadkernel=part start mmc 0 9 kernel_sect\0" \
	"part size mmc 0 9 kernel_size\0" \
	"mmc read 0x30000000 $kernel_sect $kernel_size\0" \
	"muicsetusb=i2c dev 0\0" \
	"i2c mw 25 c 9\0" \
	"i2c mw 25 d 3b\0" \
	"i2c mw 25 e 05\0" \
	"i2c mw 25 16 10\0" \
	"displayimg=unzip 200d0000 67000000\0" \
	"mw.l 138000b4 0059f9ff\0" \
	"mw.l 138001a0 67e10000\0" \
	"mw.l 13800200 00001680\0" \
	"mw.l 13801410 1\0" \
	"mw.l 13802040 e0000018\0" \
	"sleep 1\0" \
	"mw.l 13802040 e0000008\0" \
	"download=echo Thor mode enabled\0" \
	"run muicsetusb\0" \
	"run displayimg\0" \
	"mw.l 0x105c080c 0\0" \
	"thor 0 mmc 0\0"

#endif /* __SAMSUNG_GTS2_H */
