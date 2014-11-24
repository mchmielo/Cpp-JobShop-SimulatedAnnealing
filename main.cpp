#include <iostream>
#include <cstdlib>
#include <ctime>
#include "job_shop.h"


using namespace std;

int main()
{
	srand(time(NULL));
    job_shop *js = new job_shop();
    js->readFile();
    js->logClass();
    js->createSchedule();
	js->createCPath();
	js->createBlocks();
	js->logClass();
	js->swapBlocks();
    js->logClass();
	system("pause");
    return 0;
}

