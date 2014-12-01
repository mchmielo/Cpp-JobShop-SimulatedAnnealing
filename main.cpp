#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>
#include <fstream>
#include "simulated_annealing.h"

using namespace std;

vector<string> readFileNames(string file){
	vector<string> fileNames;
	string fileName;
	fstream inFile;
	int count = 0, fileNumber;
	inFile.open(file.c_str(), ios::in);
	if (!inFile.is_open()){
		return fileNames;
	}
	while (!inFile.eof()){
		if (count){
			inFile >> fileNumber;
		}
		else{
			inFile >> fileName;
			fileNames.push_back(fileName);
		}
	}
	inFile.close();
	return fileNames;
}

int main()
{
	simulated_annealing *sa = NULL;
	srand(time(NULL));
	vector<string> fileNames = readFileNames("Data/names.txt");
	if (!fileNames.empty()){
		for (int i = 0; i < fileNames.size(); ++i){
			if (i){
				string fName = "Data/";
				fName += fileNames[i];
				sa = new simulated_annealing();
				sa->getCurrPermutation().~job_shop();
				sa->getCurrPermutation().readFile(fName);
				sa->mainAlgorithm();
				cout << fName << ", Cmax = " << sa->getBestCmax() << endl;
			}
		}
	}
	if (sa != NULL){
		delete sa;
	}

	//sa->getCurrPermutation().logClass();
	system("pause");
    return 0;
}

