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
#include <QFile>

#include <assert.h>

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
    else if(errorLevel ==  CL_DEVICE_NOT_FOUND )
        return ERROR_DEVICE_NOTFOUND;
    else {
        return ERROR_GENERAL_FAILURE;
    }
}

std::string getNoInfoString() {
    return "No additional data available";
}

std::string getErrorFromFiles(const char * path) {
    QFile file(path);
    file.open(QFile::ReadOnly);
    QByteArray error = file.readAll();
    file.close();
    return std::string(error.data());
}


std::string getErrorString(ErrorTypes errorLevel) {

    switch(errorLevel) {
        case ERROR_MEM_EXHAUSTED:
            return getErrorFromFiles(":/clamity/error/ErrorStrings/ERROR_MEM_EXHAUSTED");
            break;
        case ERROR_MEM_OBJ_BUFFER:
            return getErrorFromFiles(":/clamity/error/ErrorStrings/ERROR_MEM_OBJ_BUFFER");
            break;
        case ERROR_GENERAL_FAILURE:
            return getErrorFromFiles(":/clamity/error/ErrorStrings/ERROR_GENERAL_FAILURE");
            break;
        case ERROR_DEVICE_NOTFOUND:
            return getErrorFromFiles(":/clamity/error/ErrorStrings/ERROR_DEVICE_NOTFOUND");
            break;
        default:
            return getNoInfoString();
     }
    return getNoInfoString();
}



bool Clamity::processError(bool isError,const ErrorTypes errorClass,
                                        const TestReportLevel errLevel,
                                        std::string test) {
    testrun(test,isError);
    if(!isError) {
        if(TEST_ERROR == errLevel )
            return true;
        if(TEST_PANIC == errLevel )
        testdiag(getErrorString((errorClass)));
    }
    return false;
}
