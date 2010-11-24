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

void Clamity::memBasic() {
    //static const size_t vecCount = 8192;
    static const size_t vecSize = vecCount * sizeof(cl_uint);
    static const size_t groupSize = 256;


    size_t memSize  = device.getInfo(CL_DEVICE_GLOBAL_MEM_SIZE);
    size_t memAlloc = device.getInfo(CL_DEVICE_MAX_MEM_ALLOC_SIZE);

    //Work out group size
    size_t vecCount  = memAlloc / sizeof(cl_uint);
  
    //Is the max alloc size a multiple of 4?
    size_t maxAllocMultiple = memSize / memAlloc;

    if(maxAllocMultiple!=4)
    {
       logfile << " maxAllocMultiple not a multiple of 4" <<std::endl;
    }


    logfile << "Basic memory tests" << std::endl;
    logfile << "Memory Global size : " << memSize << " Max Alloc Size: "<<memAlloc <<std::endl;
    logfile << std::endl;

    cl::Program program;
    compile(program, "basic.cl");

    logfile.flush();

    cl::Kernel kern_membasic(program, "testShiftCL");

    std::vector<cl_uint> data(groupSize);
    for (size_t i = 0; i < groupSize; i++)
        data.at(i) = 1;

    cl::Buffer memoryOne(context, CL_MEM_READ_WRITE, vecCount * sizeof(cl_uint));
    cl::Buffer memoryTwo(context, CL_MEM_READ_WRITE, vecCount * sizeof(cl_uint));
    cl::Buffer memoryThree(context, CL_MEM_READ_WRITE, vecCount * sizeof(cl_uint));
    queue.enqueueWriteBuffer(memoryOne, CL_TRUE, 0, vecCount * sizeof(cl_uint), data.data());
    queue.enqueueWriteBuffer(memoryTwo, CL_TRUE, 0, vecCount * sizeof(cl_uint), data.data());
    queue.enqueueWriteBuffer(memoryThree, CL_TRUE, 0, vecCount * sizeof(cl_uint), data.data());

    kern_membasic.setArg(0, memoryOne);
    kern_membasic.setArg(1, memoryTwo);
    kern_membasic.setArg(2, memoryThree);

    queue.enqueueNDRangeKernel(kern_membasic, cl::NDRange(), cl::NDRange(vecCount), cl::NDRange(groupSize));

    queue.enqueueReadBuffer(memory, CL_TRUE, 0, vecSize, data.data());

    bool good = true;

    for (size_t i = 0; i < vecCount; i++) {
        const cl_uint have = data.at(i);
        const cl_uint want =  2; // (i % modulo) << shiftSize;

        if (have != want ) {
            good = false;
            logfile << "    Incorrect value at " << i
                    << " (have " << have << ", want " << want << ")" << std::endl;
        }
    }

    if (good == true)
        logfile << "    Passed" << std::endl;
}
