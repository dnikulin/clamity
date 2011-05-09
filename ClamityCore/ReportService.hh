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

#ifndef REPORTSERVICE_HH
#define REPORTSERVICE_HH

#include "Common.hh"

#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <boost/function.hpp>

enum TestReportLevel {
    TEST_ERROR,    //Normal Error
    TEST_OK,       //Test passed OK
    TEST_PANIC     //Test internal failure
};

enum ErrorTypes {
    ERROR_MEM_EXHAUSTED = -5,  //The memory on the system has run out
    ERROR_MEM_OBJ_BUFFER = -4,     //Out of memory for Objects
    ERROR_MEM_HOST_EXCHAUSTED = -6,  //The memory of the host has exhausted
    ERROR_COMPILER_LOCATION = -3,  //The Compiler could not be allocated
    ERROR_GENERAL_FAILURE //We dont know what error this is
};


ErrorTypes getErrorType(int errorLevel);
std::string getErrorString(ErrorTypes errorLevel);

typedef boost::function<
    void (std::string const & /* Test */ , bool const & /* T/F */)
> TestResult;

typedef boost::function<
        void(std::string const & /* problem description */)
> ReportLine;

TestResult makeTestOutput(std::ofstream * stream);
ReportLine makeLineOutout(std::ofstream * stream);
#endif // REPORTSERVICE_HH
