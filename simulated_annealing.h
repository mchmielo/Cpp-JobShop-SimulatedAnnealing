#pragma once
#include "job_shop.h"
#define START_TEMP 1000

class simulated_annealing
{
private:
	job_shop prevPermutation;
	job_shop currPermutation;
	double currTemp;
	int bestCmax;
public:
	simulated_annealing();
	~simulated_annealing();
	void lowerTemp();
	bool compareCmaxes(int, int);
	int getBestCmax();
	job_shop &getCurrPermutation();
	void preparePermutation();
	void mainAlgorithm();
};

