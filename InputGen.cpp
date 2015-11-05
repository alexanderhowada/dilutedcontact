#include<iostream>
#include<math.h>
#include"_MPI_vector_.h"

int main(int Nargs, char *Input[]){
 _MPI_vector_<double> Inputs(7);
 double *Inputs_temp = Inputs.Get_Pointer();
 double *L = Inputs_temp++;
 double *Lambda = Inputs_temp++;
 double *p = Inputs_temp++;
 double *t_init = Inputs_temp++;
 double *NThermalMeans = Inputs_temp++;
 double *tau = Inputs_temp++;
 double *NDisConf = Inputs_temp++;

 const char *PrintSpec = "%lf";
 const char *SaveName = "Inputs.txt";

 *L = 32.0;
 *p = 1.0;
 *t_init = 250.0;
 *NThermalMeans = *t_init;
 *tau = 2.0;
 *NDisConf = 10000.0;

 FILE *save = fopen(SaveName, "a+");
 if(save == NULL) exit(1);
 for(*L = 32.0; *L < 257.0; (*L) *=2.0, *t_init *= 1.5, *NThermalMeans *= 1.5){
 for(*Lambda = 1.64872 - 0.0125; *Lambda < 1.64872 + 0.0125; *Lambda += 0.0025){
	Inputs.Print(save, PrintSpec, " ");
	fprintf(save, "\n");
 }
 }
}
