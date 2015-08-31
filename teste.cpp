#include<iostream>
#include<random>
#include<mpi.h>
#include<time.h>
#include"_DilutedContact_.h"


int main(int Nargs, char *Input[]){
 int rank, size;
 MPI_Init(&Nargs, &Input);
 MPI_Comm_rank(MPI_COMM_WORLD, &rank);
 MPI_Comm_size(MPI_COMM_WORLD, &size);

 _MPI_vector_<unsigned long long> Seeds(2);
 if(rank == 0){
	std::mt19937_64 seeds(time(NULL));
	for(int Process = 1; Process < size; Process++){
		Seeds[0] = seeds();
		Seeds[1] = seeds();
		Seeds.Send(Process, 0);
	}
	Seeds[0] = seeds();
	Seeds[1] = seeds();
 }
 else{
	Seeds.Recv(0, 0);
 }

 _DilutedContact_ lol(Seeds[0], Seeds[1], Input[1], Input[2]);
 lol.NSimul = 100000000;
 lol.Parameters[0] = 0.1234;
 lol.Set_InitialConditions();
 lol.Simulate();
 lol.Save_Simulation();

 MPI_Finalize();
 return 0;
}
