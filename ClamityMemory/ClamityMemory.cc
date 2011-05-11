// Copyright 2010-2011 Dmitri Nikulin, Enzo Reyes.
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

Q_EXPORT_PLUGIN2(ClamityMemoryPlugin, ClamityMemory);

ClamityMemory::ClamityMemory() { errorReported = ERROR_NONE_INIT;
                                 testLevel = TEST_ERROR;
                             }
ClamityMemory::~ClamityMemory() {}

std::string ClamityMemory::suiteName() const {
    return "clamity memory";
}

void ClamityMemory::runTests(Clamity &subject, TestLevel level) {

    switch (level) {
    case CORE:

        testAlloc(subject);

        if(subject.processError (memBasic(subject),errorReported,
                                 testLevel,"Memory Basic")) {
            //Ok We got here because Clamity is asking for more
            //details
            subject.testdiag("See Log file fore more details");
        }

        if(subject.processError (memBasicAnd(subject),errorReported,
                                 testLevel,"Memory AND tests")) {
            subject.testdiag("See Log file for more details");
        }





        break;

    case EXTRA:
        break;

    case NICHE:
        break;
    }
}
