// Copyright 2011 Enzo Reyes.
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
#include "ReportService.hh"
#include <boost/bind.hpp>

static void reportLine(std::ofstream * stream,
                       std::string const & message) {
    *stream << message << std::endl;
}

static void reportTest(std::ofstream * stream,
                       std::string const & message,
                       bool success)
{
    using boost::format;
    using boost::str;

    if(success)
        reportLine(stream, str(format("%s .........[passed]") % message));
    else
        reportLine(stream, str(format("%s .........[failed]") % message));
}

TestResult makeTestOutput(std::ofstream * stream) {
    return boost::bind(reportTest,stream, _1, _2);
}

ReportLine makeLineOutout(std::ofstream * stream) {
    return boost::bind(reportLine,stream, _1);
}

ErrorTypes getErrorType(int errorLevel) {

    if(errorLevel==CL_OUT_OF_RESOURCES )
        return ERROR_MEM_EXHAUSTED;
    else if(errorLevel==CL_MEM_OBJECT_ALLOCATION_FAILURE )
        return ERROR_MEM_OBJ_BUFFER;
    else
        return ERROR_GENERAL_FAILURE;
}

std::string getErrorString(ErrorTypes errorLevel) {

    switch(errorLevel) {
        case ERROR_MEM_EXHAUSTED:
            return "The memory on the test has been exhausted, \
                    This might be due to the amount of memory available in the \
                    system has been decreased by other applications";
        case ERROR_MEM_OBJ_BUFFER:
            return "The Object buffers on the device are full, this indicates \
                    System usage of the device (COMPOSITING MANAGERS)";
        case ERROR_GENERAL_FAILURE:
            return "The test has failed due to unknown circumstances please see \
                    Log file for details";
     }

}
