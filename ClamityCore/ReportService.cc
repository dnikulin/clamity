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

