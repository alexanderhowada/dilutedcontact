#include<iostream>
#include<random>
#include<mpi.h>

#include"_Parallelize_CondTimeSeries_Exp_.h"
#include"_2DDilutedContact_.h"

int main(int Nargs, char* Inputs[]){

 int rank, size;

 MPI_Init(&Nargs, &Inputs);
 MPI_Comm_rank(MPI_COMM_WORLD, &rank);
 MPI_Comm_size(MPI_COMM_WORLD, &size);

 std::random_device SeedGen;
 _MPI_vector_<unsigned int> Seed(1);

// const unsigned int NSimul = 1UL<<8;
 const unsigned int NSimul = 2;
 if(rank == 0){
	for(int process = 1; process < size; process++){
		Seed[0] = SeedGen();
		Seed.Send(process, 0);
	}
	_2DDilutedContact_ Simul(SeedGen(),Inputs[1], Inputs[2]);
	Simul.Save.Exec("PRAGMA page_size=16384;");
	Simul.Save.Exec("PRAGMA cache_size=100000;");
	Simul.Save.Exec("PRAGMA temp_store=2;");
	Simul.Save.Exec("PRAGMA synchronous=1;");
	Simul.Save.Exec("PRAGMA mmap_size=2097152;");
	Simul.Save.Exec("PRAGMA threads=4;");
	Simul.Save.Exec("VACUUM;");
	Simul.Save.Exec("PRAGMA threads;PRAGMA cache_size;PRAGMA synchronous;", _SQLite_Func_::print);
	_Parallelize_CondTimeSeries_Exp_ Parallel_Simul(+Simul, Inputs[3], rank, size);
	Parallel_Simul.Set_NTS_perSlave(NSimul);
	Parallel_Simul.run();
 }
 else{
	Seed.Recv(0, 0);
	_2DDilutedContact_ Simul(Seed[0]);
	Simul.Set_Init_infec_prob(1.0);
	_Parallelize_CondTimeSeries_Exp_ Parallel_Simul(+Simul, Inputs[3], rank, size);
	Parallel_Simul.Set_NTS_perSlave(NSimul);
	Parallel_Simul.run();
 }


 MPI_Finalize();
 return 0;
}
