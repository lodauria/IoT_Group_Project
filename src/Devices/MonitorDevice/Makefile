# name of your application
APPLICATION = monitor-device

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../RIOT

ifeq (1,$(ETHOS))
BOARD ?= nucleo-f401re

CFLAGS += -DUSE_ETHOS

PLINK ?= ethos
# Include MQTT-S client
USEMODULE += emcute
# NOTE: 6LoWPAN will be included if IEEE802.15.4 devices are present
USEMODULE += gnrc_netdev_default
USEMODULE += auto_init_gnrc_netif
# Optimize network stack to for use with a single network interface
USEMODULE += gnrc_netif_single
# Specify the mandatory networking modules for IPv6
USEMODULE += gnrc_ipv6_default
USEMODULE += stdio_ethos
USEMODULE += gnrc_uhcpc
# include UHCP client
USE_DHCPV6 ?= 0
USEMODULE += shell
USEMODULE += shell_commands

# Ethos/native TAP interface and UHCP prefix can be configured from make command
TAP ?= tap0
IPV6_PREFIX ?= fe80:2::/64
ETHOS_BAUDRATE ?= 115200
include $(CURDIR)/Makefile.ethos.conf
.PHONY: host-tools

host-tools:
	$(Q)env -u CC -u CFLAGS $(MAKE) -C $(RIOTTOOLS)
else

	BOARD ?= b-l072z-lrwan1
	# Puy here LoRa stuff
	LORA_REGION ?= EU868
	USEPKG += semtech-loramac
	USEMODULE += semtech_loramac_rx
	USEMODULE += sx1276
	USEMODULE += fmt
	FEATURES_REQUIRED += periph_rtc

	#DEVICE EUI
	DEVEUI ?= 5646874374384864
	#APPLICATION EUI
	APPEUI ?= 5465454546546546
	#APPLICATION KEY
	APPKEY ?= 880D936E211D7156B584C5321BF0EADC
	#Node ID integer, must be unique for each device
	NODE_ID ?= 1

	CFLAGS += -DNODE_ID=$(NODE_ID)
	CFLAGS += -DCONFIG_LORAMAC_APP_KEY_DEFAULT=\"$(APPKEY)\"
	CFLAGS += -DCONFIG_LORAMAC_APP_EUI_DEFAULT=\"$(APPEUI)\"
	CFLAGS += -DCONFIG_LORAMAC_DEV_EUI_DEFAULT=\"$(DEVEUI)\"

endif

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
DEVELHELP ?= 1

USEMODULE += devices
DIRS += $(CURDIR)/devices

USEMODULE += Common
DIRS += $(CURDIR)/../Common

USEMODULE += xtimer
USEMODULE += ps

FEATURES_REQUIRED += periph_gpio periph_i2c
USEPKG += u8g2
USEPKG += jsmn


QUIET ?= 1

include	$(CURDIR)/Makefile.conf.include
include $(RIOTBASE)/Makefile.include
include $(RIOTMAKE)/default-radio-settings.inc.mk
