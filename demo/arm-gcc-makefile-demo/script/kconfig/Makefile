# Kernel configuration targets
# These targets are used from top-level makefile

PHONY += menuconfig

srctree := .

Kconfig := $(srctree)/Kconfig

script := $(srctree)/script/kconfig

KCONFIG_CONFIG := $(srctree)/.config
export KCONFIG_CONFIG

KCONFIG_AUTOHEADER := $(srctree)/include/autoconf.h
export KCONFIG_AUTOHEADER

menuconfig:
	@python3 $(script)/menuconfig.py

