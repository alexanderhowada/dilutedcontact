#ifndef _2DDilutedContact__h
#define _2DDilutedContact__h

#include<iostream>
#include<random>
#include<time.h>
#include<math.h>
#include<stdlib.h>
#include<string.h>

#include"_Generic_Simulation_.h"
#include"_MPI_vector_.h"
#include"_SQLite_Database_.h"

#ifndef _SFMT_C_
#define _SFMT_C_
#include"SFMT.c"
#endif

/*********************************
	PARAMETERS
L            - Lattice size
\lambda      - infection rate
p            - ocupation probability
t_init       - initial time to start making termal means
NTermalMeans - number of termal means
tau          - time interval between means

*********************************/

/*********************************
	RESULTS
NDisConf - Number of disorder configurations
p1 - order parameter (density of infected sites)
s_p1 - standard error of the order parameters
p2 - second moment of the order parameter
s_p2 standard error of the order parameter
R - mean radius
sR - standard error of the mean radius
S - survival probability
Noccup - mean radius
*********************************/

namespace _2DLattice_{
 struct _2D_uint16_{
	uint16_t x;
	uint16_t y;
 };
}

class _2DDilutedContact_: public _Generic_Simulation_ {
	private:
 sfmt_t sfmt;

 static const unsigned int NParameters = 6;
 static const unsigned int NResults = 9;

 _2DLattice_::_2D_uint16_ *ActSit = NULL;
 typedef _2DLattice_::_2D_uint16_ _2D_L_16_;
 unsigned long long ActSit_size = 0;
 unsigned long long NActive = 0;
 unsigned long long R2 = 0;
 unsigned int L = 0;
 double T = 0.0;

 _SQLite_Database_ Save;

 bool Allocate(unsigned int);
 inline uint16_t LowerP(uint16_t);
 inline uint16_t UpperP(uint16_t);
	public:
 _MPI_vector_<double> Parameters, Results;

 int8_t **Lattice = NULL;
 unsigned long long Noccup = 0;
 unsigned long long NSimul = 1;
 
 ~_2DDilutedContact_(void);

 _2DDilutedContact_(void);
 _2DDilutedContact_(const char*, const char*);
 _2DDilutedContact_(uint32_t);
 _2DDilutedContact_(uint32_t, const char*, const char*);
 _2DDilutedContact_& Seed(uint32_t);
 _2DDilutedContact_& Set_Parameters(void);
 _2DDilutedContact_& Set_Parameters(double*);
 _2DDilutedContact_& Set_Parameters(_MPI_vector_<double>&);
 bool Set_InitialConditions(void);
 _2DDilutedContact_& Simulate(void);
 bool Gen_PercConf(void);
 _2DDilutedContact_& PrintLattice(FILE*);
 _2DDilutedContact_& Save_Simulation(void);
 
};

/*************************************

	CONSTRUCTORS

*************************************/

_2DDilutedContact_::_2DDilutedContact_(void):
	Save(),
	Parameters(NParameters),
	Results(NResults)
{
 P2_Param = +Parameters;
 P2_Res = +Results;
}

_2DDilutedContact_::_2DDilutedContact_(uint32_t seed32):
	Save(),
	Parameters(NParameters),
	Results(NResults)
{
 this->Seed(seed32);
 P2_Param = +Parameters;
 P2_Res = +Results;
}

_2DDilutedContact_::_2DDilutedContact_(uint32_t seed32, const char* Database, const char* Table):
	Save(Database, Table),
	Parameters(NParameters),
	Results(NResults)
{
 this->Seed(seed32);
 P2_Param = +Parameters;
 P2_Res = +Results;
}

_2DDilutedContact_::_2DDilutedContact_(const char* Database, const char* Table):
	Save(Database, Table),
	Parameters(NParameters),
	Results(NResults)
{
 P2_Param = +Parameters;
 P2_Res = +Results;
}

_2DDilutedContact_& _2DDilutedContact_::Seed(uint32_t seed32){
 sfmt_init_gen_rand(&sfmt, seed32);
 P2_Param = +Parameters;
 P2_Res = +Results;
 return *this;
}

bool _2DDilutedContact_::Allocate(unsigned int L){
 assert(L > 4 && "L is to small\n");
 if(this->L == L){
	return false;
 }
// this->L = L;
 ActSit_size = 4*L*L;

 if(Lattice == NULL){
	Lattice = (int8_t**)malloc(sizeof(int8_t*)*L);
	if(Lattice == NULL){
		fprintf(stderr, "Error allocating Lattice\n");
		exit(1);
	}
	for(unsigned int index = 0; index < L; index++){
		Lattice[index] = (int8_t*)malloc(sizeof(int8_t)*L);
		if(Lattice[index] == NULL){
			fprintf(stderr, "Error allocating Lattice\n");
			exit(1);
		}
		memset(Lattice[index], 0, sizeof(int8_t)*L);
	}
	ActSit = (_2D_L_16_*)calloc( ActSit_size, sizeof(_2D_L_16_));
	this->L = L;
	return true;
 }
 for(unsigned int index = 0; index < this->L; index++) free(Lattice[index]);
 this->L = L;
 Lattice = (int8_t**)realloc(Lattice, sizeof(int8_t*)*L);
 if(Lattice == NULL){
	fprintf(stderr, "Error reallocating Lattice\n");
	exit(1);
 }
 for(unsigned int index = 0; index < L; index++){
	Lattice[index] = (int8_t*)calloc(L, sizeof(int8_t));
	if(Lattice[index] == NULL){
		fprintf(stderr, "Error reallocating Lattice\n");
		exit(1);
	}
 }
 ActSit = (_2D_L_16_*)realloc(ActSit, sizeof(_2D_L_16_)*ActSit_size);
 if(ActSit == NULL){
	fprintf(stderr, "Error reallocating ActSit\n");
	exit(1);
 }
 return true;
}

_2DDilutedContact_::~_2DDilutedContact_(void){
 for(unsigned int index = 0; index < L; index++){
	free(Lattice[index]);
	Lattice[index] = NULL;
 }
 free(Lattice);
 Lattice = NULL;
 
 free(ActSit);
 ActSit = NULL;
}

/*************************************

	GET/SET DATA

*************************************/
_2DDilutedContact_& _2DDilutedContact_::Set_Parameters(void){
 Allocate(unsigned(Parameters[0]));
 return *this;
}

_2DDilutedContact_& _2DDilutedContact_::Set_Parameters(double *Param){
 Parameters = Param;
 Allocate(unsigned(Parameters[0]));
 return *this;
}

_2DDilutedContact_& _2DDilutedContact_::Set_Parameters(_MPI_vector_<double> &Param){
 if(Param.Get_size() != Parameters.Get_size()){
	fprintf(stderr, "Parameters does not have the same size in Set_Parameters\n");
 }
 Parameters = Param;
 Allocate(unsigned(Parameters[0]));
 return *this;
}


/*************************************

	Simulate

*************************************/

bool _2DDilutedContact_::Set_InitialConditions(void){
 Results = 0.0;
 for(unsigned int index = 0; index < L; index++) memset(Lattice[index], 0, sizeof(int8_t)*L);
 memset(ActSit, 0, sizeof(_2D_L_16_)*ActSit_size);
 int ini = L/2;
 Lattice[ini][ini] = 1;

 ActSit[0].x = ini;
 ActSit[0].y = ini+1;

 ActSit[1].x = ini+1;
 ActSit[1].y = ini;

 ActSit[2].x = ini-1;
 ActSit[2].y = ini;

 ActSit[3].x = ini;
 ActSit[3].y = ini-1;
 
 Noccup = 1;
 NActive = 4;

 bool Percolate = Gen_PercConf();
 
 assert(NActive == 0);

 for(unsigned int x = 0; x < L; x++){
	for(unsigned int y = 0; y < L; y++){
		if(Lattice[x][y] == 1) Lattice[x][y] = 0;
	}
 }
 Lattice[ini][ini] = 1;
 NActive = 1;
 ActSit[0].x = ini;
 ActSit[0].y = ini;
 T = 0.0;
 R2 = 0;
 return Percolate;
}


/*********************************
	PARAMETERS
L            - Lattice size
\lambda      - infection rate
p            - ocupation probability
t_init       - initial time to start making termal means
NTermalMeans - number of termal means
tau          - time interval between means

*********************************/


inline uint16_t _2DDilutedContact_::LowerP(uint16_t x){
 return x > L ? L-1 : x;
}
inline uint16_t _2DDilutedContact_::UpperP(uint16_t x){
 return x == L ? 0 : x;
}

_2DDilutedContact_& _2DDilutedContact_::Simulate(void){

 double Infec_P = Parameters[1]/(1.0 + Parameters[1]);
 uint64_t temp;

 while(T < Parameters[3] && NActive){
	T += 1.0/NActive;
	temp = sfmt_genrand_uint64(&sfmt);
	if(sfmt_genrand_res53(&sfmt) < Infec_P){
		temp = sfmt_genrand_uint64(&sfmt);
		switch( temp%4 ){
			case 0:
				temp >>= 2;
				temp %= NActive;
				if(Lattice[LowerP(ActSit[temp].x-1)][ActSit[temp].y] == 0){
					Lattice[LowerP(ActSit[temp].x-1)][ActSit[temp].y] = 1;
					ActSit[NActive].x = LowerP(ActSit[temp].x-1);
					ActSit[NActive++].y = ActSit[temp].y;
				}
				break;
			case 1:
				temp >>= 2;
				temp %= NActive;
				if(Lattice[UpperP(ActSit[temp].x+1)][ActSit[temp].y] == 0){
					Lattice[UpperP(ActSit[temp].x+1)][ActSit[temp].y] = 1;
					ActSit[NActive].x = UpperP(ActSit[temp].x+1);
					ActSit[NActive++].y = ActSit[temp].y;
				}
				break;
			case 2:
				temp >>= 2;
				temp %= NActive;
				if(Lattice[ActSit[temp].x][LowerP(ActSit[temp].y-1)] == 0){
					Lattice[ActSit[temp].x][LowerP(ActSit[temp].y-1)] = 1;
					ActSit[NActive].x = ActSit[temp].x;
					ActSit[NActive++].y = LowerP(ActSit[temp].y-1);
				}
				break;
			case 4:
				temp >>= 2;
				temp %= NActive;
				if(Lattice[ActSit[temp].x][UpperP(ActSit[temp].y+1)] == 0){
					Lattice[ActSit[temp].x][UpperP(ActSit[temp].y+1)] = 1;
					ActSit[NActive].x = ActSit[temp].x;
					ActSit[NActive++].y = UpperP(ActSit[temp].y+1);
				}
				break;
		}
	}
	else{
		temp = sfmt_genrand_uint64(&sfmt)%NActive;
		Lattice[ActSit[temp].x][ActSit[temp].y] = 0;
		ActSit[temp] = ActSit[--NActive];
	}
 }
 Results[0] = 1.0;
 Results[1] = double(NActive)/double(L*L);
 Results[2] = 0.0;
 Results[3] = double(NActive*NActive)/double( (long long) L*L*L*L);
 Results[4] = 0.0;
 Results[5] = R2;
 Results[6] = 0.0;
 Results[7] = NActive ? 1.0 : 0.0;
// printf("%llu\n", NActive);
 return *this;
}

/*********************************
	RESULTS
0 NDisConf - Number of disorder configurations
1 p1 - order parameter (density of infected sites)
2 s_p1 - standard error of the order parameters
3 p2 - second moment of the order parameter
4 s_p2 standard error of the order parameter
5 R - mean radius
6 sR - standard error of the mean radius
7 S - survival probability
8 Noccup - mean radius
*********************************/

bool _2DDilutedContact_::Gen_PercConf(void){
 double p = Parameters[2];
 bool result = 0;
 unsigned long long rand;
 unsigned int x, y, L_1;
 x = y = 0;
 L_1 = L-1;
  
 while(NActive){
	x = ActSit[(rand = sfmt_genrand_uint64(&sfmt)%NActive)].x;
	y = ActSit[rand].y;
	if( Lattice[x][y] ){
		//faz nada
	}
	else if(sfmt_genrand_res53(&sfmt) < p){
		Lattice[x][y] = 1;
		Noccup++;
		if( !(x&&y) || x == L_1 || y == L_1){
			result = 1;
                        if(!Lattice[x][(y+1)%L]){
				ActSit[NActive].x = x;
				ActSit[NActive++].y = (y+1)%L;
                        }
                        if(!Lattice[x][(y+L-1)%L]){
				ActSit[NActive].x = x;
				ActSit[NActive++].y = (y+L-1)%L;
                        }
                        if(!Lattice[(x+1)%L][y]){
				ActSit[NActive].x = (x+1)%L;
				ActSit[NActive++].y = y;
                        }
                        if(!Lattice[(x+L-1)%L][y]){
				ActSit[NActive].x = (x+L-1)%L;
				ActSit[NActive++].y = y;
                        }
			ActSit[rand] = ActSit[--NActive];
			break;
		}
		if(!Lattice[x][y+1]){
			ActSit[NActive].x = x;
			ActSit[NActive++].y = y+1;
		}
		if(!Lattice[x][y-1]){
			ActSit[NActive].x = x;
			ActSit[NActive++].y = y-1;
		}
		if(!Lattice[x+1][y]){
			ActSit[NActive].x = x+1;
			ActSit[NActive++].y = y;
		}
		if(!Lattice[x-1][y]){
			ActSit[NActive].x = x-1;
			ActSit[NActive++].y = y;
		}
	}
	else{
		Lattice[x][y] = 2;
	}
	ActSit[rand] = ActSit[--NActive];
 }

 while(NActive){
	x = ActSit[(rand = sfmt_genrand_uint64(&sfmt)%NActive)].x;
	y = ActSit[rand].y;
	if( Lattice[x][y] ){
		//faz nada
	}
	else if(sfmt_genrand_res53(&sfmt) < p){
		Lattice[x][y] = 1;
		Noccup++;
		if(!Lattice[x][y == L_1 ? 0 : y+1]){
			ActSit[NActive].x = x;
			ActSit[NActive++].y = y == L_1 ? 0 : y+1;
		}
		if(!Lattice[x][y == 0 ? L_1 : y-1]){
			ActSit[NActive].x = x;
			ActSit[NActive++].y = y == 0 ? L_1 : y-1;
		}
		if(!Lattice[x == L_1 ? 0 : x+1][y]){
			ActSit[NActive].x = x == L_1 ? 0 : x+1;
			ActSit[NActive++].y = y;
		}
		if(!Lattice[x == 0 ? L_1 : x-1][y]){
			ActSit[NActive].x = x == 0 ? L_1 : x-1;
			ActSit[NActive++].y = y;
		}
	}
	else{
		Lattice[x][y] = 2;
	}
	ActSit[rand] = ActSit[--NActive];
 }
 Results[7] = Noccup;
 return result;
}

/*************************************

	SaveData

*************************************/
_2DDilutedContact_& _2DDilutedContact_::PrintLattice(FILE *fstream){
 for(unsigned int x = 0; x < L; x++){
	unsigned int y;
	for(y = 0; y < L-1; y++) fprintf(fstream, "%d ", Lattice[x][y]);
	fprintf(fstream, "%d\n", Lattice[x][y]);
 }
 return *this;
}

_2DDilutedContact_& _2DDilutedContact_::Save_Simulation(void){
 _MPI_vector_<double> temp(NResults);
 int NOutput = Save.Searchfor(Parameters.Get_Pointer(), temp.Get_Pointer());
 if( NOutput == 1){
	temp[2] /= 4.0;
	temp[2] *= temp[2];
	temp[2] *= temp[0];
	temp[2] += pow(temp[1]*0.25, 2);

	Results[2] /= 4.0;
	Results[2] *= Results[2];
	Results[2] *= Results[0];
	Results[2] += pow(Results[1]*0.25, 2);

	temp[1] = (temp[1]*temp[0] + Results[1]*Results[0])/(temp[0] + Results[0]);

	temp[2] = (temp[2]*temp[0] + Results[2]*Results[0])/(temp[0] + Results[0]);
	temp[2] -= temp[1]*temp[1]*0.0625;
	temp[0] += Results[0];
	temp[2] /= temp[0];
	temp[2] = sqrt(temp[2]);
	temp[2] *= 4.0;
	Save.Update(Parameters.Get_Pointer(), temp.Get_Pointer());
 }
 else if(NOutput == 0){
	Save.Insert(Parameters.Get_Pointer(), Results.Get_Pointer());
 }
 else{
	fprintf(stderr, "There is more than one output to be updated at once\n");
	Results.Print(stderr, "%.14le", " ");
	fprintf(stderr, "\n");
 }
 return *this;
}

#endif
