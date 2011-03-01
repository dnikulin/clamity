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

#include "ClamityMemory.hh"

#include <iomanip>

void ClamityMemory::testAlloc(Clamity &subject) {

    using boost::format;
    using boost::str;

    Logger &log = subject.log;
    cl::Device &device = subject.device;
    cl::Context &context(subject.context);
    cl::CommandQueue queue(context, device);

    log(LOG_INFO,"Memory allocation");

    size_t size = 8;

    try {
        while (size > 0) {
            cl::Buffer memory(context, CL_MEM_READ_WRITE, size);
            queue.enqueueWriteBuffer(memory, CL_TRUE, 0, sizeof(size), &size);
            queue.enqueueWriteBuffer(memory, CL_TRUE, size - sizeof(size), sizeof(size), &size);

            log(LOG_INFO,str(format("  Allocated and tested edges of '%d' bytes") % size ));

            // Double allocation size
            size <<= 1;
        }
    } catch (cl::Error error) {
        log(LOG_ERROR,str(format("  Failed at %d     bytes") % size ));
    }
}
