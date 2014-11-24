#include <iostream>
#include <cstdlib>
#include <ctime>
#include "simulated_annealing.h"


using namespace std;

int main()
{
	srand(time(NULL));
	simulated_annealing *sa = new simulated_annealing();
	sa->mainAlgorithm();
	cout << sa->getBestCmax() << endl;

	sa->getCurrPermutation().logClass();
	system("pause");
    return 0;
}

