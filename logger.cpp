// Copyright 2011 Dmitri Nikulin.
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

#include "logger.h"

#include <iostream>

#include <boost/bind.hpp>

static void logLine(LogLevel threshold, std::string const & prefix, std::ostream * stream,
                    LogLevel level, std::string const & line) {

    if (level >= threshold)
        *stream << prefix << ": " << LOG_PREFIX[level] << ": " << line << std::endl;
}

Logger makeStreamLogger(LogLevel threshold, std::string const & prefix, std::ostream * stream) {
    return boost::bind(logLine, threshold, prefix, stream, _1, _2);
}

Logger makeSimpleLogger(LogLevel threshold, std::string const & prefix) {
    return makeStreamLogger(threshold, prefix, &std::cerr);
}
