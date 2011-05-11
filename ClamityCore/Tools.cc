// Copyright 2010 Dmitri Nikulin,Enzo Reyes
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

#include <QFile>


// Work out the number of buffers required to allocate
// all memory on the device
unsigned int Clamity::numOfBuffers(cl::Device &device) {
    memoryInfo.device = device;

    size_t memSize  = device.getInfo<CL_DEVICE_GLOBAL_MEM_SIZE>();
    size_t memAlloc = device.getInfo<CL_DEVICE_MAX_MEM_ALLOC_SIZE>();

    if(memSize < memAlloc)
        memoryInfo.numOfBuffers = 1;
    else if( memSize > memAlloc)
        memoryInfo.numOfBuffers = (memSize - ((this->memorySizeDelta * 100)/memSize)) / memAlloc;

    return memoryInfo.numOfBuffers;
}

//In case the user over-rides the available memory
//This functions will act as a mask for the CL versions

unsigned int Clamity::deviceMemoryAvail(cl::Device &device) {
    if(maxmem!=0)
        return maxmem;
    return device.getInfo<CL_DEVICE_GLOBAL_MEM_SIZE>();
}

unsigned int Clamity::maxMemoryAllocation(cl::Device &device) {
    if(maxalloc!=0)
        return maxalloc;
    return device.getInfo<CL_DEVICE_MAX_MEM_ALLOC_SIZE>();
}


// Try to work out a buffer size that occupies all of memory
// removing the dependency on MAX_ALLOC
unsigned int Clamity::recommendMemory(unsigned int deviceAllow, unsigned int maxGlobal,
                                      unsigned int numberOfBuffers) {
    unsigned int buffSize = maxGlobal / numberOfBuffers;
    unsigned int maxRequest = deviceAllow * numberOfBuffers;
    if(buffSize < deviceAllow)
        return buffSize;

    //We are here because the we are requesting a buffer
    //Bigger than we can allocate according to the driver

    //If the max we can request is less than global return
    if(maxRequest < maxGlobal)
        return deviceAllow;

    //We cannot allocate a buffer bigger than deviceAllow
    //But maxRequest is > than maxGlobal
    unsigned int maxDelta = maxRequest - maxGlobal;

    //Return the difference b/w these two
    return (buffSize - maxDelta);
}

void Clamity::reportCompile(cl::Program &program) {

    using boost::format;
    using boost::str;

    std::string buildlog = program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device);

    if (buildlog.size() > 0) {
        log(LOG_INFO, buildlog);
    }
}

void Clamity::compile(cl::Program &program, const char *path) {

    using boost::format;
    using boost::str;

    QFile file(path);

    if (file.open(QFile::ReadOnly)) {
        QByteArray code = file.readAll();
        file.close();

        cl::Program::Sources sources(1, std::make_pair(code.data(), code.size()));
        program = cl::Program(context, sources);

        try {
            log(LOG_INFO,str(format("Compiling '%s' %d bytes") % path  % code.size()));
            program.build(devices, "");

            // Save compile log even if successful,
            // as it may have warnings
            reportCompile(program);
        } catch (cl::Error error) {
            // Save compile log if there was a build exception
            if (error.err() == CL_BUILD_PROGRAM_FAILURE) {
                log(LOG_ERROR,"build failed");
                reportCompile(program);
            }

            // Re-throw the error anyway,
            // so that the whole test fails
            throw error;
        }
    }
}
