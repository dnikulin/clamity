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

#include "MemoryTools.hh"
#include "ReportService.hh"



// Structure to bind together common objects during testing
class Clamity : public boost::noncopyable {

private:

    std::ofstream               &reportfile;
    MemoryToolsResources        memoryInfo;
    unsigned int                maxalloc;
    unsigned int                maxmem;

public:

    std::ostream               &logfile;
    Logger                      log;

    TestResult                  testrun;
    ReportLine                  testdiag;

    cl::Platform                platform;
    cl::Device                  device;
    std::vector<cl::Device>     devices;
    cl::Context                 context;


    unsigned int                memoryPoolFraction;
    double                      epsilonErrorMargin;
    unsigned int                memorySizeDelta;

    // Main.cc
    Clamity(std::ostream &logfile,std::ofstream &reportfile,
            cl::Device &device,unsigned int _maxDiv,
            double _epsilon, unsigned int _memDelta,
            unsigned int _loglevel, unsigned int _maxalloc,
            unsigned int _maxmem);

    // Info.cc
    void logInfo();

    // Tools.cc
    void reportCompile(cl::Program &program);
    void compile(cl::Program &program, const char *path);
    unsigned int recommendMemory(unsigned int deviceAllow,
                                 unsigned int maxGlobal,
                                 unsigned int numberOfBuffers);
    unsigned int numOfBuffers(cl::Device device);
    unsigned int maxMemoryAllocation(cl::Device device);
    unsigned int deviceMemoryAvail(cl::Device device);
};

// Tools.cc
std::string readFile(const char *path);

#endif // CLAMITY_HH
