#include "simulated_annealing.h"

simulated_annealing::simulated_annealing()
{
	currTemp = 100;
	bestCmax = INFINITY;
}

simulated_annealing::~simulated_annealing()
{
}

void simulated_annealing::lowerTemp(){
	currTemp = currTemp*0.999;
}
bool simulated_annealing::compareCmaxes(int prevCmax, int currCmax){
	double rnd = static_cast<double>(rand()%100);
	if ((currCmax > prevCmax) || (rnd/100.0 < (exp(-(currCmax - prevCmax)) / currTemp)) )
		return true;
	return false;
}

int simulated_annealing::getBestCmax(){
	return bestCmax;
}

job_shop &simulated_annealing::getCurrPermutation(){
	return currPermutation;
}

void simulated_annealing::preparePermutation(){
	currPermutation.readFile();
	currPermutation.createSchedule();
	currPermutation.createCPath();
	currPermutation.createBlocks();
	bestCmax = currPermutation.getCmax();
}

void simulated_annealing::mainAlgorithm(){
	preparePermutation();
	while (currTemp > 0.1){
		prevPermutation = currPermutation;
		currPermutation.swapBlocks();
		currPermutation.createSchedule();
		currPermutation.findMaxCi();
		if (compareCmaxes(prevPermutation.getCmax(), currPermutation.getCmax())){
			bestCmax = currPermutation.getCmax();
			currPermutation.createCPath();
			currPermutation.createBlocks();
		}
		else{
			currPermutation = prevPermutation;
		}
		lowerTemp();
	}
}

