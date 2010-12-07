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

#include "clamity.h"

#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <limits.h>

void Clamity::memBasicAnd() {
    static const size_t groupSize = 256;

    size_t memSize  = device.getInfo<CL_DEVICE_GLOBAL_MEM_SIZE>();
    size_t memAlloc = device.getInfo<CL_DEVICE_MAX_MEM_ALLOC_SIZE>();

    // Work out group size
    size_t vecCount  = memAlloc / sizeof(cl_uint);

    // Is the max alloc size a multiple of 4?
    size_t maxAllocMultiple = memSize / memAlloc;


    unsigned long memorySize = vecCount * sizeof(cl_uint);


    logfile << "Random AND memory test" << std::endl;
    logfile << "Memory Global size : " << memSize << " Max Alloc Size: "<<memAlloc <<std::endl;
    logfile << std::endl;

    if (maxAllocMultiple != 4)
       logfile << "CL_DEVICE_MAX_MEM_ALLOC_SIZE not a multiple of 4" <<std::endl;

    cl::Program program;
    compile(program, "membasic.cl");

    logfile.flush();

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

        queue.enqueueNDRangeKernel(kern_membasic, cl::NDRange(), cl::NDRange(vecCount), cl::NDRange(groupSize));

        queue.enqueueReadBuffer(memoryC, CL_TRUE, 0, memorySize, data.data());
    } catch (cl::Error error) {
           logfile << "Test Failed --- ";
           logfile << error.what() << "(" << error.err() << ")" << std::endl;
           return;
    }

    bool good = true;

    for (size_t i = 0; i < vecCount; i++) {
        const cl_uint have = data.at(i);
        const cl_uint want = 0;

        if (have != want) {
            good = false;
            logfile << "    Incorrect value at " << i
                    << " (have " << have << ", want " << want << ")" << std::endl;
            return;
        }
    }

    if (good == true)
        logfile <<"    Passed Random Inverse AND" << std::endl;

    // In this phase of the test we will fill a buffer with random values then
    // Zero one of them, the resultant AND should be zero
    // To further test the buffers will be swapped and tested for Zero values

    for(size_t i = 0; i < vecCount; i++) {
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

        queue.enqueueNDRangeKernel(kern_membasic, cl::NDRange(), cl::NDRange(vecCount), cl::NDRange(groupSize));

        queue.enqueueReadBuffer(memoryC, CL_TRUE, 0, memorySize, data.data());
    } catch (cl::Error error) {
           logfile << "Test Failed --- ";
           logfile << error.what() << "(" << error.err() << ")" << std::endl;
           return;
    }

    for (size_t i = 0; i < vecCount; i++) {
        const cl_uint have = data.at(i);
        const cl_uint want = 0;

        if (have != want) {
            good = false;
            logfile << "    Incorrect value at " << i
                    << " (have " << have << ", want " << want << ")" << std::endl;
            return;
        }
    }

    if (good == true)
        logfile <<"    Passed Random Zero AND part 1" << std::endl;


    for(size_t i = 0; i < vecCount; i++) {
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

        queue.enqueueNDRangeKernel(kern_membasic, cl::NDRange(), cl::NDRange(vecCount), cl::NDRange(groupSize));

        queue.enqueueReadBuffer(memoryC, CL_TRUE, 0, memorySize, data.data());
    } catch (cl::Error error) {
           logfile << "Test Failed --- ";
           logfile << error.what() << "(" << error.err() << ")" << std::endl;
           return;
    }

    for (size_t i = 0; i < vecCount; i++) {
        const cl_uint have = data.at(i);
        const cl_uint want = 0;

        if (have != want) {
            good = false;
            logfile << "    Incorrect value at " << i
                    << " (have " << have << ", want " << want << ")" << std::endl;
            return;
        }
    }

    if (good == true)
        logfile <<"    Passed Random Zero AND part 2" << std::endl;

}
