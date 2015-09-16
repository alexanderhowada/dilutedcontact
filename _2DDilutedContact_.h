#ifndef _2DDilutedContact__h
#define _2DDilutedContact__h

#include<iostream>
#include<random>
#include<time.h>
#include<math.h>
#include<stdlib.h>
#include<string.h>

#include"_MPI_vector_.h"
#include"_SQLite_Database_.h"
#include"SFMT.c"

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
p4 - fourth moment of the order parameter
s_p4 - standard error of p4
*********************************/

class _2DDilutedContact_{
	private:
// std::mt19937 rand_32;
// std::mt19937_64 rand_64;
 sfmt_t sfmt;
 std::uniform_real_distribution<double> dist;
 _SQLite_Database_ Save;

 static const unsigned int NParameters = 6;
 static const unsigned int NResults = 7;


 uint16_t *ActSit = NULL;
 unsigned long long ActSit_size = 0;
 unsigned long long NActive = 0;
 unsigned int L = 0;

 bool Allocate(unsigned int);

	public:
 _MPI_vector_<double> Parameters, Results;

 int8_t **Lattice = NULL;
 unsigned long long Noccup;
 unsigned long long NSimul;
 
 ~_2DDilutedContact_(void);

 _2DDilutedContact_(void);
 _2DDilutedContact_(const char*, const char*);
 _2DDilutedContact_(uint32_t);
 _2DDilutedContact_(uint32_t, const char*, const char*);
 _2DDilutedContact_& Seed(uint32_t);
 _2DDilutedContact_& Set_Parameters(void);
 _2DDilutedContact_& Set_Parameters(double*);
 _2DDilutedContact_& Set_Parameters(_MPI_vector_<double>&);
 _2DDilutedContact_& Set_InitialConditions(void);
 _2DDilutedContact_& Simulate(void);
 bool Gen_PercConf(void);
 _2DDilutedContact_& PrintLattice(FILE*);
 _2DDilutedContact_& Save_Simulation(void);
 

};

/*************************************

	CONSTRUCTORS

*************************************/

_2DDilutedContact_::_2DDilutedContact_(void):
	dist(0.0, 1.0),
	Save(),
	Parameters(NParameters),
	Results(NResults),
	Noccup(0),
	NSimul(1)
{}

_2DDilutedContact_::_2DDilutedContact_(uint32_t seed32):
	dist(0.0, 1.0),
	Save(),
	Parameters(NParameters),
	Results(NResults),
	Noccup(0),
	NSimul(1)
{
 this->Seed(seed32);
}

_2DDilutedContact_::_2DDilutedContact_(uint32_t seed32, const char* Database, const char* Table):
	dist(0.0, 1.0),
	Save(Database, Table),
	Parameters(NParameters),
	Results(NResults),
	Noccup(0),
	NSimul(1)
{
 this->Seed(seed32);
}

_2DDilutedContact_::_2DDilutedContact_(const char* Database, const char* Table):
	dist(0.0, 1.0),
	Save(Database, Table),
	Parameters(NParameters),
	Results(NResults),
	Noccup(0),
	NSimul(1)
{}

_2DDilutedContact_& _2DDilutedContact_::Seed(uint32_t seed32){
// rand_32.seed(seed32);
// rand_64.seed(seed64);
 sfmt_init_gen_rand(&sfmt, seed32);
 return *this;
}

bool _2DDilutedContact_::Allocate(unsigned int L){
 if(this->L == L){
	for(unsigned int index = 0; index < L; index++){
		memset(Lattice[index], 0, sizeof(int8_t)*L);
	}
	memset(ActSit, 0, sizeof(uint16_t)*ActSit_size);
	return false;
 }
 this->L = L;
 ActSit_size = 8*L*L;

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
	ActSit = (uint16_t*)calloc( ActSit_size, sizeof(uint16_t)*ActSit_size);
	return true;
 }

 Lattice = (int8_t**)realloc(Lattice, sizeof(int8_t*)*L);
 if(Lattice == NULL){
	fprintf(stderr, "Error reallocating Lattice\n");
	exit(1);
 }
 for(unsigned int index = 0; index < L; index++){
	Lattice[index] = (int8_t*)realloc(Lattice[index], sizeof(int8_t)*L);
	if(Lattice[index] == NULL){
		fprintf(stderr, "Error reallocating Lattice\n");
		exit(1);
	}
	memset(Lattice[index], 0, sizeof(int8_t)*L);
 }
 ActSit = (uint16_t*)realloc(ActSit, sizeof(uint16_t)*ActSit_size);
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
 Allocate(int(Parameters[0]));
 return *this;
}

_2DDilutedContact_& _2DDilutedContact_::Set_Parameters(double *Param){
 Parameters = Param;
 Allocate(int(Parameters[0]));
 return *this;
}

_2DDilutedContact_& _2DDilutedContact_::Set_Parameters(_MPI_vector_<double> &Param){
 if(Param.Get_size() != Parameters.Get_size()){
	fprintf(stderr, "Parameters does not have the same size in Set_Parameters\n");
 }
 Parameters = Param;
 Allocate(int(Parameters[0]));
 return *this;
}


/*************************************

	Simulate

*************************************/

_2DDilutedContact_& _2DDilutedContact_::Set_InitialConditions(void){
 Results = 0.0;
 for(unsigned int index = 0; index < L; index++) memset(Lattice[index], 0, sizeof(int8_t)*L);
 int ini = L/2;
 Lattice[ini][ini] = 1;
 if(NActive != 0){
	fprintf(stderr, "Number of Active Sites in Set_InitialConditions is not 0\n");
	exit(1);
 }
 ActSit[0]= ini;
 ActSit[1]= ini+1;

 ActSit[2]= ini+1;
 ActSit[3]= ini;

 ActSit[4]= ini-1;
 ActSit[5]= ini;

 ActSit[6]= ini;
 ActSit[7]= ini-1;
 
 Noccup = 1;
 NActive = 4;
 return *this;
}

_2DDilutedContact_& _2DDilutedContact_::Simulate(void){
 printf("Simulating...\n");
 return *this;
}

bool _2DDilutedContact_::Gen_PercConf(void){
 double p = Parameters[2];
 bool result = 0;
 unsigned long long rand;
 unsigned int x, y, L_1;
 x = y = 0;
 L_1 = L-1;
  
 while(NActive){
	x = ActSit[ (rand = sfmt_genrand_uint64(&sfmt)%NActive*2 ) ];
	y = ActSit[rand +1];
	if( Lattice[x][y] ){
		//faz nada
	}
	else if(sfmt_genrand_res53(&sfmt) < p){
		Lattice[x][y] = 1;
		Noccup++;
		if( !(x&&y) || x == L_1 || y == L_1){
			result = 1;
                        if(!Lattice[x][(y+1)%L]){
				ActSit[2*NActive] = x;
				ActSit[2*NActive++ + 1] = (y+1)%L;
                        }
                        if(!Lattice[x][(y+L-1)%L]){
				ActSit[2*NActive] = x;
				ActSit[2*NActive++ + 1] = (y+L-1)%L;
                        }
                        if(!Lattice[(x+1)%L][y]){
				ActSit[2*NActive] = (x+1)%L;
				ActSit[2*NActive++ + 1] = y;
                        }
                        if(!Lattice[(x+L-1)%L][y]){
				ActSit[2*NActive] = (x+L-1)%L;
				ActSit[2*NActive++ + 1] = y;
                        }
			memcpy(&ActSit[rand], &ActSit[2*--NActive], 4);
			break;
		}
		if(!Lattice[x][y+1]){
			ActSit[2*NActive] = x;
			ActSit[2*NActive++ + 1] = y+1;
		}
		if(!Lattice[x][y-1]){
			ActSit[2*NActive] = x;
			ActSit[2*NActive++ + 1] = y-1;
		}
		if(!Lattice[x+1][y]){
			ActSit[2*NActive] = x+1;
			ActSit[2*NActive++ + 1] = y;
		}
		if(!Lattice[x-1][y]){
			ActSit[2*NActive] = x-1;
			ActSit[2*NActive++ + 1] = y;
		}
	}
	else{
		Lattice[x][y] = 2;
	}
	memcpy(&ActSit[rand], &ActSit[2*--NActive], 4);
 }

 while(NActive){
	x = ActSit[ (rand = sfmt_genrand_uint64(&sfmt)%NActive*2) ];
	y = ActSit[rand +1];
	if( Lattice[x][y] ){
		//faz nada
	}
	else if(sfmt_genrand_res53(&sfmt) < p){
		Lattice[x][y] = 1;
		Noccup++;
		if(!Lattice[x][y == L_1 ? 0 : y+1]){
			ActSit[2*NActive] = x;
			ActSit[2*NActive++ + 1] = y == L_1 ? 0 : y+1;
		}
		if(!Lattice[x][y == 0 ? L_1 : y-1]){
			ActSit[2*NActive] = x;
			ActSit[2*NActive++ + 1] = y == 0 ? L_1 : y-1;
		}
		if(!Lattice[x == L_1 ? 0 : x+1][y]){
			ActSit[2*NActive] = x == L_1 ? 0 : x+1;
			ActSit[2*NActive++ + 1] = y;
		}
		if(!Lattice[x == 0 ? L_1 : x-1][y]){
			ActSit[2*NActive] = x == 0 ? L_1 : x-1;
			ActSit[2*NActive++ + 1] = y;
		}
	}
	else{
		Lattice[x][y] = 2;
	}
	memcpy(&ActSit[rand], &ActSit[2*--NActive], 4);
 }
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
