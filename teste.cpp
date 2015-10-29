#include<iostream>
#include<mpi.h>
#include<time.h>
#include"_2DDilutedContact_.h"
#include"_Temp_Series_.h"


int main(int Nargs, char *Input[]){
 int rank, size;
 MPI_Init(&Nargs, &Input);
 MPI_Comm_size(MPI_COMM_WORLD, &size);
 MPI_Comm_rank(MPI_COMM_WORLD, &rank);

 _2DDilutedContact_ SiSimul("DCP_new.db", "DilutedContact");
 _Temp_Series_ Res_t(SiSimul, 2);
// unsigned int Res_size = SiSimul.P2_Res->Get_size();

// for(unsigned int count = 0; count < size; count
 if(rank == 0){
	for(unsigned int index = 0; index < 18; index++) Res_t.Res_Array[index] = index;
	for(unsigned int index = 0; index < 6; index++) Res_t.Parameters[index] = ~index;
	Res_t.Set_to_0();
	Res_t.Send(1, 0);
 }
 else{
	printf("lo\n\n");
	Res_t.SleepRecvAny();
	Res_t.Res_Array.Print(stdout, "%lf", " ");
	printf("\n");
	Res_t.Parameters.Print(stdout, "%lf", " ");
	printf("\n");
 }
 MPI_Finalize();
 return 0;
}

