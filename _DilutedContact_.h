#ifndef _DilutedContact__h
#define _DilutedContact__h

#include<iostream>
#include<random>
#include<unistd.h>
#include<time.h>
#include<math.h>

#include"_MPI_vector_.h"
#include"_SQLite_Database_.h"

class _DilutedContact_{
	private:
 std::mt19937 rand32;
 std::mt19937_64 rand64;
 std::uniform_real_distribution<double> dist;
 unsigned int L;
 static const unsigned int NParam = 1;
 static const unsigned int NResults = 3;
 _SQLite_Database_ Save;
	public:
// void *Lattice;
 unsigned long long NSimul = 10000000000;
 _MPI_vector_<double> Parameters, Results;
 
 _DilutedContact_(void);
 _DilutedContact_(const char*, const char*);
 _DilutedContact_(unsigned long long, unsigned long long);
 _DilutedContact_(unsigned long long, unsigned long long, const char*, const char*);
 _DilutedContact_& Seed(unsigned long long, unsigned long long);
 _DilutedContact_& Set_Parameters(double*);
 _DilutedContact_& Set_Parameters(_MPI_vector_<double>&);
 _DilutedContact_& Set_InitialConditions(void);
 _DilutedContact_& Simulate(void);
 _DilutedContact_& Save_Simulation(void);
 void AllocateLattice(unsigned int);
 

};

/*************************************

	CONSTRUCTORS

*************************************/

_DilutedContact_::_DilutedContact_(void):
	dist(0.0, 1.0),
	Save(),
	Parameters(NParam),
	Results(NResults)
{}

_DilutedContact_::_DilutedContact_(unsigned long long seed32, unsigned long long seed64):
	dist(0.0, 1.0),
	Save(),
	Parameters(NParam),
	Results(NResults)
{
 this->AllocateLattice(L);
 this->Seed(seed32, seed64);
}

_DilutedContact_::_DilutedContact_(unsigned long long seed32, unsigned long long seed64, const char* Database, const char* Table):
	dist(0.0, 1.0),
	Save(Database, Table),
	Parameters(NParam),
	Results(NResults)
{
 this->AllocateLattice(L);
 this->Seed(seed32, seed64);
}

_DilutedContact_::_DilutedContact_(const char* Database, const char* Table):
	dist(0.0, 1.0),
	Save(Database, Table),
	Parameters(NParam),
	Results(NResults)
{}

_DilutedContact_& _DilutedContact_::Seed(unsigned long long seed32, unsigned long long seed64){
 rand32.seed(seed32);
 rand64.seed(seed64);
 return *this;
}

void _DilutedContact_::AllocateLattice(unsigned int L){
 this->L = L;
}


/*************************************

	GET/SET DATA

*************************************/

_DilutedContact_& _DilutedContact_::Set_Parameters(double *Param){
 for(int index = 0; index < Parameters.Get_size(); index++){
	Parameters[index] = Param[index];
 }
 return *this;
}

_DilutedContact_& _DilutedContact_::Set_Parameters(_MPI_vector_<double> &Param){
 Parameters = Param;
 return *this;
}


/*************************************

	Simulate

*************************************/

_DilutedContact_& _DilutedContact_::Set_InitialConditions(void){
 Results = 0.0;
 return *this;
}

_DilutedContact_& _DilutedContact_::Simulate(void){
 unsigned long long temp = (unsigned long long) NSimul;
 unsigned long long InsideCircle = 0;
 while(temp--){
	if(pow(dist(rand64), 2) + pow(dist(rand64), 2) < 1.0 ){
		InsideCircle++;
	}
 }
 Results[0] = double(NSimul);
 double Mean = double(InsideCircle)/Results[0];
 Results[2] = 4.0*sqrt((Mean - pow(Mean, 2))/Results[0]);
 Results[1] = 4.0*Mean;
 return *this;
}

/*************************************

	SaveData

*************************************/

_DilutedContact_& _DilutedContact_::Save_Simulation(void){ using namespace std;
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
	Results.Print(stderr, "%.12le", " ");
	fprintf(stderr, "\n");
 }
 return *this;
}


























#endif
