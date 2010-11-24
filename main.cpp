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

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

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
}

void Clamity::testDevice() {
    std::string name = device.getInfo<CL_DEVICE_NAME>();

    logfile << "clamity test started for " << name << std::endl;
    logfile << std::endl;

    // Log device info
    // In case of a bug report, this is critical
    logInfo();

    // Run the most basic tests first
    testAlloc();
    testBasic();
    memBasic();

    // Flush log now, in case of an exception or crash after
    logfile.flush();

    logfile << "clamity test completed for " << name << std::endl;
    logfile << std::endl;
}

static std::string logFileName(size_t nplatform, size_t ndevice, cl::Device &device) {
    std::ostringstream out;
    out << "clamity-" << nplatform << "-" << ndevice << "-";
    out << device.getInfo<CL_DEVICE_NAME>();
    out << ".txt";
    return out.str();
}

int main(int argc, char **argv) {
    // Number of child processes active
    size_t nchildren = 0;

    // Discover all OpenCL platforms
    // In general, each platform belongs to a specific vendor
    std::vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);

    for (size_t nplatform = 0; nplatform < platforms.size(); nplatform++) {
        cl::Platform platform = platforms.at(nplatform);

        // Discover all OpenCL devices of any type
        std::vector<cl::Device> devices;
        platform.getDevices(CL_DEVICE_TYPE_ALL, &devices);

        for (size_t ndevice = 0; ndevice < devices.size(); ndevice++) {
            cl::Device device = devices.at(ndevice);

#ifdef CLAMITY_DEBUG
            // Invoke tests in order, log to stdout
            Clamity test(std::cout, device);
            test.testDevice();
            continue;
#endif

            // Fork a child process to test this device
            const pid_t pid = ::fork();

            std::string logname = logFileName(nplatform, ndevice, device);

            if (pid == 0) { // child process
                // Allows the OpenCL implementation to
                // release platform and device resources
                platforms.clear();
                devices.clear();

                // Open log file
                std::ofstream logfile;
                logfile.open(logname.c_str());

                // Invoke tests
                Clamity test(logfile, device);
                test.testDevice();

                // Flush explicitly, in case an exception must be traced
                logfile.flush();
                logfile.close();
                return 0;
            } else { // parent process
                nchildren++;
                std::cerr << "PID " << pid << " will write to " << logname << std::endl;
            }
        }
    }

    while (nchildren > 0) {
        nchildren--;

        int status = 0;
        const pid_t pid = ::wait(&status);

        std::cerr << "PID " << pid << " has terminated with status " << status
                << ", " << nchildren << " PIDs left" << std::endl;
    }

    return 0;
}
