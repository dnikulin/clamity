#include "testpipe.h"

TestPipe::TestPipe()
{
	testPipeLine.clear();
}

unsigned int TestPipe::RunTests()
{
	if(testPipeLine.size()==0)
		InitRandomTests(10);
	int counter = 1;
	for(vector<TestTemplate*>::iterator it = testPipeLine.begin(); it!=testPipeLine.end(); ++it)
	{
		TestTemplate * test = *it;
		if(!test->RunTest())
			return counter;
		++counter;
	}
	return 0;
}

bool TestPipe::AddTestToPlan(unsigned int index, char * testName)
{
	TestTemplate * test = namedTests[testName];
	vector<TestTemplate *>::iterator it = testPipeLine.begin();
	testPipeLine.insert(it + index,test);

	return true;
}

void TestPipe::ClearPipe()
{
	testPipeLine.clear();
}

void TestPipe::InitRandomTests(unsigned int numOfTests)
{
	bool pipeFull=false;
	while(pipeFull!=true)
	{
		for(vector<TestTemplate *>::iterator it = highPriorityQueue.begin(); it!=highPriorityQueue.end(); ++it)
		{
			if(testPipeLine.size()==numOfTests)
			{
				pipeFull = true;
				break;
			}
			TestTemplate * tmp = * it;
			testPipeLine.push_back(tmp);
		}
		for(vector<TestTemplate *>::iterator it = mediumPriorityQueue.begin(); it!=mediumPriorityQueue.end(); ++it)
		{
			if(testPipeLine.size()==numOfTests)
			{
				pipeFull = true;
				break;
			}
			TestTemplate * tmp = * it;
			testPipeLine.push_back(tmp);
		}
		for(vector<TestTemplate *>::iterator it = lowPriorityQueue.begin(); it!=lowPriorityQueue.end(); ++it)
		{
			if(testPipeLine.size()==numOfTests)
			{
				pipeFull = true;
				break;
			}
			TestTemplate * tmp = * it;
			testPipeLine.push_back(tmp);
		}
	}
}


bool TestPipe::AddTestToSuite(char *testName, CLAMITY_TEST_PRIORITY queue, TestTemplate * test)
{
    switch(queue)
    {

		case CLAMITY_TEST_HIGH:
			 this->highPriorityQueue.push_back(test);
		     break;
		case CLAMITY_TEST_MEDIUM:
			 this->mediumPriorityQueue.push_back(test);
        	 break;
		case CLAMITY_TEST_LOW:
			 this->lowPriorityQueue.push_back(test);
			 break;
		default:
			return false;
    }

	this->namedTests[testName] = test;

	return true;
}
