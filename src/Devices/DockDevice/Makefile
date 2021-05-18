# name of your application
APPLICATION = dock-device

# If no BOARD is found in the environment, use this default:
BOARD ?= nucleo-f401re

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

#Import devices and src folders
USEMODULE += devices
DIRS += $(CURDIR)/devices

USEMODULE += xtimer
USEMODULE += periph_gpio
USEMODULE += srf04
USEMODULE += shell
USEMODULE += shell_commands
USEMODULE += ps

USEMODULE += Common
DIRS += $(CURDIR)/../Common

# NOTE: 6LoWPAN will be included if IEEE802.15.4 devices are present
USEMODULE += gnrc_netdev_default
USEMODULE += auto_init_gnrc_netif
# Optimize network stack to for use with a single network interface
USEMODULE += gnrc_netif_single
# Specify the mandatory networking modules for IPv6
USEMODULE += gnrc_ipv6_default
USEMODULE += emcute
ifeq (1,$(ETHOS))
CFLAGS += -DUSE_ETHOS

PLINK ?= ethos
# Include MQTT-S client
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
# Include packages that pull up and auto-init the link layer.
# Activate ICMPv6 error messages
USEMODULE += gnrc_icmpv6_error
# Specify the mandatory networking modules for IPv6 and UDP
USEMODULE += gnrc_ipv6_router_default
USEMODULE += gnrc_udp
# Add a routing protocol
USEMODULE += gnrc_rpl
USEMODULE += auto_init_gnrc_rpl
# This application dumps received packets to STDIO using the pktdump module
USEMODULE += gnrc_pktdump
# Additional networking modules that can be dropped if not needed
USEMODULE += gnrc_icmpv6_echo
# Add also the shell, some shell commands
# Change this to 0 show compiler invocation lines by default:
USEMODULE += netstats_l2
USEMODULE += netstats_ipv6
USEMODULE += netstats_rpl
endif

USEPKG += jsmn

include $(CURDIR)/Makefile.conf.include
include $(RIOTBASE)/Makefile.include
include $(RIOTMAKE)/default-radio-settings.inc.mk