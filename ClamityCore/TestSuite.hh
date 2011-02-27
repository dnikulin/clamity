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

#ifndef CLAMITY_TEST_SUITE_HH
#define CLAMITY_TEST_SUITE_HH

#include "Clamity.hh"

#include <QtPlugin>
#include <QVector>

enum TestLevel {
    CORE    = 1,
    EXTRA   = 2,
    NICHE   = 3
};

struct TestLevelData {
    TestLevel    level;
    const char * name;
};

static TestLevelData TestLevels[] = {
    {CORE,  "core"},
    {EXTRA, "extra"},
    {NICHE, "niche"}
};

static const size_t TestLevelCount =
        sizeof(TestLevels) / sizeof(TestLevels[0]);

class TestSuite : public boost::noncopyable {
public:

    TestSuite() {}
    virtual ~TestSuite() {}

    virtual std::string suiteName() const = 0;
    virtual void runTests(Clamity &subject, TestLevel level) = 0;
};

Q_DECLARE_INTERFACE(TestSuite, "org.clamity.TestSuite/1.0");

typedef QVector<TestSuite *> TestSuites;

#endif // CLAMITY_TEST_SUITE_HH
