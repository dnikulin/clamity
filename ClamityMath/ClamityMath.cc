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

#include "ClamityMath.hh"

Q_EXPORT_PLUGIN2(ClamityMathPlugin, ClamityMath);

ClamityMath::ClamityMath() {
    errorReported = ERROR_NONE_INIT;
    testLevel = TEST_ERROR;
}

ClamityMath::~ClamityMath() {}

std::string ClamityMath::suiteName() const {
    return "clamity math";
}

void ClamityMath::runTests(Clamity &subject, TestLevel level) {
    switch (level) {
    case CORE:
        testBasic(subject);
        //basicALU(subject);  -For the moment this test causes crashes
        basicFMAD(subject);
        basicADD(subject);
        basicMULT(subject);
        break;

    case EXTRA:
        break;

    case NICHE:
        break;
    }
}
