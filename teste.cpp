#include<iostream>
#include<random>
#include<mpi.h>
#include<time.h>
#include"_2DDilutedContact_.h"

void master(int rank, int size, char *FileTable[]){

 std::mt19937 SeedGen(time(NULL));
 _MPI_vector_<unsigned int> Seeds(1);
 for(int process = 1; process < size; process++){
	Seeds[0] = SeedGen();
	Seeds.Send(process, 0);
 }

 _2DDilutedContact_ Sim_Contr(FileTable[0], FileTable[1]);



 printf("Exit 0\n");
}

void slave(int rank, int size){

 _MPI_vector_<unsigned int> Seed(1);
 Seed.Recv(0, 0);


 printf("Exit %d\n", rank);
}

int main(int Nargs, char *Input[]){
 int rank, size;
 MPI_Init(&Nargs, &Input);
 MPI_Comm_size(MPI_COMM_WORLD, &size);
 MPI_Comm_rank(MPI_COMM_WORLD, &rank);

 if(!rank){
	master(rank, size, &Input[1]);
 }
 else{
	slave(rank, size);
 }

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
