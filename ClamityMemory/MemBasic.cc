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

bool ClamityMemory::memBasic(Clamity &subject) {

    using boost::format;
    using boost::str;

    Logger &log = subject.log;
    cl::Device &device = subject.device;
    cl::Context &context(subject.context);
    cl::CommandQueue queue(context, device);

    static const unsigned int maxShift = 32;  // We can only shift up to 31 places

    unsigned int currShift = 0;  // How many times we have shifted already
    unsigned int shiftedVal = 1;

    size_t memSize  =  subject.deviceMemoryAvail(device);//device.getInfo<CL_DEVICE_GLOBAL_MEM_SIZE>();
    size_t memAlloc = subject.maxMemoryAllocation(device);     //device.getInfo<CL_DEVICE_MAX_MEM_ALLOC_SIZE>();

    size_t maxBuff = subject.recommendMemory(memAlloc,memSize,3) / subject.memoryPoolFraction;

    // Work out group size
    size_t vecCount  = maxBuff / sizeof(cl_uint);

    // Is the max alloc size a multiple of 4?
    size_t maxAllocMultiple = memSize / memAlloc;

    unsigned long memorySize = vecCount * sizeof(cl_uint);

    log(LOG_INFO,"Basic memory tests");
    log(LOG_INFO, str(format("Memory Global size : %d Max Alloc Size: %d") % memSize % memAlloc));

    if (maxAllocMultiple != 4)
       log(LOG_WARN,"CL_DEVICE_MAX_MEM_ALLOC_SIZE not a multiple of 4");

    cl::Program program;
    subject.compile(program, ClamityMemoryCL);

    cl::Kernel kern_membasic(program, "testMemCL");

    std::vector<cl_uint> data(vecCount);
    for (size_t i = 0; i < vecCount; i++)
        data.at(i) = shiftedVal;

    cl::Buffer memoryA(context, CL_MEM_READ_ONLY, memorySize);
    cl::Buffer memoryB(context, CL_MEM_READ_ONLY, memorySize);
    cl::Buffer memoryC(context, CL_MEM_WRITE_ONLY, memorySize);

    do {
        try {
            queue.enqueueWriteBuffer(memoryA, CL_TRUE, 0, memorySize, data.data());
            queue.enqueueWriteBuffer(memoryB, CL_TRUE, 0, memorySize, data.data());

            kern_membasic.setArg(0, memoryC);
            kern_membasic.setArg(1, memoryA);
            kern_membasic.setArg(2, memoryB);

            queue.enqueueNDRangeKernel(kern_membasic, cl::NullRange, cl::NDRange(vecCount), cl::NullRange);

            queue.enqueueReadBuffer(memoryC, CL_TRUE, 0, memorySize, data.data());
        } catch (cl::Error error) {
            log(LOG_PANIC, str(format("Test Failed --- %s '( %s)'") % error.what() % error.err()));
            testLevel = TEST_PANIC;
            //Get a generic descriptor of the error at hand
            errorReported = getErrorType(error.err());
            return false;
        }

        bool good = true;

        for (size_t i = 0; i < vecCount; i++) {
            const cl_uint have = data.at(i);
            const cl_uint want = shiftedVal * 2 ; // (i % modulo) << shiftSize;

            if (have != want) {
                good = false;
                log(LOG_ERROR, str(format("    Incorrect at offset %d (have: %d want: %d) ") % i % have % want));
                testLevel = TEST_ERROR;
                return false;
            }
        }

        if (good == true)
            log(LOG_INFO,str(format("Shift: %d ......Passed") % currShift));

        currShift++;

        for (size_t i = 0; i < vecCount; i++)
            data.at(i) = shiftedVal << currShift;

        shiftedVal <<= currShift;
    } while (currShift < maxShift);
    return true;
}
