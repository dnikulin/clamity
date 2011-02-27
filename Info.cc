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

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <boost/algorithm/string.hpp>
using namespace std;
using namespace boost;


#include <boost/function.hpp>
#include <boost/format.hpp>

using boost::format;

void Clamity::logInfo() {
    logSystem(LOG_INFO,"Platform");
    logSystem(LOG_INFO,str(format("  Vendor  " + platform.getInfo<CL_PLATFORM_VENDOR>()  )));
    logSystem(LOG_INFO,str(format("  Name    " + platform.getInfo<CL_PLATFORM_NAME>()    )));
    logSystem(LOG_INFO,str(format("  Version " + platform.getInfo<CL_PLATFORM_VERSION>() )));
    logSystem(LOG_INFO,str(format("  Profile " + platform.getInfo<CL_PLATFORM_PROFILE>() )));
    logSystem(LOG_INFO,"");

    logSystem(LOG_INFO,"Device");
    logSystem(LOG_INFO,str(format("  Vendor  " + device.getInfo<CL_DEVICE_VENDOR>() )));
    logSystem(LOG_INFO,str(format("  Name    " + device.getInfo<CL_DEVICE_NAME>()   )));
    logSystem(LOG_INFO,str(format("  Version " + device.getInfo<CL_DEVICE_VERSION>() )));
    logSystem(LOG_INFO,str(format("  Type    " + device.getInfo<CL_DEVICE_TYPE>() )));
    logSystem(LOG_INFO,str(format("  Profile " + device.getInfo<CL_DEVICE_PROFILE>() )));


    // TODO: Log more device info
}
