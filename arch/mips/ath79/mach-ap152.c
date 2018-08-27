
/*
 * Atheros AP152 reference board support
 *
 * Copyright (c) 2014 The Linux Foundation. All rights reserved.
 * Copyright (c) 2012 Gabor Juhos <juhosg@openwrt.org>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 */

#include <linux/platform_device.h>
#include <linux/ath9k_platform.h>
#include <linux/ar8216_platform.h>

#include <asm/mach-ath79/ar71xx_regs.h>

#include "common.h"
#include "dev-m25p80.h"
#include "machtypes.h"
#include "pci.h"
#include "dev-eth.h"
#include "dev-gpio-buttons.h"
#include "dev-leds-gpio.h"
#include "dev-spi.h"
#include "dev-usb.h"
#include "dev-wmac.h"
#if OK_PATCH
#include "dev-ap9x-pci.h"
#endif

#define AP152_GPIO_LED_WLAN             12
#define AP152_GPIO_LED_WPS              13
#define AP152_GPIO_LED_STATUS           13

#define AP152_GPIO_LED_WAN              4
#define AP152_GPIO_LED_LAN1             16
#define AP152_GPIO_LED_LAN2             15
#define AP152_GPIO_LED_LAN3             14
#define AP152_GPIO_LED_LAN4             11

#define AP152_GPIO_LED_USB0		7
#define AP152_GPIO_LED_USB1		8

#define AP152_GPIO_BTN_RESET            2
#define AP152_GPIO_BTN_WPS              1
#define AP152_KEYS_POLL_INTERVAL        20     /* msecs */
#define AP152_KEYS_DEBOUNCE_INTERVAL    (3 * AP152_KEYS_POLL_INTERVAL)

#define AP152_MAC0_OFFSET               0
#define AP152_MAC1_OFFSET               6
#define AP152_WMAC_CALDATA_OFFSET       0x1000
#if OK_PATCH
#define AP152_PCI_CALDATA_OFFSET        0x5000
#define BOARDID_OFFSET			        0x20
#endif

#define AP152_GPIO_MDC			3
#define AP152_GPIO_MDIO			4

static struct gpio_led ap152_leds_gpio[] __initdata = {
	{
		.name		= "ap152:green:usb0",
		.gpio		= AP152_GPIO_LED_USB0,
		.active_low	= 1,
	},
	{
		.name		= "ap152:green:usb1",
		.gpio		= AP152_GPIO_LED_USB1,
		.active_low	= 1,
	},
};

static struct gpio_keys_button ap152_gpio_keys[] __initdata = {
        {
                .desc           = "WPS button",
                .type           = EV_KEY,
                .code           = KEY_WPS_BUTTON,
                .debounce_interval = AP152_KEYS_DEBOUNCE_INTERVAL,
                .gpio           = AP152_GPIO_BTN_WPS,
                .active_low     = 1,
        },
        {
                .desc           = "Reset button",
                .type           = EV_KEY,
                .code           = KEY_RESTART,
                .debounce_interval = AP152_KEYS_DEBOUNCE_INTERVAL,
                .gpio           = AP152_GPIO_BTN_RESET,
                .active_low     = 1,
        },
};


#if OK_PATCH
/* A820 */
#define AP152_QTSA820_GPIO_LED_GREEN	1

#define AP152_QTSA820_GPIO_BTN_RESET    2

static struct gpio_led ap152_qtsa820_leds_gpio[] __initdata = {
	{
		.name		= "ap152:green:status",
		.gpio		= AP152_QTSA820_GPIO_LED_GREEN,
		.active_low	= 1,
	},
};

static struct gpio_keys_button ap152_qtsa820_gpio_keys[] __initdata = {
        {
                .desc           = "Reset button",
                .type           = EV_KEY,
                .code           = KEY_RESTART,
                .debounce_interval = AP152_KEYS_DEBOUNCE_INTERVAL,
                .gpio           = AP152_QTSA820_GPIO_BTN_RESET,
                .active_low     = 1,
        },
};

/* WL8200-T2 */
#define AP152_WL8200T2_GPIO_LED_RED		6
#define AP152_WL8200T2_GPIO_LED_GREEN	7
#define AP152_WL8200T2_GPIO_LED_BLUE	8

#define AP152_WL8200T2_GPIO_BTN_RESET   2

static struct gpio_led ap152_wl8200t2_leds_gpio[] __initdata = {
	{
		.name		= "ap152:red:status",
		.gpio		= AP152_WL8200T2_GPIO_LED_RED,
		.active_low	= 1,
	},
	{
		.name		= "ap152:green:status",
		.gpio		= AP152_WL8200T2_GPIO_LED_GREEN,
		.active_low	= 1,
	},
	{
		.name		= "ap152:blue:status",
		.gpio		= AP152_WL8200T2_GPIO_LED_BLUE,
		.active_low	= 1,
	},
};

static struct gpio_keys_button ap152_wl8200t2_gpio_keys[] __initdata = {
        {
                .desc           = "Reset button",
                .type           = EV_KEY,
                .code           = KEY_RESTART,
                .debounce_interval = AP152_KEYS_DEBOUNCE_INTERVAL,
                .gpio           = AP152_WL8200T2_GPIO_BTN_RESET,
                .active_low     = 1,
        },
};

/* WL8200R2 */

#define AP152_WL8200R2_GPIO_LED_RED		6
#define AP152_WL8200R2_GPIO_LED_GREEN	7
#define AP152_WL8200R2_GPIO_LED_BLUE	8

#define AP152_WL8200R2_GPIO_BTN_RESET   2


static struct gpio_led ap152_wl8200r2_leds_gpio[] __initdata = {
	{
		.name		= "ap152:red:status",
		.gpio		= AP152_WL8200R2_GPIO_LED_RED,
		.active_low	= 1,
	},
	{
		.name		= "ap152:green:status",
		.gpio		= AP152_WL8200R2_GPIO_LED_GREEN,
		.active_low	= 1,
	},
	{
		.name		= "ap152:blue:status",
		.gpio		= AP152_WL8200R2_GPIO_LED_BLUE,
		.active_low	= 1,
	},
};

static struct gpio_keys_button ap152_wl8200r2_gpio_keys[] __initdata = {
        {
                .desc           = "Reset button",
                .type           = EV_KEY,
                .code           = KEY_RESTART,
                .debounce_interval = AP152_KEYS_DEBOUNCE_INTERVAL,
                .gpio           = AP152_WL8200R2_GPIO_BTN_RESET,
                .active_low     = 1,
        },
};

/* W282 */

#define AP152_QTSW282_GPIO_LED_GREEN	13

#define AP152_QTSW282_GPIO_LED_LAN	    4
#define AP152_QTSW282_GPIO_LED_2G	    12
#define AP152_QTSW282_GPIO_LED_5G	    1

#define AP152_QTSW282_GPIO_BTN_RESET    17

static struct gpio_led ap152_qtsw282_leds_gpio[] __initdata = {
	{
		.name		= "ap152:green:status",
		.gpio		= AP152_QTSW282_GPIO_LED_GREEN,
		.active_low	= 1,
	},
};

static struct gpio_keys_button ap152_qtsw282_gpio_keys[] __initdata = {
        {
                .desc           = "Reset button",
                .type           = EV_KEY,
                .code           = KEY_RESTART,
                .debounce_interval = AP152_KEYS_DEBOUNCE_INTERVAL,
                .gpio           = AP152_QTSW282_GPIO_BTN_RESET,
                .active_low     = 1,
        },
};

/* A750 */

#define AP152_OKA750_GPIO_LED_GREEN	11

#define AP152_OKA750_GPIO_BTN_RESET 7

static struct gpio_led ap152_oka750_leds_gpio[] __initdata = {
	{
		.name		= "ap152:green:status",
		.gpio		= AP152_OKA750_GPIO_LED_GREEN,
		.active_low	= 1,
	},
};

static struct gpio_keys_button ap152_oka750_gpio_keys[] __initdata = {
        {
                .desc           = "Reset button",
                .type           = EV_KEY,
                .code           = KEY_RESTART,
                .debounce_interval = AP152_KEYS_DEBOUNCE_INTERVAL,
                .gpio           = AP152_OKA750_GPIO_BTN_RESET,
                .active_low     = 1,
        },
};

#endif /* OK_PATCH */




static struct ar8327_pad_cfg ap152_ar8337_pad0_cfg = {
	.mode = AR8327_PAD_MAC_SGMII,
	.sgmii_txclk_phase_sel = AR8327_SGMII_CLK_PHASE_RISE,
	.sgmii_rxclk_phase_sel = AR8327_SGMII_CLK_PHASE_FALL,
};

static struct ar8327_platform_data ap152_ar8337_data = {
	.pad0_cfg = &ap152_ar8337_pad0_cfg,
	.cpuport_cfg = {
		.force_link = 1,
		.speed = AR8327_PORT_SPEED_1000,
		.duplex = 1,
		.txpause = 1,
		.rxpause = 1,
	},
};

static struct mdio_board_info ap152_mdio0_info[] = {
	{
		.bus_id = "ag71xx-mdio.0",
		.phy_addr = 0,
		.platform_data = &ap152_ar8337_data,
	},
};

static void __init ap152_mdio_setup(void)
{
	ath79_gpio_output_select(AP152_GPIO_MDC, QCA956X_GPIO_OUT_MUX_GE0_MDC);
	ath79_gpio_output_select(AP152_GPIO_MDIO, QCA956X_GPIO_OUT_MUX_GE0_MDO);

	ath79_register_mdio(0, 0x0);
}

#if OK_PATCH
#define AP152_EINFO_OFFSET 0x20
#define AP152_EINFO_DEV_NAME_LEN 33
#define AP152_EINFO_DEV_NAME_OFFSET 10
static char ok_dev_name[AP152_EINFO_DEV_NAME_LEN] = {0};
static void __init ath79_init_dev_name(unsigned char *dst, const unsigned char *src)
{
    const unsigned char *ptr1;
    const unsigned char *ptr2;
    int size = 0;
    ptr1 = ptr2 = src + AP152_EINFO_OFFSET + AP152_EINFO_DEV_NAME_OFFSET;
    while (*ptr1++ != '\"');
    size = ptr1 - ptr2;
    size = (size < AP152_EINFO_DEV_NAME_LEN - 1) ? size : (AP152_EINFO_DEV_NAME_LEN - 1);
    memcpy(dst, ptr2, size);
    dst[size - 1]='\0';
}
#endif /* OK_PATCH */

static void __init ap152_setup(void)
{
	u8 *art = (u8 *) KSEG1ADDR(0x1fff0000);

	ath79_register_m25p80(NULL);

#if OK_PATCH
    ath79_init_dev_name(ok_dev_name, art);
    printk("-->OK_DEV_NAME=%s ", ok_dev_name);

/* supported device list */
#define AP152_QTSA820_DEV_NAME        "A820"
#define AP152_QTSA822_DEV_NAME        "A822"
#define AP152_QTSA920_DEV_NAME        "A920"
#define AP152_QTSA923_DEV_NAME        "A923"
#define AP152_WL8200T2_DEV_NAME       "WL8200-T2"
#define AP152_WL8200R2_DEV_NAME       "WL8200-R2"
#define AP152_WL8200I2_DEV_NAME       "WL8200-I2"
#define AP152_QTSW282_DEV_NAME        "W282"
#define AP152_OKA750_DEV_NAME         "A750"

    if (!strcmp(ok_dev_name, AP152_QTSA820_DEV_NAME) || !strcmp(ok_dev_name, AP152_QTSA822_DEV_NAME)) {
        ath79_register_leds_gpio(-1, ARRAY_SIZE(ap152_qtsa820_leds_gpio),
                ap152_qtsa820_leds_gpio);
        ath79_register_gpio_keys_polled(-1, AP152_KEYS_POLL_INTERVAL,
                ARRAY_SIZE(ap152_qtsa820_gpio_keys),
                ap152_qtsa820_gpio_keys);

    } else if (!strcmp(ok_dev_name, AP152_WL8200T2_DEV_NAME)) {
        ath79_register_leds_gpio(-1, ARRAY_SIZE(ap152_wl8200t2_leds_gpio),
                ap152_wl8200t2_leds_gpio);
        ath79_register_gpio_keys_polled(-1, AP152_KEYS_POLL_INTERVAL,
                ARRAY_SIZE(ap152_wl8200t2_gpio_keys),
                ap152_wl8200t2_gpio_keys);

    } else if (!strcmp(ok_dev_name, AP152_WL8200R2_DEV_NAME) || !strcmp(ok_dev_name, AP152_WL8200I2_DEV_NAME) ||
            !strcmp(ok_dev_name, AP152_QTSA920_DEV_NAME) || !strcmp(ok_dev_name, AP152_QTSA923_DEV_NAME)) {
        ath79_register_leds_gpio(-1, ARRAY_SIZE(ap152_wl8200r2_leds_gpio),
                ap152_wl8200r2_leds_gpio);
        ath79_register_gpio_keys_polled(-1, AP152_KEYS_POLL_INTERVAL,
                ARRAY_SIZE(ap152_wl8200r2_gpio_keys),
                ap152_wl8200r2_gpio_keys);

    } else if (!strcmp(ok_dev_name, AP152_QTSW282_DEV_NAME)) {
	    ath79_gpio_direction_select(AP152_QTSW282_GPIO_LED_LAN, true);
	    ath79_gpio_direction_select(AP152_QTSW282_GPIO_LED_2G, true);
	    ath79_gpio_direction_select(AP152_QTSW282_GPIO_LED_5G, true);
	    ath79_gpio_output_select(AP152_QTSW282_GPIO_LED_LAN,
			QCA953X_GPIO_OUT_MUX_LED_LINK5);
	    ath79_gpio_output_select(AP152_QTSW282_GPIO_LED_2G,
			QCA953X_GPIO_OUT_MUX_LED_LINK1);
	    ath79_gpio_output_select(AP152_QTSW282_GPIO_LED_5G,
			QCA953X_GPIO_OUT_MUX_LED_LINK2);

        ath79_register_leds_gpio(-1, ARRAY_SIZE(ap152_qtsw282_leds_gpio),
                ap152_qtsw282_leds_gpio);
        ath79_register_gpio_keys_polled(-1, AP152_KEYS_POLL_INTERVAL,
                ARRAY_SIZE(ap152_qtsw282_gpio_keys),
                ap152_qtsw282_gpio_keys);

        ath79_register_pci();

        ath79_register_wmac(art + AP152_WMAC_CALDATA_OFFSET, NULL);

        ath79_register_mdio(0, 0x0);
        ath79_register_mdio(1, 0x0);

        ath79_init_mac(ath79_eth0_data.mac_addr, art + AP152_MAC0_OFFSET, 0);
        ath79_init_mac(ath79_eth1_data.mac_addr, art + AP152_MAC1_OFFSET, 0);

        /* WAN port */
        ath79_eth0_data.phy_if_mode = PHY_INTERFACE_MODE_MII;
        ath79_eth0_data.speed = SPEED_100;
        ath79_eth0_data.duplex = DUPLEX_FULL;
        ath79_eth0_data.phy_mask = BIT(4);
        ath79_register_eth(0);

        /* LAN ports */
        ath79_eth1_data.phy_if_mode = PHY_INTERFACE_MODE_GMII;
        ath79_eth1_data.speed = SPEED_1000;
        ath79_eth1_data.duplex = DUPLEX_FULL;
        ath79_switch_data.phy_poll_mask |= BIT(4);
        ath79_switch_data.phy4_mii_en = 1;
        ath79_register_eth(1);

        return;

    } else if (!strcmp(ok_dev_name, AP152_OKA750_DEV_NAME)) {
        ath79_register_leds_gpio(-1, ARRAY_SIZE(ap152_oka750_leds_gpio),
                ap152_oka750_leds_gpio);
        ath79_register_gpio_keys_polled(-1, AP152_KEYS_POLL_INTERVAL,
                ARRAY_SIZE(ap152_oka750_gpio_keys),
                ap152_oka750_gpio_keys);

        ath79_register_pci();

        ath79_register_wmac(art + AP152_WMAC_CALDATA_OFFSET, NULL);

        ath79_register_mdio(0, 0x0);
        ath79_register_mdio(1, 0x0);

        ath79_init_mac(ath79_eth0_data.mac_addr, art + AP152_MAC0_OFFSET, 0);
        ath79_init_mac(ath79_eth1_data.mac_addr, art + AP152_MAC1_OFFSET, 0);

        /* WAN port */
        ath79_eth0_data.phy_if_mode = PHY_INTERFACE_MODE_MII;
        ath79_eth0_data.speed = SPEED_100;
        ath79_eth0_data.duplex = DUPLEX_FULL;
        ath79_eth0_data.phy_mask = BIT(4);
        ath79_register_eth(0);

        /* LAN ports */
        ath79_eth1_data.phy_if_mode = PHY_INTERFACE_MODE_GMII;
        ath79_eth1_data.speed = SPEED_1000;
        ath79_eth1_data.duplex = DUPLEX_FULL;
        ath79_switch_data.phy_poll_mask |= BIT(4);
        ath79_switch_data.phy4_mii_en = 1;
        ath79_register_eth(1);

        return;

    } else {
        printk("WARN: %s using default GPIO Setting.", ok_dev_name);
        ath79_register_leds_gpio(-1, ARRAY_SIZE(ap152_leds_gpio),
                ap152_leds_gpio);
        ath79_register_gpio_keys_polled(-1, AP152_KEYS_POLL_INTERVAL,
                ARRAY_SIZE(ap152_gpio_keys),
                ap152_gpio_keys);
    }

#else /* !OK_PATCH */

	ath79_register_leds_gpio(-1, ARRAY_SIZE(ap152_leds_gpio),
			ap152_leds_gpio);
    ath79_register_gpio_keys_polled(-1, AP152_KEYS_POLL_INTERVAL,
                                        ARRAY_SIZE(ap152_gpio_keys),
                                        ap152_gpio_keys);
	ath79_register_usb();

	ap152_mdio_setup();

	mdiobus_register_board_info(ap152_mdio0_info,
				    ARRAY_SIZE(ap152_mdio0_info));

	ath79_register_wmac(art + AP152_WMAC_CALDATA_OFFSET, NULL);
	ath79_register_pci();

	/* GMAC0 is connected to an AR8337 switch */
	ath79_init_mac(ath79_eth0_data.mac_addr, art + AP152_MAC0_OFFSET, 0);
	ath79_eth0_data.phy_if_mode = PHY_INTERFACE_MODE_SGMII;
	ath79_eth0_data.speed = SPEED_1000;
	ath79_eth0_data.duplex = DUPLEX_FULL;
	ath79_eth0_data.phy_mask = BIT(0);
	ath79_eth0_data.force_link = 1;
	ath79_eth0_data.mii_bus_dev = &ath79_mdio0_device.dev;
	ath79_eth0_pll_data.pll_1000 = 0x06000000;
	ath79_register_eth(0);
    return;

#endif /* OK_PATCH */

    ath79_register_usb();

    ap152_mdio_setup();

    mdiobus_register_board_info(ap152_mdio0_info,
            ARRAY_SIZE(ap152_mdio0_info));

    ath79_register_wmac(art + AP152_WMAC_CALDATA_OFFSET, NULL);
	ath79_register_pci();

    /* GMAC0 is connected to an AR8337 switch */
    ath79_init_mac(ath79_eth0_data.mac_addr, art + AP152_MAC0_OFFSET, 0);
    ath79_eth0_data.phy_if_mode = PHY_INTERFACE_MODE_SGMII;
    ath79_eth0_data.speed = SPEED_1000;
    ath79_eth0_data.duplex = DUPLEX_FULL;
    ath79_eth0_data.phy_mask = BIT(0);
    ath79_eth0_data.force_link = 1;
    ath79_eth0_data.mii_bus_dev = &ath79_mdio0_device.dev;
    ath79_eth0_pll_data.pll_1000 = 0x06000000;
    ath79_register_eth(0);
    return;
}

MIPS_MACHINE(ATH79_MACH_AP152, "AP152", "Qualcomm Atheros AP152 reference board",
	     ap152_setup);



#if OK_PATCH

#define UBNT_UNIFI_GPIO_LED_WHITE       7
#define UBNT_UNIFI_GPIO_LED_BLUE        8

static struct gpio_led ubnt_unifi_leds_gpio[] __initdata = {
	{
		.name		= "ubnt:white:dome",
		.gpio		= UBNT_UNIFI_GPIO_LED_WHITE,
		.active_low	= 0,
	}, {
		.name		= "ubnt:blue:dome",
		.gpio		= UBNT_UNIFI_GPIO_LED_BLUE,
		.active_low	= 0,
	}
};

#define UBNT_UNIFI_GPIO_BTN_RESET		2

#define UBNT_UNIFI_KEYS_POLL_INTERVAL	20
#define UBNT_UNIFI_KEYS_DEBOUNCE_INTERVAL	(3 * UBNT_UNIFI_KEYS_POLL_INTERVAL)


static struct gpio_keys_button ubnt_unifi_gpio_keys[] __initdata = {
	{
		.desc			= "reset",
		.type			= EV_KEY,
		.code			= KEY_RESTART,
		.debounce_interval	= UBNT_UNIFI_KEYS_DEBOUNCE_INTERVAL,
		.gpio			= UBNT_UNIFI_GPIO_BTN_RESET,
		.active_low		= 1,
	}
};

    
static struct flash_platform_data ubnt_unifi_flash_data = {
        /* mx25l12805d and mx25l12835f have the same JEDEC ID */
        .type = "mx25l12805d",
};

static void __init ubnt_unifi_pro_setup(void)
{
	u8 *eeprom = (u8 *) KSEG1ADDR(0x1fff0000);

	ath79_register_m25p80(&ubnt_unifi_flash_data);

	ath79_register_leds_gpio(-1, ARRAY_SIZE(ubnt_unifi_leds_gpio),
			ubnt_unifi_leds_gpio);

    ath79_register_gpio_keys_polled(-1, AP152_KEYS_POLL_INTERVAL,
                                        ARRAY_SIZE(ubnt_unifi_gpio_keys),
                                        ubnt_unifi_gpio_keys);

	ath79_register_usb();

	ap152_mdio_setup();

	mdiobus_register_board_info(ap152_mdio0_info,
				    ARRAY_SIZE(ap152_mdio0_info));

	ath79_register_wmac(eeprom + AP152_WMAC_CALDATA_OFFSET, NULL);

    ap91_pci_init(eeprom + AP152_PCI_CALDATA_OFFSET, NULL);

	/* GMAC0 is connected to an AR8337 switch */
	ath79_init_mac(ath79_eth0_data.mac_addr, eeprom + AP152_MAC0_OFFSET, 0);
	ath79_eth0_data.phy_if_mode = PHY_INTERFACE_MODE_SGMII;
	ath79_eth0_data.speed = SPEED_1000;
	ath79_eth0_data.duplex = DUPLEX_FULL;
	ath79_eth0_data.phy_mask = BIT(0);
	ath79_eth0_data.force_link = 1;
	ath79_eth0_data.mii_bus_dev = &ath79_mdio0_device.dev;
	ath79_eth0_pll_data.pll_1000 = 0x06000000;
	ath79_register_eth(0);
}


static struct ar8327_pad_cfg ubnt_lite_ar8337_pad0_cfg = {
	.mode = AR8327_PAD_MAC_SGMII,
	.sgmii_txclk_phase_sel = AR8327_SGMII_CLK_PHASE_RISE,
	.sgmii_rxclk_phase_sel = AR8327_SGMII_CLK_PHASE_FALL,
};

static struct ar8327_platform_data ubnt_lite_ar8337_data = {
	.pad0_cfg = &ubnt_lite_ar8337_pad0_cfg,
	.cpuport_cfg = {
		.force_link = 1,
		.speed = AR8327_PORT_SPEED_100,
		.duplex = 1,
		.txpause = 1,
		.rxpause = 1,
	},
};

static struct mdio_board_info ubnt_lite_mdio0_info[] = {
	{
		.bus_id = "ag71xx-mdio.0",
		.phy_addr = 0,
		.platform_data = &ubnt_lite_ar8337_data,
	},
};



static void __init ubnt_unifi_lite_setup(void)
{
	u8 *eeprom = (u8 *) KSEG1ADDR(0x1fff0000);

	ath79_register_m25p80(&ubnt_unifi_flash_data);

	ath79_register_leds_gpio(-1, ARRAY_SIZE(ubnt_unifi_leds_gpio),
			ubnt_unifi_leds_gpio);

    ath79_register_gpio_keys_polled(-1, AP152_KEYS_POLL_INTERVAL,
                                        ARRAY_SIZE(ubnt_unifi_gpio_keys),
                                        ubnt_unifi_gpio_keys);

	ath79_gpio_output_select(AP152_GPIO_MDC, QCA956X_GPIO_OUT_MUX_GE0_MDC);
	ath79_gpio_output_select(AP152_GPIO_MDIO, QCA956X_GPIO_OUT_MUX_GE0_MDO);
	ath79_register_mdio(0, ~BIT(4));

	mdiobus_register_board_info(ubnt_lite_mdio0_info,
				    ARRAY_SIZE(ubnt_lite_mdio0_info));

	ath79_register_wmac(eeprom + AP152_WMAC_CALDATA_OFFSET, NULL);
    ap91_pci_init(eeprom + AP152_PCI_CALDATA_OFFSET, NULL);

	/* GMAC0 is connected to an AR8337 switch */
	ath79_init_mac(ath79_eth0_data.mac_addr, eeprom + AP152_MAC0_OFFSET, 0);
	ath79_eth0_data.phy_if_mode = PHY_INTERFACE_MODE_SGMII;
	ath79_eth0_data.phy_mask = BIT(4);
	ath79_eth0_data.mii_bus_dev = &ath79_mdio0_device.dev;
	ath79_eth0_pll_data.pll_10 = 0x00001313;
	ath79_register_eth(0);
}

#define SYSTEM_ID_OFFSET 12
static void __init ok_ubnt_unifi_setup(void)
{
    u8 *eeprom = (u8 *) KSEG1ADDR(0x1fff0000);
    u16 system_id = ntohs(*((u16*)(eeprom+SYSTEM_ID_OFFSET)));

    if (system_id == 0xe517) {
        printk("-->OK_DEV_NAME=UBNTLITE");
        ubnt_unifi_lite_setup();
    } else if (system_id == 0xe527) {
        printk("-->OK_DEV_NAME=UBNTLR");
        ubnt_unifi_lite_setup();
    } else if (system_id == 0xe537) {
        printk("-->OK_DEV_NAME=UBNTPRO");
        ubnt_unifi_pro_setup();
    } else {
        printk("WARN: %x using default UBNTPRO Setting.", system_id);
        ubnt_unifi_pro_setup();
    }
}

MIPS_MACHINE(ATH79_MACH_UBNT_UNIFI, "UBNT-UF", "Ubiquiti UniFi",
	     ok_ubnt_unifi_setup);

#endif
