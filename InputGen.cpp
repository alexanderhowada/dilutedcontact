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
 const char *SaveName = "Inputs2.txt";

 *L = 128.0;
 *p = 1.0;
 *t_init = 100000.0;
 *NThermalMeans = 500000.0;
 *tau = 5.0;
 *NDisConf = (1<<9);

 FILE *save = fopen(SaveName, "w");
 if(save == NULL) exit(1);
 for(*Lambda = 1.6; *Lambda < 5.0; *Lambda += 0.025){
 for(*p = 0.6; *p <= 1.001; *p+=0.025){
		Inputs.Print(save, PrintSpec, " ");
		fprintf(save, "\n");
 }
 }
}
