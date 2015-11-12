#include<iostream>
#include<mpi.h>
#include<time.h>
#include"_2DDilutedContact_.h"
#include"_Time_Series_.h"
#include"_Parallelize_TimeSeries_.h"

int main(int Nargs, char *Input[]){
 int rank, size;
 MPI_Init(&Nargs, &Input);
 MPI_Comm_size(MPI_COMM_WORLD, &size);
 MPI_Comm_rank(MPI_COMM_WORLD, &rank);

 _2DDilutedContact_ SiSimul("DCP_new.db", "DilutedContact");
 _Time_Series_ Res_t(SiSimul, 2);
// unsigned int Res_size = SiSimul.P2_Res->Get_size();

// for(unsigned int count = 0; count < size; count
 if(rank == 0){
	Res_t.Res_Array.Realloc(SiSimul.Results.Get_size()*3);
	for(int index = 0; index < Res_t.Res_Array.Get_size(); index++) Res_t.Res_Array[index] = index;
	for(unsigned int index = 0; index < 6; index++) Res_t.Parameters[index] = ~index;
//	Res_t.Set_to_0();
	Res_t.SendAll(1, 0);
	Res_t.Set_to_0();
	Res_t.Res_Array.Realloc(SiSimul.Results.Get_size()*1);
	Res_t.SendAll(1, 0);
 }
 else{
	Res_t.SleepRecvAnyAll();
	Res_t.Res_Array.Print(stdout, "%.0lf", " ");
	printf("\n");
	Res_t.Parameters.Print(stdout, "%lf", " ");
	printf("\n");
	Res_t.SleepRecvAnyAll();
	Res_t.Res_Array.Print(stdout, "%.0lf", " ");
	printf("\n");
 }
 MPI_Finalize();
 return 0;
}

