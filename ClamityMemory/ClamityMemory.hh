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

#ifndef CLAMITY_PLUGIN_MEMORY_HH
#define CLAMITY_PLUGIN_MEMORY_HH

#include "TestSuite.hh"

class ClamityMemory : public QObject, public TestSuite {
    Q_OBJECT;
    Q_INTERFACES(TestSuite);

private:
    TestReportLevel testLevel;
    ErrorTypes errorReported;
public:

    ClamityMemory();
    virtual ~ClamityMemory();

    virtual std::string suiteName() const;
    virtual void runTests(Clamity &subject, TestLevel level);

    void testAlloc(Clamity &subject);
    bool memBasic(Clamity &subject);
    bool memBasicAnd(Clamity &subject);
};

static const char * const ClamityMemoryCL = ":/clamity/memory/ClamityMemory.cl";

#endif // CLAMITY_PLUGIN_MEMORY_HH
