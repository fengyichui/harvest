# Kernel configuration targets
# These targets are used from top-level makefile

PHONY += menuconfig alldefconfig

srctree := .

Kconfig := $(srctree)/Kconfig

script := $(srctree)/script/kconfig

# export to kconfiglib.py
KCONFIG_CONFIG := $(srctree)/.config
export KCONFIG_CONFIG

# autoheader
KCONFIG_AUTOHEADER := $(srctree)/include/autoconf.h
export KCONFIG_AUTOHEADER

# menu config
menuconfig:
	@python3 $(script)/menuconfig.py

# all default config
alldefconfig:
	@python3 $(script)/alldefconfig.py

