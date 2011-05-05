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


#ifndef MEMORYTOOLS_HH
#define MEMORYTOOLS_HH

#include "Clamity.hh"
#define MEMORY_FRACTION 1

typedef struct __MemoryToolResource {

    cl::Device                  device;
    cl::Context                 context;

    unsigned int                maxMemory;
    unsigned int                numOfBuffers;

}MemoryToolsResources;

#endif // MEMORYTOOLS_HH
