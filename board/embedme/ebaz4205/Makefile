# SPDX-License-Identifier: GPL-2.0+
#
# (C) Copyright 2020
# Lukas Lichtl - admin@embed-me.com
#

obj-y	:= ebaz4205.o
obj-$(CONFIG_SPL_BUILD) += ps7_init_gpl.o

# Suppress "warning: function declaration isn't a prototype"
CFLAGS_REMOVE_ps7_init_gpl.o := -Wstrict-prototypes
