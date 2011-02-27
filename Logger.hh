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

#ifndef CLAMITY_LOGGER_H
#define CLAMITY_LOGGER_H

#include "Common.hh"

#include <ostream>

#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <boost/function.hpp>

enum LogLevel {
    LOG_DEBUG       = 0,
    LOG_INFO        = 1,
    LOG_WARN        = 2,
    LOG_ERROR       = 3,
    LOG_CRITICAL    = 4,
    LOG_ALERT       = 5,
    LOG_PANIC       = 6
};

static const char * const LOG_PREFIX[] = {
    "DEBUG",
    "INFO",
    "WARN",
    "ERROR",
    "CRITICAL",
    "ALERT",
    "PANIC"
};

typedef boost::function<
    void (LogLevel /* level */, std::string const & /* line */)
> Logger;

Logger makeSimpleLogger(LogLevel threshold, std::string const & prefix);
Logger makeStreamLogger(LogLevel threshold, std::string const & prefix, std::ostream * stream);

#endif // CLAMITY_LOGGER_H
