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

void ClamityMath::testBasic(Clamity &subject) {
    Logger &log = subject.log;
    cl::Device &device = subject.device;
    cl::Context &context(subject.context);
    cl::CommandQueue queue(context, device);

    using boost::format;
    using boost::str;

    static const size_t vecCount = 8192;
    static const size_t vecSize = vecCount * sizeof(cl_uint);
    static const cl_uint modulo = 1337;
    static const cl_uint shiftSize = 7;

    log(LOG_INFO, "Basic sanity tests");

    cl::Program program;
    subject.compile(program, ClamityMathCL);

    log(LOG_INFO, "  Unsigned shift test");

    cl::Kernel kern_shift(program, "testShiftCL");

    std::vector<cl_uint> data(vecCount);
    for (size_t i = 0; i < vecCount; i++)
        data.at(i) = (i % modulo);

    cl::Buffer memory(context, CL_MEM_READ_WRITE, vecSize);
    queue.enqueueWriteBuffer(memory, CL_TRUE, 0, vecSize, data.data());

    kern_shift.setArg(0, memory);
    kern_shift.setArg(1, shiftSize);

    queue.enqueueNDRangeKernel(kern_shift, cl::NullRange, cl::NDRange(vecCount), cl::NullRange);

    queue.enqueueReadBuffer(memory, CL_TRUE, 0, vecSize, data.data());

    bool good = true;

    for (size_t i = 0; i < vecCount; i++) {
        const cl_uint have = data.at(i);
        const cl_uint want = (i % modulo) << shiftSize;

        if (have != want) {
            good = false;
            log(LOG_ERROR, str(format("    Incorrect at offset %d (have: %d want: %d) ") % i % have % want));
            testPassed = false;
            testLevel = TEST_ERROR;
        }
    }

    if (good == true)
        log(LOG_INFO,"    Passed");
    testPassed = true;
}
