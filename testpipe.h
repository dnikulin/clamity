#ifndef TESTPIPE_H
#define TESTPIPE_H


#include <vector>
#include <map>
#include "TestTemplate.h"  //empty template class that needs to be extended later on for plugins etc.
using namespace std;

enum CLAMITY_TEST_PRIORITY {CLAMITY_TEST_HIGH, CLAMITY_TEST_MEDIUM, CLAMITY_TEST_LOW};

class TestPipe
{
private:
    vector <TestTemplate *> testPipeLine;
    vector <TestTemplate *> highPriorityQueue;
    vector <TestTemplate *> mediumPriorityQueue;
    vector <TestTemplate *> lowPriorityQueue;
	map <char *, TestTemplate *> namedTests;

public:
    TestPipe();
    unsigned int RunTests();  //if return is non zero indicates the test that failed
    bool AddTestToPlan(unsigned int index, char * testName);
    void ClearPipe();
    void InitRandomTests(unsigned int);
    bool AddTestToSuite(char *, CLAMITY_TEST_PRIORITY,TestTemplate *);
};

#endif // TESTPIPE_H
