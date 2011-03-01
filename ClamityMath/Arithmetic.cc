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

#include "ClamityMath.hh"

#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <limits.h>


#include <cmath>  /* for std::abs(double) */

static bool isEqual(double x, double y, double epsilonErrorMargin)
{
    return std::abs(x - y) <= epsilonErrorMargin * std::abs(x);
}

static bool CheckResults(cl_float * data, cl_float * wanted, unsigned int vecCount, Logger &log, double epsilonErrorMargin) {

    using boost::format;
    using boost::str;

    for (size_t i = 0; i < vecCount; i++) {
        const cl_float have = data[i];
        const cl_float want = wanted[i];
        if (!isEqual(have, want, epsilonErrorMargin)) {
            log(LOG_ERROR,str(format("Test Failed --- Incorrect value at %d  (GPU has: %f - CPU Has: %f)") % i \
                    % have % want ));
            return false;
        }
    }
    return true;
}

void ClamityMath::basicALU(Clamity &subject) {

    using boost::format;
    using boost::str;

    Logger &log = subject.log;
    cl::Device &device = subject.device;
    cl::Context &context(subject.context);
    cl::CommandQueue queue(context, device);


    size_t memSize  = device.getInfo<CL_DEVICE_GLOBAL_MEM_SIZE>();
    size_t memAlloc = device.getInfo<CL_DEVICE_MAX_MEM_ALLOC_SIZE>()/MEMORY_FRACTION;

    // Work out group size
    size_t vecCount  = memAlloc / sizeof(cl_float);

    // Is the max alloc size a multiple of 4?
    size_t maxAllocMultiple = memSize / memAlloc;

    unsigned long memorySize = vecCount * sizeof(cl_float);

    log(LOG_INFO,"Basic ALU tests");
    log(LOG_INFO,"Testing for A = A + (B * C) - Executing over 65535 iterations");
    log(LOG_INFO,str(format("Math Global size : %d - Max Alloc Size: %d") % memSize % memAlloc));

    if (device.getInfo<CL_DEVICE_TYPE>() == CL_DEVICE_TYPE_CPU) {
       log(LOG_INFO,"CPU device detected skipping.... ");
       return;
    }

    if (maxAllocMultiple != 4)
       log(LOG_WARN,"CL_DEVICE_MAX_MEM_ALLOC_SIZE not a multiple of 4");

    cl::Program program;
    subject.compile(program, ClamityMathCL);

    cl::Kernel kern_membasic(program, "testALU");

    std::vector<cl_float> data(vecCount);
    std::vector<cl_float> dataA(vecCount);
    std::vector<cl_float> dataB(vecCount);

    for (size_t i = 0; i < vecCount; i++) {
        data[i]  =  rand() % UINT_MAX;
        dataA[i] =  rand() % (UINT_MAX / 2) ;
        dataB[i] =  rand() % (UINT_MAX / 2) ;
    }

    cl::Buffer memoryA(context, CL_MEM_READ_ONLY, memorySize);
    cl::Buffer memoryB(context, CL_MEM_READ_ONLY, memorySize);
    cl::Buffer memoryC(context, CL_MEM_WRITE_ONLY, memorySize);

    log(LOG_INFO,"Executing test kernel ");

    try {
        queue.enqueueWriteBuffer(memoryA, CL_TRUE, 0, memorySize, dataA.data());
        queue.enqueueWriteBuffer(memoryB, CL_TRUE, 0, memorySize, dataB.data());
        queue.enqueueWriteBuffer(memoryC, CL_TRUE, 0, memorySize, data.data());

        kern_membasic.setArg(0, memoryC);
        kern_membasic.setArg(1, memoryA);
        kern_membasic.setArg(2, memoryB);

        queue.enqueueNDRangeKernel(kern_membasic, cl::NullRange, cl::NDRange(vecCount), cl::NullRange);

        queue.finish();
        queue.enqueueReadBuffer(memoryC, CL_TRUE, 0, memorySize, data.data());
    } catch (cl::Error error) {
        log(LOG_PANIC, str(format("Test Failed --- %s '( %s)'") % error.what() % error.err()));
        return;
    }

    log(LOG_INFO,"  Test Passed");

}

void ClamityMath::basicFMAD(Clamity &subject) {

    using boost::format;
    using boost::str;

    Logger &log = subject.log;
    cl::Device &device = subject.device;
    cl::Context &context(subject.context);
    cl::CommandQueue queue(context, device);
    double epsilonErrorMargin = subject.epsilonErrorMargin;


    size_t memSize  = device.getInfo<CL_DEVICE_GLOBAL_MEM_SIZE>();
    size_t memAlloc = device.getInfo<CL_DEVICE_MAX_MEM_ALLOC_SIZE>()/MEMORY_FRACTION;

    // Work out group size
    size_t vecCount  = memAlloc / sizeof(cl_float);

    // Is the max alloc size a multiple of 4?
    size_t maxAllocMultiple = memSize / memAlloc;

    unsigned long memorySize = vecCount * sizeof(cl_float);

    log(LOG_INFO,"Basic ALU tests");
    log(LOG_INFO,"Testing for A = A + (B * C)");
    log(LOG_INFO,str(format("Math Global size : %d - Max Alloc Size: %d") % memSize % memAlloc));

    if (maxAllocMultiple != 4)
       log(LOG_WARN,"CL_DEVICE_MAX_MEM_ALLOC_SIZE not a multiple of 4");

    cl::Program program;
    subject.compile(program, ClamityMathCL);

    cl::Kernel kern_membasic(program, "BasicFMAD");

    std::vector<cl_float> data(vecCount);
    std::vector<cl_float> dataA(vecCount);
    std::vector<cl_float> dataB(vecCount);
    std::vector<cl_float> results(vecCount);

    log(LOG_INFO,"Creating results table");

    for (size_t i = 0; i < vecCount; i++) {
        data[i]  =  rand() % UINT_MAX;
        dataA[i] =  rand() % ( UINT_MAX /2 ) ;
        dataB[i] =  rand() % ( UINT_MAX /2 ) ;
        results[i] =  data[i] + ( dataA[i] * dataB[i]);   //Verify against the CPU results
    }

    cl::Buffer memoryA(context, CL_MEM_READ_ONLY, memorySize);
    cl::Buffer memoryB(context, CL_MEM_READ_ONLY, memorySize);
    cl::Buffer memoryC(context, CL_MEM_WRITE_ONLY, memorySize);

    log(LOG_INFO,"Executing test kernel ");

    try {
        queue.enqueueWriteBuffer(memoryA, CL_TRUE, 0, memorySize, dataA.data());
        queue.enqueueWriteBuffer(memoryB, CL_TRUE, 0, memorySize, dataB.data());
        queue.enqueueWriteBuffer(memoryC, CL_TRUE, 0, memorySize, data.data());

        kern_membasic.setArg(0, memoryC);
        kern_membasic.setArg(1, memoryA);
        kern_membasic.setArg(2, memoryB);

        queue.enqueueNDRangeKernel(kern_membasic, cl::NullRange, cl::NDRange(vecCount), cl::NullRange);

        queue.enqueueReadBuffer(memoryC, CL_TRUE, 0, memorySize, data.data());
    } catch (cl::Error error) {
        log(LOG_PANIC, str(format("Test Failed --- %s '( %s)'") % error.what() % error.err()));
        return;
    }

    log(LOG_INFO,"Verifying results");
    if (!CheckResults(data.data(), results.data(), vecCount, log,epsilonErrorMargin))
       log(LOG_ERROR,"  Test Failed!");
    else
       log(LOG_INFO,"  Test Passed");
}

void ClamityMath::basicADD(Clamity &subject) {

    using boost::format;
    using boost::str;

    Logger &log = subject.log;
    cl::Device &device = subject.device;
    cl::Context &context(subject.context);
    cl::CommandQueue queue(context, device);
    double epsilonErrorMargin = subject.epsilonErrorMargin;

    size_t memSize  = device.getInfo<CL_DEVICE_GLOBAL_MEM_SIZE>();
    size_t memAlloc = device.getInfo<CL_DEVICE_MAX_MEM_ALLOC_SIZE>() / MEMORY_FRACTION;

    // Work out group size
    size_t vecCount  = memAlloc / sizeof(cl_float);

    // Is the max alloc size a multiple of 4?
    size_t maxAllocMultiple = memSize / memAlloc;

    unsigned long memorySize = vecCount * sizeof(cl_float);

    log(LOG_INFO,"Basic ALU tests");
    log(LOG_INFO,"Testing for A = B + C");
    log(LOG_INFO,str(format("Math Global size : %d - Max Alloc Size: %d") % memSize % memAlloc));

    if (maxAllocMultiple != 4)
        log(LOG_WARN,"CL_DEVICE_MAX_MEM_ALLOC_SIZE not a multiple of 4");

    cl::Program program;
    subject.compile(program, ClamityMathCL);

    cl::Kernel kern_membasic(program, "BasicADD");

    std::vector<cl_float> data(vecCount);

    for (size_t i = 0; i < vecCount; i++) {
        data[i] = 1;
    }

    cl::Buffer memoryA(context, CL_MEM_READ_ONLY, memorySize);
    cl::Buffer memoryB(context, CL_MEM_READ_ONLY, memorySize);
    cl::Buffer memoryC(context, CL_MEM_WRITE_ONLY, memorySize);

    try {
        queue.enqueueWriteBuffer(memoryA, CL_TRUE, 0, memorySize, data.data());
        queue.enqueueWriteBuffer(memoryB, CL_TRUE, 0, memorySize, data.data());

        kern_membasic.setArg(0, memoryC);
        kern_membasic.setArg(1, memoryA);
        kern_membasic.setArg(2, memoryB);

        queue.enqueueNDRangeKernel(kern_membasic, cl::NullRange, cl::NDRange(vecCount), cl::NullRange);

        queue.enqueueReadBuffer(memoryC, CL_TRUE, 0, memorySize, data.data());
    } catch (cl::Error error) {
        log(LOG_PANIC,str(format("Test Failed --- %s '( %s)'") % error.what() % error.err()));
        return;
    }

    bool good = true;

    for (size_t i = 0; i < vecCount; i++) {
        const cl_float have = data[i];
        const cl_float want =  2.0f;

        if (have != want) {
            good = false;
            log(LOG_ERROR,str(format("Test Failed --- Incorrect value at %d  (have %f want %f)") % i \
                    % have % want ));
            return;
        }
    }

    std::vector<cl_float> dataA(vecCount);
    std::vector<cl_float> dataB(vecCount);
    std::vector<cl_float> results(vecCount);

    log(LOG_INFO,"Testing Random ADD operation ");

    for (size_t i = 0; i < vecCount; i++) {
        dataA[i] =  rand() % (UINT_MAX / 2);
        dataB[i] =  rand() % (UINT_MAX / 2);
        results[i] = dataA[i] + dataB[i];   //Verify against the CPU results
    }

    try {
        queue.enqueueWriteBuffer(memoryA, CL_TRUE, 0, memorySize, dataA.data());
        queue.enqueueWriteBuffer(memoryB, CL_TRUE, 0, memorySize, dataB.data());

        kern_membasic.setArg(0, memoryC);
        kern_membasic.setArg(1, memoryA);
        kern_membasic.setArg(2, memoryB);

        queue.enqueueNDRangeKernel(kern_membasic, cl::NullRange, cl::NDRange(vecCount), cl::NullRange);

        queue.enqueueReadBuffer(memoryC, CL_TRUE, 0, memorySize, data.data());
    } catch (cl::Error error) {
        log(LOG_PANIC,str(format("Test Failed --- %s '( %s)'") % error.what() % error.err()));
        return;
    }

    if (!CheckResults(data.data(), results.data(), vecCount, log, epsilonErrorMargin))
       log(LOG_ERROR,"  Test Failed!");
    else
       log(LOG_INFO,"  Test Passed");

}

void ClamityMath::basicMULT(Clamity &subject) {

    using boost::format;
    using boost::str;

    Logger &log = subject.log;
    cl::Device &device = subject.device;
    cl::Context &context(subject.context);
    cl::CommandQueue queue(context, device);
    double epsilonErrorMargin = subject.epsilonErrorMargin;


    size_t memSize  = device.getInfo<CL_DEVICE_GLOBAL_MEM_SIZE>();
    size_t memAlloc = device.getInfo<CL_DEVICE_MAX_MEM_ALLOC_SIZE>()/MEMORY_FRACTION;

    // Work out group size
    size_t vecCount  = memAlloc / sizeof(cl_float);

    // Is the max alloc size a multiple of 4?
    size_t maxAllocMultiple = memSize / memAlloc;

    unsigned long memorySize = vecCount * sizeof(cl_float);

    log(LOG_INFO,"Basic ALU tests");
    log(LOG_INFO,"Testing for A = B * C");
    log(LOG_INFO,str(format("Math Global size : %d  - Max Alloc Size: %d") % memSize % memAlloc));

    if (maxAllocMultiple != 4)
        log(LOG_INFO,"CL_DEVICE_MAX_MEM_ALLOC_SIZE not a multiple of 4");

    cl::Program program;
    subject.compile(program, ClamityMathCL);

    cl::Kernel kern_membasic(program, "BasicMUL");

    std::vector<cl_float> data(vecCount);
    std::vector<cl_float> dataA(vecCount);
    std::vector<cl_float> dataB(vecCount);
    std::vector<cl_float> results(vecCount);

    for (size_t i = 0; i < vecCount; i++) {
        dataA[i] = 1;
        dataB[i] = 1;
    }

    cl::Buffer memoryA(context, CL_MEM_READ_ONLY, memorySize);
    cl::Buffer memoryB(context, CL_MEM_READ_ONLY, memorySize);
    cl::Buffer memoryC(context, CL_MEM_WRITE_ONLY, memorySize);

    try {
        queue.enqueueWriteBuffer(memoryA, CL_TRUE, 0, memorySize, dataA.data());
        queue.enqueueWriteBuffer(memoryB, CL_TRUE, 0, memorySize, dataB.data());

        kern_membasic.setArg(0, memoryC);
        kern_membasic.setArg(1, memoryA);
        kern_membasic.setArg(2, memoryB);

        queue.enqueueNDRangeKernel(kern_membasic, cl::NullRange, cl::NDRange(vecCount), cl::NullRange);

        queue.enqueueReadBuffer(memoryC, CL_TRUE, 0, memorySize, data.data());
    } catch (cl::Error error) {
        log(LOG_PANIC,str(format("Test Failed --- %s '( %s)'") % error.what() % error.err()));
        return;
    }

    bool good = true;
    for (size_t i = 0; i < vecCount; i++) {
        const cl_float have = data[i];
        const cl_float want =  1.0f ; // (i % modulo) << shiftSize;

        if (have != want) {
            good = false;
            log(LOG_ERROR,str(format("Test Failed --- Incorrect value at %d  (have %f want %f)") % i \
                    % have % want ));
            return;
        }
    }

    log(LOG_INFO,"Testing Random MULT operation ");

    for (size_t i = 0; i < vecCount; i++) {
        dataA[i] =  rand() % (UINT_MAX / 2);
        dataB[i] =  rand() % (UINT_MAX / 2);
        results[i] = dataA[i] * dataB[i];   //Verify against the CPU results
    }

    try {
        queue.enqueueWriteBuffer(memoryA, CL_TRUE, 0, memorySize, dataA.data());
        queue.enqueueWriteBuffer(memoryB, CL_TRUE, 0, memorySize, dataB.data());

        kern_membasic.setArg(0, memoryC);
        kern_membasic.setArg(1, memoryA);
        kern_membasic.setArg(2, memoryB);

        queue.enqueueNDRangeKernel(kern_membasic, cl::NullRange, cl::NDRange(vecCount), cl::NullRange);

        queue.enqueueReadBuffer(memoryC, CL_TRUE, 0, memorySize, data.data());
    } catch (cl::Error error) {
        log(LOG_PANIC,str(format("Test Failed --- %s '( %s)'") % error.what() % error.err()));
        return;
    }

    if (!CheckResults(data.data(), results.data(), vecCount, log, epsilonErrorMargin))
       log(LOG_ERROR,"  Test Failed!");
    else
       log(LOG_INFO,"  Test Passed");

}
