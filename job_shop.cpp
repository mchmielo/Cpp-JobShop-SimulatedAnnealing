
#include "job_shop.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>

using namespace std;

job_shop::job_shop()
{
    pi = NULL;
    ti = NULL;
    ps = NULL;
    czasi = NULL;
    ci = NULL;
    lp = NULL;
    T = NULL;
	ph = NULL;
	cPath = NULL;
	cPathColor = NULL;
	cPathIndexes = NULL;
    n = 0;
    m = 0;
    z = 0;
	blockSwaps = 0;
	cMax = 0;
}

job_shop::~job_shop()
{
    if( pi != NULL){
        delete pi;
		pi = NULL;
    }
    if( ti != NULL){
        delete ti;
		ti = NULL;
    }
    if( czasi != NULL){
        delete czasi;
		czasi = NULL;
    }
    if( ps != NULL){
        delete ps;
		ps = NULL;
    }
    if( lp != NULL){
        delete lp;
		lp = NULL;
    }
    if( ci != NULL){
        delete ci;
		ci = NULL;
    }
    if( T != NULL){
        delete T;
		T = NULL;
    }
	if (ph != NULL){
		delete ph;
		ph = NULL;
	}
	if (cPath != NULL){
		delete cPath;
		cPath = NULL;
	}
	if (cPathColor != NULL){
		delete cPathColor;
		cPathColor = NULL;
	}
	if (cPathIndexes!= NULL){
		delete cPathIndexes;
		cPathIndexes = NULL;
	}
}

bool job_shop::readFile(string file)
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

void job_shop::createPi()
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

void job_shop::makeTi()
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

void job_shop::makeLp()
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

void job_shop::logClass()
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
void job_shop::evalPH(int i){
	ph[i] = (ci[this->pi[ps[i] - 1]] > ci[T[i - 1]]) ? this->pi[ps[i] - 1] : T[i - 1];
}

void job_shop::evalCi(int i)
{
    ci[i] = max(ci[this->pi[ps[i]-1]], ci[T[i-1]]) + czasi[i];      // poprzednik maszynowy
}

void job_shop::createSchedule()
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
int job_shop::findMaxCi(){
	int max = 0, max_i = 0;
	for (int i = 0; i <= this->n; ++i){
		if (ci[i] > max){
			max = ci[i];
			max_i =i;
		}
	}
	cMax = max;
	return max_i;
}

void job_shop::createCPath(){
	int count = 1, maxCi = findMaxCi(), tmp;
	tmp = maxCi;
	while (ph[tmp] != 0){				// zliczenie elementow w œcie¿ce krytycznej
		tmp = ph[tmp];
		count++;
	}
	if (this->cPath != NULL)
		delete this->cPath;
	if (this->cPathColor != NULL)
		delete this->cPathColor;
											// alokacja potrzebnej pamieci dla sciezki krytycznej
	this->cPath = new int[count+4];			// w tym element 0 jest liczb¹ elmentów sk, element drugi i ostatni s¹ zerami
	this->cPathColor = new int[count];		// i blokow
	for (int i = 0; i < count; ++i){
		cPathColor[i] = 0;
	}
	cPath[0] = count;
	cPath[1] = 0;
	cPath[count + 2] = 0;
	cPath[count + 1] = maxCi;
	while (ph[cPath[count + 1]] != 0 && count > 0){		// wyznaczanie œcie¿ki krytycznej
		cPath[count] = ph[cPath[count + 1]];
		count--;
	}
}

void job_shop::createBlocks(){
	int index = 0;
	blockSwaps = 0;
	for (int i = 2; i < (cPath[0] + 2); ++i){

	//nie jest ostatni w œcie¿ce   //poprzednik w œcie¿ce jest tech.	//nastêpnik w œcie¿ce jest maszynowy
		if ((cPath[i + 1] != 0) && (cPath[i - 1] == T[cPath[i] - 1]) && (cPath[i + 1] == pi[ps[cPath[i]] + 1])){		// pocz¹tek bloku
			cPathColor[i - 2] = 1;
			blockSwaps++;
		}
	//nie jest pierwszy w œcie¿ce   //nastêpnik w œcie¿ce jest tech.	//poprzednik w œcie¿ce jest maszynowy
		if ((cPath[i - 1] != 0) && (cPath[i + 1] == ti[cPath[i]]) && (cPath[i - 1] == pi[ps[cPath[i]] - 1])){		// koniec bloku
			cPathColor[i - 2] = -1;
			blockSwaps++;
		}
	}
	if (cPathIndexes != NULL){
		delete cPathIndexes;
	}
	cPathIndexes = new int[blockSwaps];
	for (int i = 0; i < cPath[0]; ++i){
		if (cPathColor[i] != 0)
			cPathIndexes[index++] = i;
	}
}

void job_shop::swapBlocks(){
	int rnd = (rand() % blockSwaps ) + 0;
	int tmp, tmp1, tmp2;
	tmp1 = cPath[cPathIndexes[rnd] + 2];						// losowy element do zamiany
	tmp2 = cPath[cPathIndexes[rnd] + 2 + cPathColor[cPathIndexes[rnd]]];	// nastêpny (pocz¹tek bloku) lub poprzedni (koniec bloku) element
	tmp = pi[ps[tmp1]];							// zamiana elementów w permutacji
	pi[ps[tmp1]] = pi[ps[tmp2]];
	pi[ps[tmp2]] = tmp;
	tmp = ps[tmp1];								// zamiana elementów w ps
	ps[tmp1] = ps[tmp2];
	ps[tmp2] = tmp;
	tmp = czasi[tmp1];							//zamiana czasów
	czasi[tmp1] = czasi[tmp2];
	czasi[tmp2] = tmp;
	for (int i = 0; i < n + 1; ++i){			//przygotowanie pozosta³ych tabel do aktualizacji
		ti[i] = 0;
		T[i] = 0;
		lp[i] = 0;
	}
	makeTi();
	makeLp();
}

int job_shop::getCmax(){
	return cMax;
}

