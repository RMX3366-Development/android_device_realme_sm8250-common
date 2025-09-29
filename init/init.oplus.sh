#! /vendor/bin/sh
#
# Copyright (C) 2022 The LineageOS Project
#
# SPDX-License-Identifier: Apache-2.0
#

prjname=$(getprop ro.boot.prjname)

# Radio
case "$prjname" in
    19706|19795|20607|21615|21619|2161A|2169A|2169B|21623|21732|21733)
        setprop vendor.radio.multisim.config dsds
        ;;
    19705)
        setprop vendor.radio.multisim.config ssss
        ;;
esac
