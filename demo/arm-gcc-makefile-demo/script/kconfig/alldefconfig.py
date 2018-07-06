#!/usr/bin/env python

# Copyright (c) 2018, Ulf Magnusson
# SPDX-License-Identifier: ISC

# Works like 'make alldefconfig'. Verified by the test suite to generate
# identical output to 'make alldefconfig' for all ARCHes.
#
# The default output filename is '.config'. A different filename can be passed
# in the KCONFIG_CONFIG environment variable.
#
# Usage for the Linux kernel:
#
#   $ make [ARCH=<arch>] scriptconfig SCRIPT=Kconfiglib/alldefconfig.py

import kconfiglib

def main():

    kconfiglib.standard_kconfig().write_config(
        kconfiglib.standard_config_filename())

    # liqiang+ {
    import os
    _config_autoheader_filename = os.environ.get("KCONFIG_AUTOHEADER")
    if _config_autoheader_filename is None:
        _config_autoheader_filename = "include/autoconf.h"
    kconfiglib.standard_kconfig().write_autoconf(_config_autoheader_filename)
    # }

if __name__ == "__main__":
    main()
