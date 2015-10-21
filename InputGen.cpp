#include<iostream>
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
 *t_init = 1000000.0;
 *NThermalMeans = 5000.0;
 *tau = 20.0;
 *NDisConf = 100.0;

 FILE *save = fopen(SaveName, "a+");
 if(save == NULL) exit(1);

 for(*Lambda = 2.5; *Lambda < 4.0; *Lambda += 0.01){
	Inputs.Print(save, PrintSpec, " ");
	fprintf(save, "\n");
 }
}
