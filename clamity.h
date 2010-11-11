// Copyright 2010 Dmitri Nikulin.
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

#ifndef CLAMITY_H
#define CLAMITY_H

// Use official OpenCL C++ wrapper in exception mode
// Failed API calls will abort with cl::Error
#define __CL_ENABLE_EXCEPTIONS
#include <CL/cl.hpp>

#include <iomanip>
#include <ostream>
#include <string>
#include <vector>

// Structure to bind together common objects during testing
struct Clamity {
    std::ostream               &logfile;

    cl::Platform                platform;
    cl::Device                  device;
    std::vector<cl::Device>     devices;
    cl::Context                 context;
    cl::CommandQueue            queue;

    // main.cpp
    Clamity(std::ostream &logfile, cl::Device &device);
    void testDevice();

    // info.cpp
    void logInfo();

    // alloc.cpp
    void testAlloc();

    // basic.cpp
    void testBasic();

    // tools.cpp
    void reportCompile(cl::Program &program);
    void compile(cl::Program &program, const char *path);
};

// tools.cpp
std::string readFile(const char *path);

#endif // CLAMITY_H
