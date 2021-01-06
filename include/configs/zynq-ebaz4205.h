/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * (C) Copyright 2020 embed-me
 *
 * Configuration settings for the EBAZ4205 Board
 * See zynq-common.h for Zynq common configs
 */

#ifndef __CONFIG_ZYNQ_EBAZ4205_H
#define __CONFIG_ZYNQ_EBAZ4205_H


/* Configuration for zynq-common.h */
//#define CONFIG_ZYNQ_GEM
//#define CONFIG_CMD_DHCP
#include <configs/zynq-common.h>


/* We use s2-Button to avoid handoff */
#undef CONFIG_BOOTDELAY
#define CONFIG_BOOTDELAY 0


/* We do not comply with mainline yet */
#undef CONFIG_BOOTCOMMAND
#define CONFIG_BOOTCOMMAND "if run s2_button_pressed_test; then " \
            "run $modeboot || run distro_bootcmd;"\
    "else " \
            "echo Abort boot...;" \
    "fi;" \


/* We do not comply with FIT images yet */
#undef CONFIG_EXTRA_ENV_SETTINGS
#define CONFIG_EXTRA_ENV_SETTINGS	\
	"kernel_image=uImage\0"	\
	"kernel_load_address=0x2080000\0" \
        "kernel_size=0x500000\0" \
        "mmc_loadkernel=load mmc 0 ${kernel_load_address} ${kernel_image}\0" \
	"ramdisk_image=ebaz4205-image-minimal-ebaz4205-zynq7.cpio.gz.u-boot\0"	\
	"ramdisk_load_address=0x4000000\0"	\
        "ramdisk_size=0x5E0000\0" \
        "mmc_loadramdisk=load mmc 0 ${ramdisk_load_address} ${ramdisk_image}\0" \
	"devicetree_image=ebaz4205-zynq7.dtb\0"	\
	"devicetree_load_address=0x2000000\0"	\
        "devicetree_size=0x20000\0"             \
        "mmc_loaddtb=load mmc 0 ${devicetree_load_address} ${devicetree_image}\0" \
	"bitstream_image=download-ebaz4205-zynq7.bit\0"	\
	"bitstream_load_address=0x100000\0"	\
        "mmc_loadfpga=load mmc 0 ${bitstream_load_address} ${bitstream_image}\0" \
        "fpga_config=fpga load 0 ${bitstream_load_address} ${filesize}\0" \
	"loadbootenv_addr=0x2000000\0" \
	"bootenv=uEnv.txt\0" \
	"loadbootenv=load mmc 0 ${loadbootenv_addr} ${bootenv}\0" \
	"importbootenv=echo Importing environment from SD ...; " \
		"env import -t ${loadbootenv_addr} $filesize\0" \
	"sd_uEnvtxt_existence_test=test -e mmc 0 /uEnv.txt\0" \
        "s2_button_pressed_test=gpio input 20;\0" \
        "leds_bootstate_0=gpio clear 54; gpio clear 55;\0" \
        "leds_bootstate_1=gpio clear 54; gpio set 55;\0" \
        "leds_bootstate_2=gpio set 54; gpio clear 55;\0" \
        "leds_bootstate_3=gpio set 54; gpio set 55;\0" \
	"preboot=if test $modeboot = sdboot && env run sd_uEnvtxt_existence_test; " \
			"then if env run loadbootenv; " \
				"then env run importbootenv; " \
			"fi; " \
		"fi; \0" \
	"norboot=echo Copying Linux from NOR flash to RAM... && " \
		"cp.b 0xE2100000 ${kernel_load_address} ${kernel_size} && " \
		"cp.b 0xE2600000 ${devicetree_load_address} ${devicetree_size} && " \
		"echo Copying ramdisk... && " \
		"cp.b 0xE2620000 ${ramdisk_load_address} ${ramdisk_size} && " \
		"bootm ${kernel_load_address} ${ramdisk_load_address} ${devicetree_load_address}\0" \
	"qspiboot=echo Copying Linux from QSPI flash to RAM... && " \
		"sf probe 0 0 0 && " \
		"sf read ${kernel_load_address} 0x100000 ${kernel_size} && " \
		"sf read ${devicetree_load_address} 0x600000 ${devicetree_size} && " \
		"echo Copying ramdisk... && " \
		"sf read ${ramdisk_load_address} 0x620000 ${ramdisk_size} && " \
		"bootm ${kernel_load_address} ${ramdisk_load_address} ${devicetree_load_address}\0" \
	"uenvboot=" \
		"if run loadbootenv; then " \
			"echo Loaded environment from ${bootenv}; " \
			"run importbootenv; " \
		"fi; " \
		"if test -n $uenvcmd; then " \
			"echo Running uenvcmd ...; " \
			"run uenvcmd; " \
		"fi\0" \
	"sdboot=if mmcinfo; then " \
                        "run leds_bootstate_0;" \
			"run uenvboot;" \
			"echo Copying Linux from SD to RAM...;" \
			"run mmc_loadkernel && run mmc_loaddtb && run mmc_loadramdisk && run leds_bootstate_2;" \
                        "echo Handoff to Linux kernel...;" \
			"bootm ${kernel_load_address} ${ramdisk_load_address} ${devicetree_load_address}; " \
		"fi\0" \
	"nandboot=echo Copying Linux from NAND flash to RAM... && " \
		"nand read ${kernel_load_address} 0x100000 ${kernel_size} && " \
		"nand read ${devicetree_load_address} 0x600000 ${devicetree_size} && " \
		"echo Copying ramdisk... && " \
		"nand read ${ramdisk_load_address} 0x620000 ${ramdisk_size} && " \
		"bootm ${kernel_load_address} ${ramdisk_load_address} ${devicetree_load_address}\0" \
	"jtagboot=echo TFTPing Linux to RAM... && " \
		"tftpboot ${kernel_load_address} ${kernel_image} && " \
		"tftpboot ${devicetree_load_address} ${devicetree_image} && " \
		"tftpboot ${ramdisk_load_address} ${ramdisk_image} && " \
		"bootm ${kernel_load_address} ${ramdisk_load_address} ${devicetree_load_address}\0" \
		BOOTENV

#endif /* __CONFIG_ZYNQ_EBAZ4205_H */
