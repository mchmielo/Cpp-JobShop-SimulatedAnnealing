#include <iostream>
#include "problem_gniazdowy.h"

using namespace std;

int main()
{
    problem_gniazdowy *pg = new problem_gniazdowy();
    pg->readFile();
    pg->logClass();
    pg->createSchedule();
	pg->createCPath();
	pg->createBlocks();
    pg->logClass();
	system("pause");
    return 0;
}

