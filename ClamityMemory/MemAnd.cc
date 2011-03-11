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

#include "ClamityMemory.hh"

#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <limits.h>

void ClamityMemory::memBasicAnd(Clamity &subject) {

    using boost::format;
    using boost::str;

    Logger &log = subject.log;

    cl::Device &device = subject.device;
    cl::Context &context(subject.context);
    cl::CommandQueue queue(context, device);

    size_t memSize  = device.getInfo<CL_DEVICE_GLOBAL_MEM_SIZE>();
    size_t memAlloc = device.getInfo<CL_DEVICE_MAX_MEM_ALLOC_SIZE>();

    size_t maxBuff = subject.recommendMemory(memAlloc,memSize,3) / subject.memoryPoolFraction;

    // Work out group size
    size_t vecCount  = maxBuff / sizeof(cl_uint);

    // Is the max alloc size a multiple of 4?
    size_t maxAllocMultiple = memSize / memAlloc;

    unsigned long memorySize = vecCount * sizeof(cl_uint);

    log(LOG_INFO, "Random AND memory test");
    log(LOG_INFO, str(format("Memory Global size : %d Max Alloc Size: %d") % memSize % memAlloc));

    if (maxAllocMultiple != 4)
       log(LOG_WARN,"CL_DEVICE_MAX_MEM_ALLOC_SIZE not a multiple of 4");

    cl::Program program;
    subject.compile(program, ClamityMemoryCL);


    cl::Kernel kern_membasic(program, "testMemAndCL");

    std::vector<cl_uint> data(vecCount);
    std::vector<cl_uint> invertedData(vecCount);

    srand(time(NULL));

    unsigned int randomVal = 0;

    for (size_t i = 0; i < vecCount; i++) {
        randomVal  = rand() %  UINT_MAX ;
        data.at(i) = randomVal;
        invertedData.at(i) =~ randomVal;
    }

    cl::Buffer memoryA(context, CL_MEM_READ_ONLY, memorySize);
    cl::Buffer memoryB(context, CL_MEM_READ_ONLY, memorySize);
    cl::Buffer memoryC(context, CL_MEM_WRITE_ONLY, memorySize);

    try {
        queue.enqueueWriteBuffer(memoryA, CL_TRUE, 0, memorySize, data.data());
        queue.enqueueWriteBuffer(memoryB, CL_TRUE, 0, memorySize, invertedData.data());

        kern_membasic.setArg(0, memoryC);
        kern_membasic.setArg(1, memoryA);
        kern_membasic.setArg(2, memoryB);

        queue.enqueueNDRangeKernel(kern_membasic, cl::NullRange, cl::NDRange(vecCount), cl::NullRange);

        queue.enqueueReadBuffer(memoryC, CL_TRUE, 0, memorySize, data.data());
    } catch (cl::Error error) {
        log(LOG_PANIC, str(format("Test Failed --- %s '( %s)'") % error.what() % error.err()));
        return;
    }

    bool good = true;

    for (size_t i = 0; i < vecCount; i++) {
        const cl_uint have = data.at(i);
        const cl_uint want = 0;

        if (have != want) {
            good = false;
            log(LOG_ERROR, str(format("    Incorrect at value %d (have: %d want: %d) ") % i % have % want));
            return;
        }
    }

    if (good == true)
        log(LOG_INFO,"Passed Random Inverse AND");

    // In this phase of the test we will fill a buffer with random values then
    // Zero one of them, the resultant AND should be zero
    // To further test the buffers will be swapped and tested for Zero values

    for (size_t i = 0; i < vecCount; i++) {
        randomVal = rand() % UINT_MAX;
        data.at(i) = randomVal;
        invertedData.at(i) = 0;
    }

    try {
        queue.enqueueWriteBuffer(memoryA, CL_TRUE, 0, memorySize, data.data());
        queue.enqueueWriteBuffer(memoryB, CL_TRUE, 0, memorySize, invertedData.data());

        kern_membasic.setArg(0, memoryC);
        kern_membasic.setArg(1, memoryA);
        kern_membasic.setArg(2, memoryB);

        queue.enqueueNDRangeKernel(kern_membasic, cl::NullRange, cl::NDRange(vecCount), cl::NullRange);

        queue.enqueueReadBuffer(memoryC, CL_TRUE, 0, memorySize, data.data());
    } catch (cl::Error error) {
        log(LOG_PANIC, str(format("Test Failed --- %s '( %s)'") % error.what() % error.err()));
        return;
    }

    for (size_t i = 0; i < vecCount; i++) {
        const cl_uint have = data.at(i);
        const cl_uint want = 0;

        if (have != want) {
            good = false;
            log(LOG_ERROR, str(format("    Incorrect at value %d (have: %d want: %d) ") % i % have % want));
            return;
        }
    }

    if (good == true)
        log(LOG_INFO,"    Passed Random Zero AND part 1");

    for (size_t i = 0; i < vecCount; i++) {
        randomVal = rand() % UINT_MAX;
        data.at(i) = 0;
        invertedData.at(i) = randomVal;
    }

    try {
        queue.enqueueWriteBuffer(memoryA, CL_TRUE, 0, memorySize, data.data());
        queue.enqueueWriteBuffer(memoryB, CL_TRUE, 0, memorySize, invertedData.data());

        kern_membasic.setArg(0, memoryC);
        kern_membasic.setArg(1, memoryA);
        kern_membasic.setArg(2, memoryB);

        queue.enqueueNDRangeKernel(kern_membasic, cl::NullRange, cl::NDRange(vecCount), cl::NullRange);

        queue.enqueueReadBuffer(memoryC, CL_TRUE, 0, memorySize, data.data());
    } catch (cl::Error error) {
        log(LOG_PANIC, str(format("Test Failed --- %s '( %s)'") % error.what() % error.err()));
        return;
    }

    for (size_t i = 0; i < vecCount; i++) {
        const cl_uint have = data.at(i);
        const cl_uint want = 0;

        if (have != want) {
            good = false;
            log(LOG_ERROR, str(format("    Incorrect at value %d (have: %d want: %d) ") % i % have % want));
            return;
        }
    }

    if (good == true)
        log(LOG_INFO,"    Passed Random Zero AND part 2");
}
