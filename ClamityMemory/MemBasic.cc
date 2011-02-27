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

void ClamityMemory::memBasic(Clamity &subject) {
    std::ostream &logfile = subject.logfile;
    cl::Device &device = subject.device;
    cl::Context context(subject.devices);
    cl::CommandQueue queue(context, device);

    static const size_t groupSize = 256;
    static const unsigned int maxShift = 32;  // We can only shift up to 31 places

    unsigned int currShift = 0;  // How many times we have shifted already
    unsigned int shiftedVal = 1;

    size_t memSize  = device.getInfo<CL_DEVICE_GLOBAL_MEM_SIZE>();
    size_t memAlloc = device.getInfo<CL_DEVICE_MAX_MEM_ALLOC_SIZE>()/MEMORY_FRACTION;

    // Work out group size
    size_t vecCount  = memAlloc / sizeof(cl_uint);

    // Is the max alloc size a multiple of 4?
    size_t maxAllocMultiple = memSize / memAlloc;

    unsigned long memorySize = vecCount * sizeof(cl_uint);

    logfile << "Basic memory tests" << std::endl;
    logfile << "Memory Global size : " << memSize << " Max Alloc Size: "<<memAlloc <<std::endl;
    logfile << std::endl;

    if (maxAllocMultiple != 4)
       logfile << "CL_DEVICE_MAX_MEM_ALLOC_SIZE not a multiple of 4" <<std::endl;

    cl::Program program;
    subject.compile(program, "MemBasic.cl");

    logfile.flush();

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
            const cl_uint want = shiftedVal * 2 ; // (i % modulo) << shiftSize;

            if (have != want) {
                good = false;
                logfile << "Test Failed --- "<<std::endl<< "    Incorrect value at " << i
                        << " (have " << have << ", want " << want << ")" << std::endl;
                return;
            }
        }

        if (good == true)
            logfile << "Shift: " << currShift <<"    Passed" << std::endl;

        currShift++;

        for (size_t i = 0; i < vecCount; i++)
            data.at(i) = shiftedVal << currShift;

        shiftedVal <<= currShift;
    } while (currShift < maxShift);
}
