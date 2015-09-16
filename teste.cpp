#include<iostream>
#include<random>
#include<mpi.h>
#include<time.h>
#include"_2DDilutedContact_.h"

int main(int Nargs, char *Input[]){
 int rank, size;
 MPI_Init(&Nargs, &Input);
 MPI_Comm_size(MPI_COMM_WORLD, &size);
 MPI_Comm_rank(MPI_COMM_WORLD, &rank);

// _2DDilutedContact_ Sisi(Input[1], Input[2]);
 _2DDilutedContact_ Sisi((unsigned long long) time(NULL));
 _MPI_vector_<double> Parameters( Sisi.Parameters.Get_size() );

 Parameters[0] = 5.0;
 Parameters[2] = 0.5;

 Sisi.Set_Parameters(Parameters);
 Sisi.Set_InitialConditions();
 while(!Sisi.Gen_PercConf()) Sisi.Set_InitialConditions();
 Sisi.PrintLattice(stdout);
 printf("%llu\n", Sisi.Noccup);

 MPI_Finalize();
 return 0;
}

//int main(int Nargs, char *Input[]){
// int rank, size;
// MPI_Init(&Nargs, &Input);
// MPI_Comm_rank(MPI_COMM_WORLD, &rank);
// MPI_Comm_size(MPI_COMM_WORLD, &size);

// _MPI_vector_<unsigned long long> Seeds(2);
// if(rank == 0){
//	std::mt19937_64 seeds(time(NULL));
//	for(int Process = 1; Process < size; Process++){
//		Seeds[0] = seeds();
//		Seeds[1] = seeds();
//		Seeds.Send(Process, 0);
//	}
//	Seeds[0] = seeds();
//	Seeds[1] = seeds();
// }
// else{
//	Seeds.Recv(0, 0);
// }

// _2DDilutedContact_ lol(Seeds[0], Seeds[1], Input[1], Input[2]);
// _MPI_vector_<double> Param(lol.Parameters.Get_size());
// double *x = (double*)malloc(sizeof(double)*lol.Parameters.Get_size());
// x[0] = 6.0;
// lol.Set_Parameters(x);
// free(x);x = NULL;
// lol.Set_InitialConditions();
// lol.Simulate();
// lol.PrintLattice(stdout);

// MPI_Finalize();
// return 0;
//}
