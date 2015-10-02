#include<iostream>

#include<mpi.h>

#include"_Parallelize_Stationary_.h"
#include"_2DDilutedContact_.h"

int main(int Nargs, char* Inputs[]){

 int rank, size;

 MPI_Init(&Nargs, &Inputs);
 MPI_Comm_rank(MPI_COMM_WORLD, &rank);
 MPI_Comm_size(MPI_COMM_WORLD, &size);

 _2DDilutedContact_ Simul(time(NULL),Inputs[1], Inputs[2]);
 _Parallelize_Stationary_ Parallel_Simul(+Simul, Inputs[3], rank, size);
 Parallel_Simul.run();

 MPI_Finalize();
 return 0;
}
