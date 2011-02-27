// Copyright 2010-2011 Dmitri Nikulin, Enzo Reyes.
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

#include <fstream>
#include <iostream>
#include <sstream>

Clamity::Clamity(std::ostream &_logfile, cl::Device &_device)
    : logfile(_logfile), device(_device) {

    // Retrieve platform from device
    platform = device.getInfo<CL_DEVICE_PLATFORM>();

    // Create context containing only this device
    std::vector<cl::Device> devices;
    devices.push_back(device);
    context = cl::Context(devices);

    // Create command queue
    queue = cl::CommandQueue(context, device);
    log = makeStreamLogger(LOG_DEBUG,"CLAMITY",&_logfile);
}

void Clamity::testDevice() {
    using boost::format;
    using boost::str;

    std::string name = device.getInfo<CL_DEVICE_NAME>();

    log(LOG_INFO,str(format("Clamity test started for %s ") % name));
    // logfile << "clamity test started for " << name << std::endl;
    //logfile << std::endl;

    // Log device info
    // In case of a bug report, this is critical
    logInfo();

    // Run the most basic tests first
    testAlloc();
    testBasic();
    memBasic();
    memBasicAnd();
    basicADD();
    basicMULT();
    basicFMAD();
    basicALU();
    // Flush log now, in case of an exception or crash after
    logfile.flush();

    logfile << "clamity test completed for " << name << std::endl;
    logfile << std::endl;
}

int main(int argc, char **argv) {
    // Discover all OpenCL platforms
    // In general, each platform belongs to a specific vendor
    std::vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);

    try {
        for (size_t nplatform = 0; nplatform < platforms.size(); nplatform++) {
            cl::Platform platform = platforms.at(nplatform);

            // Discover all OpenCL devices of any type
            std::vector<cl::Device> devices;
            platform.getDevices(CL_DEVICE_TYPE_ALL, &devices);

            for (size_t ndevice = 0; ndevice < devices.size(); ndevice++) {
                cl::Device device = devices.at(ndevice);

                // Invoke tests in order, log to stdout
                Clamity test(std::cout, device);
                test.testDevice();
            }
        }
    } catch (cl::Error error) {
         std::cout << "Test Failed --- ";
         std::cout << error.what() << "(" << error.err() << ")" << std::endl;
    }

    return 0;
}
