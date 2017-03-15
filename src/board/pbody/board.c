/*
 * Copyright (C) 2015 Google Inc.
 * Copyright (C) 2015 Intel Corporation
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but without any warranty; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <gbb_header.h>
#include <pci.h>
#include <pci/pci.h>
#include <libpayload.h>
#include <sysinfo.h>
#include <vboot_api.h>

#include "base/init_funcs.h"
#include "base/list.h"
#include "base/vpd_util.h"
#include "drivers/bus/i2c/designware.h"
#include "drivers/bus/i2c/i2c.h"
#include "drivers/ec/cros/lpc.h"
#include "drivers/flash/flash.h"
#include "drivers/flash/memmapped.h"
#include "drivers/gpio/skylake.h"
#include "drivers/gpio/sysinfo.h"
#include "drivers/hid/hid-vkb.h"
#include "drivers/hid/i2c-hid.h"
#include "drivers/power/pch.h"
#include "drivers/sound/gpio_pdm.h"
#include "drivers/sound/route.h"
#include "drivers/sound/ssm4567.h"
#include "drivers/storage/blockdev.h"
#include "drivers/storage/sdhci.h"
#include "drivers/tpm/lpc.h"
#include "drivers/tpm/tpm.h"
#include "vboot/util/commonparams.h"
#include "vboot/util/flag.h"

/*
 * Clock frequencies for the eMMC and SD ports are defined below. The minimum
 * frequency is the same for both interfaces, the firmware does not run any
 * interface faster than 52 MHz, but defines maximum eMMC frequency as 200 MHz
 * for proper divider settings.
 */
#define EMMC_SD_CLOCK_MIN	400000
#define EMMC_CLOCK_MAX		200000000
#define SD_CLOCK_MAX		52000000

#define KEY_POS_NULL	KEY_POS(0, 0, 0, 0)

/* For US KB */

#define KEY_POS_CTRL    KEY_POS(200, 775, 188, 92)
#define KEY_POS_ENTER   KEY_POS(2516, 1143, 172, 76)
#define KEY_POS_SPACE   KEY_POS(1230, 775, 448, 92)
#define KEY_POS_TAB     KEY_POS(131, 1318, 119, 76)
#define KEY_POS_ESC     KEY_POS(113, 1640, 100, 47)
#define KEY_POS_UP      KEY_POS(2406, 824, 98, 44)
#define KEY_POS_DOWN    KEY_POS(2406, 727, 98, 44)
#define KEY_POS_LEFT    KEY_POS(2203, 727, 80, 44)
#define KEY_POS_RIGHT   KEY_POS(2608, 727, 80, 44)
#define KEY_POS_U       KEY_POS(1458, 1318,80, 76)
#define KEY_POS_D       KEY_POS(768, 1143, 80, 76)
#define KEY_POS_L       KEY_POS(1872, 1143, 80, 76)

/* For Germany KB */

#define DE_KEY_POS_CTRL    KEY_POS(200, 775, 188, 92)
#define DE_KEY_POS_ENTER   KEY_POS(2608, 1230, 80, 163)
#define DE_KEY_POS_SPACE   KEY_POS(1230, 775, 448, 92)
#define DE_KEY_POS_TAB     KEY_POS(131, 1318, 119, 76)
#define DE_KEY_POS_ESC     KEY_POS(113, 1640, 100, 47)
#define DE_KEY_POS_UP      KEY_POS(2406, 824, 98, 44)
#define DE_KEY_POS_DOWN    KEY_POS(2406, 727, 98, 44)
#define DE_KEY_POS_LEFT    KEY_POS(2203, 727, 80, 44)
#define DE_KEY_POS_RIGHT   KEY_POS(2608, 727, 80, 44)
#define DE_KEY_POS_U       KEY_POS(1458, 1318,80, 76)
#define DE_KEY_POS_D       KEY_POS(768, 1143, 80, 76)
#define DE_KEY_POS_L       KEY_POS(1872, 1143, 80, 76)

/* For Japan KB */

#define JP_KEY_POS_CTRL    KEY_POS(178, 775, 166, 92)
#define JP_KEY_POS_ENTER   KEY_POS(2608, 1230, 80, 163)
#define JP_KEY_POS_SPACE   KEY_POS(1228, 775, 319, 92)
#define JP_KEY_POS_TAB     KEY_POS(131, 1318, 119, 76)
#define JP_KEY_POS_ESC     KEY_POS(113, 1640, 100, 47)
#define JP_KEY_POS_UP      KEY_POS(2406, 824, 98, 44)
#define JP_KEY_POS_DOWN    KEY_POS(2406, 727, 98, 44)
#define JP_KEY_POS_LEFT    KEY_POS(2203, 727, 80, 44)
#define JP_KEY_POS_RIGHT   KEY_POS(2608, 727, 80, 44)
#define JP_KEY_POS_U       KEY_POS(1458, 1318,80, 76)
#define JP_KEY_POS_D       KEY_POS(768, 1143, 80, 76)
#define JP_KEY_POS_L       KEY_POS(1872, 1143, 80, 76)

struct key_array_t board_key_list[] = {
	{{KEY_POS_U,     KEY_POS_CTRL}, 2, 0x15          },	/* ctrl-u */
	{{KEY_POS_D,     KEY_POS_CTRL}, 2, 0x04          },	/* ctrl-d */
	{{KEY_POS_L,     KEY_POS_CTRL}, 2, 0x0c          },	/* ctrl-l */
	{{KEY_POS_ENTER, KEY_POS_NULL}, 1, '\r'          },	/* enter */
	{{KEY_POS_SPACE, KEY_POS_NULL}, 1, ' '           },	/* space */
	{{KEY_POS_TAB,   KEY_POS_NULL}, 1, '\t'          },	/* tab */
	{{KEY_POS_ESC,   KEY_POS_NULL}, 1, 0x1b          },	/* esc */
	{{KEY_POS_UP,    KEY_POS_NULL}, 1, VB_KEY_UP        },	/* up */
	{{KEY_POS_DOWN,  KEY_POS_NULL}, 1, VB_KEY_DOWN      },	/* down */
	{{KEY_POS_LEFT,  KEY_POS_NULL}, 1, VB_KEY_LEFT      },	/* left */
	{{KEY_POS_RIGHT, KEY_POS_NULL}, 1, VB_KEY_RIGHT     },	/* right */
	{{KEY_POS_ENTER, KEY_POS_CTRL}, 2, VB_KEY_CTRL_ENTER},	/* ctrl-enter */
	{}
};

struct key_array_t de_board_key_list[] = {
	{{DE_KEY_POS_U,     DE_KEY_POS_CTRL}, 2, 0x15      },	/* ctrl-u */
	{{DE_KEY_POS_D,     DE_KEY_POS_CTRL}, 2, 0x04      },	/* ctrl-d */
	{{DE_KEY_POS_L,     DE_KEY_POS_CTRL}, 2, 0x0c      },	/* ctrl-l */
	{{DE_KEY_POS_ENTER, KEY_POS_NULL}, 1, '\r'         },	/* enter */
	{{DE_KEY_POS_SPACE, KEY_POS_NULL}, 1, ' '          },	/* space */
	{{DE_KEY_POS_TAB,   KEY_POS_NULL}, 1, '\t'         },	/* tab */
	{{DE_KEY_POS_ESC,   KEY_POS_NULL}, 1, 0x1b         },	/* esc */
	{{DE_KEY_POS_UP,    KEY_POS_NULL}, 1, VB_KEY_UP    },	/* up */
	{{DE_KEY_POS_DOWN,  KEY_POS_NULL}, 1, VB_KEY_DOWN  },	/* down */
	{{DE_KEY_POS_LEFT,  KEY_POS_NULL}, 1, VB_KEY_LEFT  },	/* left */
	{{DE_KEY_POS_RIGHT, KEY_POS_NULL}, 1, VB_KEY_RIGHT },	/* right */
	{{DE_KEY_POS_ENTER, DE_KEY_POS_CTRL}, 2, VB_KEY_CTRL_ENTER},
		/* ctrl-enter */
	{}
};


struct key_array_t jp_board_key_list[] = {
	{{JP_KEY_POS_U,     JP_KEY_POS_CTRL}, 2, 0x15      },	/* ctrl-u */
	{{JP_KEY_POS_D,     JP_KEY_POS_CTRL}, 2, 0x04      },	/* ctrl-d */
	{{JP_KEY_POS_L,     JP_KEY_POS_CTRL}, 2, 0x0c      },	/* ctrl-l */
	{{JP_KEY_POS_ENTER, KEY_POS_NULL}, 1, '\r'         },	/* enter */
	{{JP_KEY_POS_SPACE, KEY_POS_NULL}, 1, ' '          },	/* space */
	{{JP_KEY_POS_TAB,   KEY_POS_NULL}, 1, '\t'         },	/* tab */
	{{JP_KEY_POS_ESC,   KEY_POS_NULL}, 1, 0x1b         },	/* esc */
	{{JP_KEY_POS_UP,    KEY_POS_NULL}, 1, VB_KEY_UP    },	/* up */
	{{JP_KEY_POS_DOWN,  KEY_POS_NULL}, 1, VB_KEY_DOWN  },	/* down */
	{{JP_KEY_POS_LEFT,  KEY_POS_NULL}, 1, VB_KEY_LEFT  },	/* left */
	{{JP_KEY_POS_RIGHT, KEY_POS_NULL}, 1, VB_KEY_RIGHT },	/* right */
	{{JP_KEY_POS_ENTER, JP_KEY_POS_CTRL}, 2, VB_KEY_CTRL_ENTER},
		/* ctrl-enter */
	{}
};
static i2chiddev_t *i2c_dev;
static GpioCfg *int_cfg;

static void board_get_keyboard_layout(void)
{
	const char regioncode_key[] = "region";
	char country_code[8], *cc;

	if (!vpd_gets(regioncode_key, country_code, sizeof(country_code))) {
		printf("No region code found in VPD\n");
		board_virtual_keyboard_layout(&board_key_list[0]);
		return;
	}
	/* Add only the two letter Alpha-2 country code, remove the variant
	 * in region code */
	country_code[2] = 0;
	cc = strdup(country_code);
	if(!strncasecmp(cc,"DE",2))
		board_virtual_keyboard_layout(&de_board_key_list[0]);
	else if(!strncasecmp(cc,"JP",2))
		board_virtual_keyboard_layout(&jp_board_key_list[0]);
	else
		board_virtual_keyboard_layout(&board_key_list[0]);
}

/*
 * Workaround for issue where silego is unable to see EC reset to clear the
 * EC_IN_RW state when attempting to enter recovery via servo.  This allows FAFT
 * to transition the system to developer mode.
 */
static int ec_in_rw_workaround_get_value(GpioOps *me)
{
	GoogleBinaryBlockHeader *gbb = cparams.gbb_data;

	if (gbb->flags & GBB_FLAG_FAFT_KEY_OVERIDE) {
		/* Override is enabled, return 0 for FAFT. */
		printf("FAFT override enabled, returning 0 for ECINRW flag\n");
		return 0;
	}

	/* Override is not enabled, lookup the real GPIO state. */
	GpioOps *ecinrw = sysinfo_lookup_gpio("EC in RW", 1,
			    new_skylake_gpio_input_from_coreboot);
	return ecinrw->get(ecinrw);
}

GpioOps *ec_in_rw_workaround_gpio(void)
{
	GpioOps *ops = xzalloc(sizeof(*ops));
	ops->get = &ec_in_rw_workaround_get_value;
	return ops;
}

static int board_havekey(void)
{
	if (i2c_dev != NULL)
		return vkb_havekey(i2c_dev) != 0;

	return 0;
}

static int board_getchar(void)
{
	return vkb_getchar();
}

static struct console_input_driver board_input_driver =
{
	NULL,
	&board_havekey,
	&board_getchar
};

static int int_status(void)
{
	return !int_cfg->ops.get(&int_cfg->ops);
}

static int hw_reset(void)
{
	return 0;
}

static int board_setup(void)
{
	sysinfo_install_flags(new_skylake_gpio_input_from_coreboot);
	flag_replace(FLAG_ECINRW, ec_in_rw_workaround_gpio());

	/* MEC1322 Chrome EC */
	CrosEcLpcBus *cros_ec_lpc_bus =
		new_cros_ec_lpc_bus(CROS_EC_LPC_BUS_MEC);
	cros_ec_set_bus(&cros_ec_lpc_bus->ops);

	/* 16MB SPI Flash */
	flash_set_ops(&new_mem_mapped_flash(0xff000000, 0x1000000)->ops);

	/* SPI TPM memory mapped to act like LPC TPM */
	tpm_set_ops(&new_lpc_tpm((void *)(uintptr_t)0xfed40000)->ops);

	/* PCH Power */
	power_set_ops(&skylake_power_ops);

	/* eMMC */
	SdhciHost *emmc = new_pci_sdhci_host(PCI_DEV(0, 0x1e, 4),
			SDHCI_PLATFORM_NO_EMMC_HS200,
			EMMC_SD_CLOCK_MIN, EMMC_CLOCK_MAX);
	list_insert_after(&emmc->mmc_ctrlr.ctrlr.list_node,
			&fixed_block_dev_controllers);

	/* SD Card (if present) */
	pcidev_t sd_pci_dev = PCI_DEV(0, 0x1e, 6);
	uint16_t sd_vendor_id = pci_read_config32(sd_pci_dev, REG_VENDOR_ID);
	if (sd_vendor_id == PCI_VENDOR_ID_INTEL) {
		SdhciHost *sd = new_pci_sdhci_host(sd_pci_dev, 1,
					   EMMC_SD_CLOCK_MIN, SD_CLOCK_MAX);
		list_insert_after(&sd->mmc_ctrlr.ctrlr.list_node,
				  &removable_block_dev_controllers);
	}
	/* The interrupt is level sensitive and active low */
	int_cfg = new_skylake_gpio_input(GPP_B3);

	/* Virtual Keyboard is on I2C1 */
	DesignwareI2c *i2c1 =
		new_pci_designware_i2c(PCI_DEV(0, 0x15, 1), 400000);

	i2c_dev = new_i2c_hid(&i2c1->ops, 0x20, 0x20, &int_status, &hw_reset);

	if (i2c_dev) {
		if (!configure_virtual_keyboard(i2c_dev)) {
			console_add_input_driver(&board_input_driver);
			printf("virtual keyboard installed\n");
			board_get_keyboard_layout();
		} else {
			i2c_dev = NULL;
			printf("virtual keyboard install failed\n");
		}
	}

	/* Speaker Amp Codec is on I2C4 */
	DesignwareI2c *i2c4 =
		new_pci_designware_i2c(PCI_DEV(0, 0x19, 2), 400000);
	ssm4567Codec *speaker_amp_left =
		new_ssm4567_codec(&i2c4->ops, 0x34, SSM4567_MODE_PDM);

	/* Activate buffer to disconnect I2S from PCH and allow GPIO */
	GpioCfg *i2s2_buffer = new_skylake_gpio_output(GPP_D22, 0);
	gpio_set(&i2s2_buffer->ops, 1);

	/* Use GPIO to bit-bang PDM to the codec */
	GpioCfg *i2s2_sclk = new_skylake_gpio_output(GPP_F0, 0);
	GpioCfg *i2s2_txd  = new_skylake_gpio_output(GPP_F2, 0);
	GpioPdm *pdm = new_gpio_pdm(&i2s2_sclk->ops,	/* PDM Clock GPIO */
				    &i2s2_txd->ops,	/* PDM Data GPIO */
				    85000,		/* Clock Start */
				    16000,		/* Sample Rate */
				    2,			/* Channels */
				    1000);		/* Volume */

	/* Connect the Codec to the PDM source */
	SoundRoute *sound = new_sound_route(&pdm->ops);
	list_insert_after(&speaker_amp_left->component.list_node,
			  &sound->components);
	sound_set_ops(&sound->ops);

	return 0;
}

INIT_FUNC(board_setup);
