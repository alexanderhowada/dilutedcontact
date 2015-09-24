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
 sscanf(FileTable[2], "%lf", &Sim_Contr.Parameters[0]);
 sscanf(FileTable[3], "%lf", &Sim_Contr.Parameters[2]);
// Sim_Contr.Parameters.Print(stdout, "%lf", " ");fflush(stdout);
// Sim_Contr.Parameters[0] = 2048.0;
// Sim_Contr.Parameters[2] = 0.5;

 for(int process = 1; process < size; process++){
	Sim_Contr.Parameters.Send(process, 0);
 }

 long double MNoccup = 0.0;
 const unsigned int NMeans = 100;
 for(unsigned int Nsimul = 0; Nsimul < NMeans - size + 1; Nsimul++){
	Sim_Contr.Results.SleepRecvAny();
	MNoccup += Sim_Contr.Results[0];
	Sim_Contr.Parameters.Send(Sim_Contr.Results.Status.MPI_SOURCE, Sim_Contr.Results.Status.MPI_TAG);
 }
 Sim_Contr.Parameters[2] = 4.0;
 for(int process = 1; process < size; process++){
	Sim_Contr.Results.SleepRecvAny();
	MNoccup += Sim_Contr.Results[0];
	Sim_Contr.Parameters.Send(Sim_Contr.Results.Status.MPI_SOURCE, Sim_Contr.Results.Status.MPI_TAG);
 }
 printf("%Le\n", MNoccup/double(NMeans)/Sim_Contr.Parameters[0]/Sim_Contr.Parameters[0]);
// printf("Exit 0\n");
}

void slave(int rank, int size){

 _MPI_vector_<unsigned int> Seed(1);
 Seed.Recv(0, 0);
 _2DDilutedContact_ Simul(Seed[0]);
// time_t t1, t2;
 Simul.Parameters.Recv(0, 0);
 const unsigned int Ntimes = 1000;
 while(Simul.Parameters[2] < 1.0 && Simul.Parameters[2] >= 0.0){
	unsigned long long Means = 0;
	for(unsigned int times = 0; times < Ntimes; times++){
		Simul.Set_Parameters();
		Simul.Set_InitialConditions();
		Simul.Gen_PercConf();
		Means += Simul.Noccup;
	}
	Simul.Results[0] = double(Means)/double(Ntimes);
	Simul.Results.Send(0, 0);
	Simul.Parameters.Recv(0, 0);
 }
// printf("Exit %d\n", rank);
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

