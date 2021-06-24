# name of your application
APPLICATION = dock-device

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../RIOT

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
DEVELHELP ?= 1

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

ifeq (1,$(FAKE))
	CFLAGS += -DFAKE_SENSOR
endif

USEMODULE += xtimer
USEMODULE += periph_gpio
USEMODULE += shell
USEMODULE += shell_commands
USEMODULE += ps
USEPKG += jsmn

#Import devices and src folders
USEMODULE += devices
DIRS += $(CURDIR)/devices

USEMODULE += Common
DIRS += $(CURDIR)/../Common


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
USEMODULE += emcute
USEMODULE += stdio_ethos
USEMODULE += gnrc_uhcpc
# include UHCP client
USE_DHCPV6 ?= 0

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
DEVEUI ?= 5619846374413846
#APPLICATION EUI
APPEUI ?= 1234567899125648
#APPLICATION KEY
APPKEY ?= 99E078CD3F5398CE3C046C24953C2BD0
#Node ID integer, must be unique for each device
NODE_ID ?= 1

CFLAGS += -DNODE_ID=$(NODE_ID)
CFLAGS += -DCONFIG_LORAMAC_APP_KEY_DEFAULT=\"$(APPKEY)\"
CFLAGS += -DCONFIG_LORAMAC_APP_EUI_DEFAULT=\"$(APPEUI)\"
CFLAGS += -DCONFIG_LORAMAC_DEV_EUI_DEFAULT=\"$(DEVEUI)\"

endif

include $(CURDIR)/Makefile.conf.include
include $(RIOTBASE)/Makefile.include
