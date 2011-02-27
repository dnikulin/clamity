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

#include "Clamity.hh"

#include <fstream>
#include <sstream>

std::string readFile(const char *path) {
    char buffer[4096];

    std::fstream in;
    in.open(path);

    std::ostringstream out;

    while (true) {
        std::streamsize count = in.readsome(buffer, sizeof(buffer));

        if (count < 1)
            break;

        out.write(buffer, count);
    }

    return out.str();
}

void Clamity::reportCompile(cl::Program &program) {
    std::string buildlog = program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device);

    if (buildlog.size() > 0) {
        logfile << buildlog << std::endl;
        logfile << std::endl;
        logfile.flush();
    }
}

void Clamity::compile(cl::Program &program, const char *path) {
    std::string source = readFile(path);

    cl::Program::Sources sources(1, std::make_pair(source.c_str(), source.size()));
    program = cl::Program(context, sources);

    try {
        logfile << "Compiling '" << path << "', " << source.size() << " bytes" << std::endl;
        program.build(devices, "");

        // Save compile log even if successful,
        // as it may have warnings
        reportCompile(program);
    } catch (cl::Error error) {
        // Save compile log if there was a build exception
        if (error.err() == CL_BUILD_PROGRAM_FAILURE)
            reportCompile(program);

        // Re-throw the error anyway,
        // so that the whole test fails
        throw error;
    }
}
