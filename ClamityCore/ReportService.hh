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


typedef boost::function<
    void (std::string const & /* Test */ , bool const & /* T/F */)
> TestResult;

typedef boost::function<
        void(std::string const & /* problem description */)
> ReportLine;

TestResult makeTestOutput(std::ofstream * stream);
ReportLine makeLineOutout(std::ofstream * stream);
#endif // REPORTSERVICE_HH
