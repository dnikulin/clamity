// Copyright 2010 Dmitri Nikulin.
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

#include "clamity.h"

void Clamity::logInfo() {
    logfile << "Platform" << std::endl;
    logfile << "  Vendor            " << platform.getInfo<CL_PLATFORM_VENDOR>() << std::endl;
    logfile << "  Name              " << platform.getInfo<CL_PLATFORM_NAME>() << std::endl;
    logfile << "  Version           " << platform.getInfo<CL_PLATFORM_VERSION>() << std::endl;
    logfile << "  Profile           " << platform.getInfo<CL_PLATFORM_PROFILE>() << std::endl;
    logfile << std::endl;

    logfile << "Device" << std::endl;
    logfile << "  Vendor            " << device.getInfo<CL_DEVICE_VENDOR>() << std::endl;
    logfile << "  Name              " << device.getInfo<CL_DEVICE_NAME>() << std::endl;
    logfile << "  Version           " << device.getInfo<CL_DEVICE_VERSION>() << std::endl;
    logfile << "  Type              " << device.getInfo<CL_DEVICE_TYPE>() << std::endl;
    logfile << "  Profile           " << device.getInfo<CL_DEVICE_PROFILE>() << std::endl;
    logfile << std::endl;

    // TODO: Log more device info
}
