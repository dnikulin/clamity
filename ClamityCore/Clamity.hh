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

#ifndef CLAMITY_HH
#define CLAMITY_HH

#include "Logger.hh"

#define MEMORY_FRACTION 1

// Structure to bind together common objects during testing
class Clamity : public boost::noncopyable {
public:

    std::ostream               &logfile;
    Logger                      log;

    cl::Platform                platform;
    cl::Device                  device;
    std::vector<cl::Device>     devices;
    cl::Context                 context;
    cl::CommandQueue            queue;

    // Main.cc
    Clamity(std::ostream &logfile, cl::Device &device);

    // Info.cc
    void logInfo();

    // Tools.cc
    void reportCompile(cl::Program &program);
    void compile(cl::Program &program, const char *path);
};

// Tools.cc
std::string readFile(const char *path);

#endif // CLAMITY_HH
