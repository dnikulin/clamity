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

#include "TestSuite.hh"

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QPluginLoader>

#include <fstream>
#include <iostream>
#include <sstream>

#include <boost/program_options.hpp>
#include <boost/asio/ip/host_name.hpp>
#include "Clamity.hh"

int main(int argc, char **argv) {
    QCoreApplication app(argc, argv);
    Q_UNUSED(app);

    const double errorEpsilonDefault = 1e-5;
    const unsigned int maxMemDivDefault = 1;
    const unsigned int defaultMemDetla = 10;
    double errorEpsilon=0;
    unsigned int maxMemDiv=0;
    unsigned int memDelta = 0;
    unsigned int maxAlloc = 0;
    unsigned int logLevel = 1;
    unsigned int maxDev = 0;
    bool skipCPUTesting = false;
    bool writeLog = false;

    std::ofstream reportfile;
    std::ostream *outstream;

    boost::program_options::options_description cmdDescription("Allowed Options");
    cmdDescription.add_options()("help","lists command line arguments and how to use clamity")
                   ("error-epsilon",boost::program_options::value<double>(&errorEpsilon)->default_value(errorEpsilonDefault),
                    "Set the error epsilon value for float calculation")
                   ("mem-delta",boost::program_options::value<unsigned int>(&memDelta)->default_value(defaultMemDetla),
                    "Sets the memory size delta, for buffer allocations")
                   ("mem-max-alloc",boost::program_options::value<unsigned int>(&maxAlloc)->default_value(0),
                    "Sets the maximum buffer size in BYTES")
                   ("mem-dev-max",boost::program_options::value<unsigned int>(&maxDev)->default_value(0),
                    "Sets the maximum global size in BYTES")
                   ("max-mem-div",boost::program_options::value<unsigned int>(&maxMemDiv)->default_value(maxMemDivDefault),
                    "Divides any buffer allocation calculation by this size useful for tests that over allocate")
                   ("skip-cpu","Skip the CPU tests")
                   ("log-tofile","writes the log output to a file rather than stdout")
                   ("log-level",boost::program_options::value<unsigned int>(&logLevel)->default_value(1),
                    "Determines the logging verbosity of clamity");

    boost::program_options::variables_map vm;
    boost::program_options::store(boost::program_options::parse_command_line(argc,argv,cmdDescription),vm);
    boost::program_options::notify(vm);

    if(vm.count("help")) {
        std::cout << "Clamity OpenCL Testing platform version 0.5.0a" <<std::endl;
        std::cout << "copyright Enzo Reyes, Dmitri Nikulin Licenced under GPLv3" <<std::endl;
        std::cout << "See the file COPYING for details" <<std::endl;
        std::cout << std::endl;
        std::cout << cmdDescription <<std::endl;
        return 0;
    }

    if(vm.count("skip-cpu")) {
        skipCPUTesting = true;
        std::cout<<"Skipping the CPU for testing"<<std::endl;
    }

    std::ofstream logfile;
    if(vm.count ("log-tofile")) {
        writeLog = true;
        std::string logfilename("clamity."+boost::asio::ip::host_name() +".log");
        logfile.open(logfilename.c_str ());
        outstream = &logfile;
    }
    else
        outstream = &std::cout;

    //Set up the report file output
    std::string filename("clamity."+boost::asio::ip::host_name() +".report");
    reportfile.open(filename.c_str());

    // Vector of all test suites found via plugins.
    TestSuites suites;

    // Load static and dynamic test suite plugins.
    loadStaticSuites(&suites);
    loadDynamicSuites(&suites);

    // Discover all OpenCL platforms
    // In general, each platform belongs to a specific vendor
    std::vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);

    try {
        for (size_t nplatform = 0; nplatform < platforms.size(); nplatform++) {
            cl::Platform platform = platforms.at(nplatform);

            // Discover all OpenCL devices of any type
            std::vector<cl::Device> devices;
            platform.getDevices(CL_DEVICE_TYPE_ALL, &devices);

            for (size_t ndevice = 0; ndevice < devices.size(); ndevice++) {
                cl::Device device = devices.at(ndevice);
                if(skipCPUTesting && device.getInfo<CL_DEVICE_TYPE>() == CL_DEVICE_TYPE_CPU)
                    continue;
                // Invoke tests in order, log to stdout
                Clamity subject(*outstream,reportfile, device,maxMemDiv,
                                errorEpsilon,memDelta, logLevel,
                                maxAlloc,maxDev);
                testDevice(subject, suites);
            }
        }
    } catch (cl::Error error) {
         std::cout << "Test Failed --- ";
         std::cout << error.what() << "(" << error.err() << ")" << std::endl;
    }

    //Close the report output
    reportfile.flush ();
    reportfile.close ();

    if(writeLog) {
        logfile.flush ();
        logfile.close ();;
    }

    return 0;
}
