//#include<iostream>
//#include<mpi.h>
#include<time.h>
//#include"_2DDilutedContactRdRand_.h"
#include"_2DDilutedContact_.h"
//#include"_Time_Series_.h"
//#include"_Parallelize_TimeSeries_.h"

int main(int Nargs, char *Input[]){
 int rank, size;
 MPI_Init(&Nargs, &Input);
 MPI_Comm_size(MPI_COMM_WORLD, &size);
 MPI_Comm_rank(MPI_COMM_WORLD, &rank);

 _2DDilutedContact_ SiSimul(time(NULL));

 FILE *Inputs = fopen("Inputs.txt", "r");
 
 SiSimul.Parameters.Scan(Inputs, "%lf", "%*c");
 SiSimul.Parameters.Print(stdout, "%lf", " ");
 printf("\n");
 unsigned long long NSimul = 100;
// double tau = SiSimul.Parameters[5];
 for(; NSimul; NSimul--){
	SiSimul.Set_Parameters();
	SiSimul.Set_InitialConditions();
	for(SiSimul.Parameters[3] = 0.0; SiSimul.Parameters[3] < SiSimul.Parameters[4]; SiSimul.Parameters[3] += SiSimul.Parameters[5]){
		SiSimul.Simulate();
	}
 }

 MPI_Finalize();
 return 0;
}

