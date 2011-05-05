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
unsigned int Clamity::numOfBuffers(cl::Device device) {
    memoryInfo.device = device;

    size_t memSize  = device.getInfo<CL_DEVICE_GLOBAL_MEM_SIZE>();
    size_t memAlloc = device.getInfo<CL_DEVICE_MAX_MEM_ALLOC_SIZE>();

    if(memSize < memAlloc)
        memoryInfo.numOfBuffers = 1;
    else if( memSize > memAlloc)
        memoryInfo.numOfBuffers = (memSize - ((this->memorySizeDelta * 100)/memSize)) / memAlloc;

    return memoryInfo.numOfBuffers;
}


// Work out the maximum size you can allocated from this
// device - no the max alloc size - but what the device
// will return
    /*
unsigned int Clamity::maxMemoryAllocation(cl::Device device,
                                          cl::Context context) {
    bool success = false;
    if(memoryInfo.device != device) {

        //Obtain the memory sizes
        size_t memSize  = device.getInfo<CL_DEVICE_GLOBAL_MEM_SIZE>();
        size_t memAlloc = device.getInfo<CL_DEVICE_MAX_MEM_ALLOC_SIZE>();

        //pre calculate the first 10% shrink

        unsigned int memShrink = memSize -  ((this->memorySizeDelta * 100) / memSize);

        //We have an allocation that is bigger than the possible
        //global memory on the device... try allocating upto the
        //the global size minus a delta (around 10% of the global size)
        if(memSize < memAlloc) {
            while(1) {
                try {
                    cl::Buffer memory(context, CL_MEM_READ_WRITE, memSize - memShrink);
                    queue.enqueueWriteBuffer(memory, CL_TRUE, 0, sizeof(memSize), &size);
                    success = true;
                    break;
                } catch (cl::Error error) {
                    //Shrink the memory Size
                    memSize = memSize - memShrink;
                    //Check if the memory Size is now smaller than the delta
                    if(memSize < MemShrink)
                        break;
                    //Continue the loop
                    continue;
                }
            }
            if(success) {
                memoryInfo.device = device;
                memoryInfo.maxMemory = memSize;
                return memSize;
            }
            else {
                log(LOG_PANIC, "There has been an error Allocating memory on the device");
                return 0;
            }
        }

    }

    return memoryInfo.maxMemory;
}*/


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
