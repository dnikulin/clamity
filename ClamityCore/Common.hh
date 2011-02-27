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

#ifndef CLAMITY_COMMON_HH
#define CLAMITY_COMMON_HH

// Use standard Boost headers
#include <boost/cstdint.hpp>
#include <boost/noncopyable.hpp>

// Use standard STL headers
#include <string>
#include <vector>

// Use official OpenCL C++ wrapper in exception mode
// Failed API calls will abort with cl::Error
#define __CL_ENABLE_EXCEPTIONS
#include <CL/cl.hpp>

#endif // CLAMITY_COMMON_HH
