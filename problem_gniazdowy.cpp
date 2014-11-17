
#include "problem_gniazdowy.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>

using namespace std;

problem_gniazdowy::problem_gniazdowy()
{
    pi = NULL;
    ti = NULL;
    ps = NULL;
    czasi = NULL;
    ci = NULL;
    lp = NULL;
    T = NULL;
	ph = NULL;
    n = 0;
    m = 0;
    z = 0;
}

problem_gniazdowy::~problem_gniazdowy()
{
    if( pi != NULL){
        delete pi;
    }
    if( ti != NULL){
        delete ti;
    }
    if( czasi != NULL){
        delete czasi;
    }
    if( ps != NULL){
        delete ps;
    }
    if( lp != NULL){
        delete lp;
    }
    if( ci != NULL){
        delete ci;
    }
    if( T != NULL){
        delete T;
    }
	if (ph != NULL){
		delete ph;
	}
}

bool problem_gniazdowy::readFile(string file)
{
    fstream inFile;
    inFile.open(file.c_str(), ios::in);
    if(!inFile.is_open()){
        return false;
    }

    int i = 0;
    int oper = 0;
    int *mPos = NULL;           //tabela zapamiêtuje obecna pozycjê na ka¿dej maszynie
    while( !inFile.eof() ){
        if(i == 0){
            inFile >> this->z;      // wczytanie liczby zadan
            inFile >> this->m;      // wczytanie liczby maszyn
            createPi();             // zaalokowanie pamieci na podstawie liczby zadan i maszyn
            mPos = new int[this->m];    // zaalokowanie pamieci w tabeli pomocniczej i wyzerowanie jej
            for(int j = 0; j < this->m; ++j){
                mPos[j] = 0;
            }
        }
        else{
            if(oper < this->n){
                int mNumber;
                inFile >> mNumber;      // wczytanie nr maszyny
                int pos = (mNumber)*this->z + mNumber+1 + mPos[mNumber];    // obliczenie pozycji w tabeli pi
                this->pi[pos] = oper+1; // wype³nienie tabeli pi
                this->ps[oper+1] = pos;
                mPos[mNumber]++;        // inkrementacja obecnej pozycji na danej maszyny
                inFile >> this->czasi[oper+1];  // wczytanie czasu kolejnej operacji
                oper++;
            }
            else{                       // wczytywanie "nadmiaru" z pliku
                int tmp;
                inFile >> tmp;
            }
        }
        i++;
    }
    makeTi();
    makeLp();
    if(mPos != NULL){
        delete mPos;
    }
    inFile.close();
    return true;

}

void problem_gniazdowy::createPi()
{
    this->n = this->m * this->z;
    this->pi = new int[this->n+this->m+1];
    this->ti = new int[this->n+1];
    this->ps = new int[this->n+1];
    this->czasi = new int[this->n+1];
    this->lp = new int[this->n+1];
    this->ci = new int[this->n+1];
    this->T = new int[this->n+1];
	this->ph = new int[this->n + 1];
    for(int i = 0; i < this->n+this->m+1 ; ++i){
        this->pi[i] = 0;
    }
    for(int i = 0; i <= this->n; ++i){
        this->ti[i] = 0;
        this->lp[i] = 0;
        this->ci[i] = 0;
        this->T[i] = 0;
		this->ph[i] = 0;
    }
    this->ps[0] = 0;
    this->czasi[0] = 0;
}

void problem_gniazdowy::makeTi()
{
    for(int i = 1; i <= this->n; ++i){
        if((i-1)%this->m != 0){
            this->ti[i-1] = this->pi[this->ps[i]];
        }
        if((i)%this->m != 0){
            this->T[i] = i;
        }
    }
}

void problem_gniazdowy::makeLp()
{
    int indexes = -1;						// indeksy ktore nalezy w kolejnej petli inkrementowac
    for(int i = this->n; i > 0; --i){		// iteracja od tylu po nastêpnikach technologicznych
        /* nastêpniki technologiczne */
        if(indexes > 0){
            this->lp[indexes]++;
        }
        if(this->ti[i-1] != 0){
            indexes = this->ti[i-1];
        }
        else indexes = -1;

        /* nastêpniki maszynowe */

        if(this->pi[this->ps[i]-1] != 0){
            lp[i]++;            //jeœli element przed danym nie jest 0 to ma poprzednika
        }
    }
    if(indexes > 0){
        this->lp[indexes]++;
    }
}

void problem_gniazdowy::logClass()
{
    cout << endl << "pi: " << endl;
    for(int j = 0; j < this->n+this->m+1; ++j){
        if(this->pi[j] == 0)
            cout << this->pi[j] << endl;
        else cout << this->pi[j] << " ";
    }
    cout << endl << "ps: " << endl;
    for(int j = 0; j <= this->n; ++j){
        cout << this->ps[j] << " ";
    }
    cout << endl << "ti: " << endl;
    for(int j = 0; j < this->n; ++j){
        cout << this->ti[j] << " ";
    }
    cout << endl << "czasi: " << endl;
    for(int j = 0; j <= this->n; ++j){
        cout << this->czasi[j] << " ";
    }
    cout << endl << "lp: " << endl;
    for(int j = 0; j <= this->n; ++j){
        cout << this->lp[j] << " ";
    }
    cout << endl << "ci:" << endl;
    for(int j = 0; j <= this->n; ++j){
        cout << this->ci[j] << " ";
    }
	cout << endl << "ph:" << endl;
	for (int j = 0; j <= this->n; ++j){
		cout << this->ph[j] << " ";
	}
}
void problem_gniazdowy::evalPH(int i){
	ph[i] = (ci[this->pi[ps[i] - 1]] > ci[T[i - 1]]) ? this->pi[ps[i] - 1] : T[i - 1];
}

void problem_gniazdowy::evalCi(int i)
{
    ci[i] = max(ci[this->pi[ps[i]-1]], ci[T[i-1]]) + czasi[i];      // poprzednik maszynowy
}

void problem_gniazdowy::createSchedule()
{
    queue<int> Q;			// kolejka z gotowymi operacjami
    for(int i = 1; i <= this->n; ++i){
        if(lp[i] == 0){
            Q.push(i);
        }
    }
    while(!Q.empty()){
        int i = Q.front(), ns, count = 0;
        evalCi(i);
		evalPH(i);
        Q.pop();
        ns = ti[i];		//nastepnik technologiczny
        do{
            if(ns != 0){        
                lp[ns]--;
                if(lp[ns] == 0)
                    Q.push(ns);        
            }
			ns = pi[ps[i] + 1];
            count++;
        }while(count < 2);	// trzeba wszystko powtorzyc dla nastepnika maszynowego
    }
}

