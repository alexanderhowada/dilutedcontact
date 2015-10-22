#include<iostream>
#include<random>
#include<mpi.h>

#include"_Parallelize_Stationary_.h"
#include"_2DDilutedContact_.h"

int main(int Nargs, char* Inputs[]){

 int rank, size;

 MPI_Init(&Nargs, &Inputs);
 MPI_Comm_rank(MPI_COMM_WORLD, &rank);
 MPI_Comm_size(MPI_COMM_WORLD, &size);

 std::random_device SeedGen;
 _MPI_vector_<unsigned int> Seed(1);

 if(rank == 0){
	for(int process = 1; process < size; process++){
		Seed[0] = SeedGen();
		Seed.Send(process, 0);
	}
	_2DDilutedContact_ Simul(time(NULL),Inputs[1], Inputs[2]);
	_Parallelize_Stationary_ Parallel_Simul(+Simul, Inputs[3], rank, size);
	Parallel_Simul.run();
 }
 else{
	Seed.Recv(0, 0);
	_2DDilutedContact_ Simul(Seed[0]);
	_Parallelize_Stationary_ Parallel_Simul(+Simul, Inputs[3], rank, size);
	Parallel_Simul.run();
 }


 MPI_Finalize();
 return 0;
}