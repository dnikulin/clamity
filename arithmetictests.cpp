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

bool CheckResults(cl_float * data, cl_float * wanted ,unsigned int vecCount, std::ostream  &logfile) {
  
  for (size_t i = 0; i < vecCount; i++) {
        const cl_float have = data[i];
        const cl_float want = wanted[i] ; 
        if (have != want) {
           logfile << "    Incorrect value at " << i
                    << " (have " << have << ", want " << want << ")" << std::endl;
            return false;
        }
    }
  return true;
}


void Clamity::basicADD() {
    static const size_t groupSize = 256;

    size_t memSize  = device.getInfo<CL_DEVICE_GLOBAL_MEM_SIZE>();
    size_t memAlloc = device.getInfo<CL_DEVICE_MAX_MEM_ALLOC_SIZE>();

    // Work out group size
    size_t vecCount  = memAlloc / sizeof(cl_float);

    // Is the max alloc size a multiple of 4?
    size_t maxAllocMultiple = memSize / memAlloc;


    unsigned long memorySize = vecCount * sizeof(cl_float);


    logfile << "Basic ALU tests" << std::endl;
    logfile << "Testing for A = B + C" <<std::endl;
    logfile << "Memory Global size : " << memSize << " Max Alloc Size: "<<memAlloc <<std::endl;
    logfile << std::endl;

    if (maxAllocMultiple != 4)
       logfile << "CL_DEVICE_MAX_MEM_ALLOC_SIZE not a multiple of 4" <<std::endl;

    cl::Program program;
    compile(program, "basicsum.cl");

    logfile.flush();

    cl::Kernel kern_membasic(program, "BasicADD");

    //std::vector<cl_float4> data(vecCount);
    cl_float * data = (cl_float *) malloc(sizeof(cl_float) * vecCount);
    for (size_t i = 0; i < vecCount; i++) {
        data[i] = 1;
    }

    cl::Buffer memoryA(context, CL_MEM_READ_ONLY, memorySize);
    cl::Buffer memoryB(context, CL_MEM_READ_ONLY, memorySize);
    cl::Buffer memoryC(context, CL_MEM_WRITE_ONLY, memorySize);

    try {
        queue.enqueueWriteBuffer(memoryA, CL_TRUE, 0, memorySize, data);
        queue.enqueueWriteBuffer(memoryB, CL_TRUE, 0, memorySize, data);

        kern_membasic.setArg(0, memoryC);
        kern_membasic.setArg(1, memoryA);
        kern_membasic.setArg(2, memoryB);

        queue.enqueueNDRangeKernel(kern_membasic, cl::NDRange(), cl::NDRange(vecCount), cl::NDRange(groupSize));

        queue.enqueueReadBuffer(memoryC, CL_TRUE, 0, memorySize, data);
    } catch (cl::Error error) {
           logfile << "Test Failed --- ";
           logfile << error.what() << "(" << error.err() << ")" << std::endl;
           free(data);
           return;
    }

    bool good = true;

    for (size_t i = 0; i < vecCount; i++) {
        const cl_float have = data[i];
        const cl_float want =  2.0f ; // (i % modulo) << shiftSize;

        if (have != want) {
            good = false;
            logfile << "    Incorrect value at " << i
                    << " (have " << have << ", want " << want << ")" << std::endl;
            return;
        }
    }
    cl_float * dataA = (cl_float *) malloc(sizeof(cl_float) * vecCount);
    cl_float * dataB = (cl_float *) malloc(sizeof(cl_float) * vecCount);
    cl_float * results = (cl_float *) malloc(sizeof(cl_float) * vecCount);
    
    logfile <<"Testing Random ADD operation " << std::endl;
    
    
    for (size_t i = 0; i < vecCount; i++) {
        dataA[i] =  rand() % ( UINT_MAX /2 ) ;
        dataB[i] =  rand() % ( UINT_MAX /2 ) ;
	results[i] = dataA[i] + dataB[i];   //Verify against the CPU results
    }
    
     try {
        queue.enqueueWriteBuffer(memoryA, CL_TRUE, 0, memorySize, dataA);
        queue.enqueueWriteBuffer(memoryB, CL_TRUE, 0, memorySize, dataB);

        kern_membasic.setArg(0, memoryC);
        kern_membasic.setArg(1, memoryA);
        kern_membasic.setArg(2, memoryB);

        queue.enqueueNDRangeKernel(kern_membasic, cl::NDRange(), cl::NDRange(vecCount), cl::NDRange(groupSize));

        queue.enqueueReadBuffer(memoryC, CL_TRUE, 0, memorySize, data);
    } catch (cl::Error error) {
           logfile << "Test Failed --- ";
           logfile << error.what() << "(" << error.err() << ")" << std::endl;
           free(data);
           return;
    }
    if(!CheckResults(data,results,vecCount,logfile))
       logfile<<"  Test Failed!"<<std::endl;
    else
       logfile<<"  Test Passed"<<std::endl;
    
    
    free(data);
    free(dataA);
    free(dataB);
}

void Clamity::basicMULT() {
    static const size_t groupSize = 256;

    size_t memSize  = device.getInfo<CL_DEVICE_GLOBAL_MEM_SIZE>();
    size_t memAlloc = device.getInfo<CL_DEVICE_MAX_MEM_ALLOC_SIZE>();

    // Work out group size
    size_t vecCount  = memAlloc / sizeof(cl_float);

    // Is the max alloc size a multiple of 4?
    size_t maxAllocMultiple = memSize / memAlloc;


    unsigned long memorySize = vecCount * sizeof(cl_float);


    logfile << "Basic ALU tests" << std::endl;
    logfile << "Testing for A = B * C" <<std::endl;
    logfile << "Memory Global size : " << memSize << " Max Alloc Size: "<<memAlloc <<std::endl;
    logfile << std::endl;

    if (maxAllocMultiple != 4)
       logfile << "CL_DEVICE_MAX_MEM_ALLOC_SIZE not a multiple of 4" <<std::endl;

    cl::Program program;
    compile(program, "basicsum.cl");

    logfile.flush();

    cl::Kernel kern_membasic(program, "BasicMUL");

    //std::vector<cl_float4> data(vecCount);
    cl_float * dataA = (cl_float *) malloc(sizeof(cl_float) * vecCount);
    cl_float * dataB = (cl_float *) malloc(sizeof(cl_float) * vecCount);
    cl_float * data = (cl_float *) malloc(sizeof(cl_float) * vecCount);
    cl_float * results = (cl_float *) malloc(sizeof(cl_float) * vecCount);
    for (size_t i = 0; i < vecCount; i++) {
        dataA[i] = 1;
        dataB[i] = 1;
    }

    cl::Buffer memoryA(context, CL_MEM_READ_ONLY, memorySize);
    cl::Buffer memoryB(context, CL_MEM_READ_ONLY, memorySize);
    cl::Buffer memoryC(context, CL_MEM_WRITE_ONLY, memorySize);

    try {
        queue.enqueueWriteBuffer(memoryA, CL_TRUE, 0, memorySize, dataA);
        queue.enqueueWriteBuffer(memoryB, CL_TRUE, 0, memorySize, dataB);

        kern_membasic.setArg(0, memoryC);
        kern_membasic.setArg(1, memoryA);
        kern_membasic.setArg(2, memoryB);

        queue.enqueueNDRangeKernel(kern_membasic, cl::NDRange(), cl::NDRange(vecCount), cl::NDRange(groupSize));

        queue.enqueueReadBuffer(memoryC, CL_TRUE, 0, memorySize, data);
    } catch (cl::Error error) {
           logfile << "Test Failed --- ";
           logfile << error.what() << "(" << error.err() << ")" << std::endl;
           free(data);
           free(dataA);
           free(dataB);
           return;
    }
    
    bool good = true;
    for (size_t i = 0; i < vecCount; i++) {
       const cl_float have = data[i];
       const cl_float want =  1.0f ; // (i % modulo) << shiftSize;

       if (have != want) {
           good = false;
           logfile << "    Incorrect value at " << i
                   << " (have " << have << ", want " << want << ")" << std::endl;
           return;
       }
    }
    
    //Random Mult
    
    logfile <<"Testing Random MULT operation " << std::endl;
    
    
    for (size_t i = 0; i < vecCount; i++) {
        dataA[i] =  rand() % ( UINT_MAX /2 ) ;
        dataB[i] =  rand() % ( UINT_MAX /2 ) ;
	results[i] = dataA[i] * dataB[i];   //Verify against the CPU results
    }

    
     try {
        queue.enqueueWriteBuffer(memoryA, CL_TRUE, 0, memorySize, dataA);
        queue.enqueueWriteBuffer(memoryB, CL_TRUE, 0, memorySize, dataB);

        kern_membasic.setArg(0, memoryC);
        kern_membasic.setArg(1, memoryA);
        kern_membasic.setArg(2, memoryB);

        queue.enqueueNDRangeKernel(kern_membasic, cl::NDRange(), cl::NDRange(vecCount), cl::NDRange(groupSize));

        queue.enqueueReadBuffer(memoryC, CL_TRUE, 0, memorySize, data);
    } catch (cl::Error error) {
           logfile << "Test Failed --- ";
           logfile << error.what() << "(" << error.err() << ")" << std::endl;
           free(data);
           free(dataA);
           free(dataB);
           return;
    }
    
    if(!CheckResults(data,results,vecCount,logfile))
       logfile<<"  Test Failed!"<<std::endl;
    else
       logfile<<"  Test Passed"<<std::endl;
    
    
    free(data);
    free(dataA);
    free(dataB);
}
