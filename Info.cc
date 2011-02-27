// Copyright 2010 Dmitri Nikulin, Enzo Reyes.
//
// This file is part of clamity.
//
// clamity is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// clamity is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with clamity.  If not, see <http://www.gnu.org/licenses/>.

#include "Clamity.hh"

#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>

void Clamity::logInfo() {
    using boost::format;
    using boost::str;

    logSystem(LOG_INFO, "Platform");
    logSystem(LOG_INFO, str(format("  Vendor     %s") % platform.getInfo<CL_PLATFORM_VENDOR>()));
    logSystem(LOG_INFO, str(format("  Name       %s") % platform.getInfo<CL_PLATFORM_NAME>()));
    logSystem(LOG_INFO, str(format("  Version    %s") % platform.getInfo<CL_PLATFORM_VERSION>()));
    logSystem(LOG_INFO, str(format("  Profile    %s") % platform.getInfo<CL_PLATFORM_PROFILE>()));
    logSystem(LOG_INFO, "");

    logSystem(LOG_INFO, "Device");
    logSystem(LOG_INFO, str(format("  Vendor     %s") % device.getInfo<CL_DEVICE_VENDOR>()));
    logSystem(LOG_INFO, str(format("  Name       %s") % device.getInfo<CL_DEVICE_NAME>()));
    logSystem(LOG_INFO, str(format("  Version    %s") % device.getInfo<CL_DEVICE_VERSION>()));
    logSystem(LOG_INFO, str(format("  Type       %d") % device.getInfo<CL_DEVICE_TYPE>()));
    logSystem(LOG_INFO, str(format("  Profile    %s") % device.getInfo<CL_DEVICE_PROFILE>()));
    logSystem(LOG_INFO, "");

    // TODO: Log more device info
}
