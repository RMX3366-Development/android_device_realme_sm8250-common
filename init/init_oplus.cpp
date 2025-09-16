/*
 * Copyright (C) 2022-2023 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include <android-base/file.h>
#include <android-base/logging.h>
#include <android-base/properties.h>
#include <android-base/strings.h>

#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_
#include <sys/_system_properties.h>

using android::base::GetProperty;
using android::base::ReadFileToString;
using android::base::Split;
using android::base::Trim;

/*
 * SetProperty does not allow updating read only properties and as a result
 * does not work for our use case. Write "OverrideProperty" to do practically
 * the same thing as "SetProperty" without this restriction.
 */
void OverrideProperty(const char* name, const char* value) {
    size_t valuelen = strlen(value);

    prop_info* pi = (prop_info*)__system_property_find(name);
    if (pi != nullptr) {
        __system_property_update(pi, value, valuelen);
    } else {
        __system_property_add(name, strlen(name), value, valuelen);
    }
}

/*
 * Only for read-only properties. Properties that can be wrote to more
 * than once should be set in a typical init script (e.g. init.oplus.hw.rc)
 * after the original property has been set.
 */
void vendor_load_properties() {
    auto prjname = std::stoi(GetProperty("ro.boot.prjname", "0"));

    switch (prjname) {
        case 19795: // bladerunner CN
            OverrideProperty("ro.product.product.model", "RMX2071");
            OverrideProperty("ro.product.product.device", "RMX2071CN");
            break;
        case 20607: // bladerunner_48m CN
            OverrideProperty("ro.product.product.model", "RMX2072");
            OverrideProperty("ro.product.product.device", "RMX2072CN");
            break;
        case 19705: // bladerunner EU
            OverrideProperty("ro.product.product.model", "RMX2075");
            OverrideProperty("ro.product.product.device", "RMX2075L1");
            break;
        case 19706: // bladerunner IN
            OverrideProperty("ro.product.product.model", "RMX2076");
            OverrideProperty("ro.product.product.device", "RMX2076L1");
            break;
        case 21615: // rivena CN
            OverrideProperty("ro.product.product.model", "RMX3366");
            OverrideProperty("ro.product.product.device", "RE546F");
            break;
        case 21619: // bitra CN
        case 136730: // bitra CN (Dragon Ball Edition)
            OverrideProperty("ro.product.product.model", "RMX3370");
            OverrideProperty("ro.product.product.device", "RE5473");
            break;
        case 136858: // bitra IN
        case 136859: // bitra EU
            OverrideProperty("ro.product.product.model", "RMX3370");
            OverrideProperty("ro.product.product.device", "RE879AL1");
            break;
        case 21623: // spartan CN
            OverrideProperty("ro.product.product.model", "RMX3372");
            OverrideProperty("ro.product.product.device", "RE5477");
            break;
        case 21732: // spartan IN
        case 21733: // spartan EU
            OverrideProperty("ro.product.product.model", "RMX3371");
            OverrideProperty("ro.product.product.device", "RE54E4L1");
            break;
        default:
            LOG(ERROR) << "Unexpected project name: " << prjname;
    }

    if (std::string content; ReadFileToString("/proc/devinfo/ddr_type", &content)) {
        OverrideProperty("ro.boot.ddr_type", Split(Trim(content), "\t").back().c_str());
    }
}
