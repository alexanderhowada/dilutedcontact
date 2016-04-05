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

 *L = 1024.0;
 *p = 0.9;
 *t_init = 100.;
 *NThermalMeans = 1000000.;
 *tau = 0.05;
 *NDisConf = 300000.;

 FILE *save = fopen(SaveName, "a+");
 if(save == NULL) exit(1);
 for(*L=1024; *L<2049; *L*=2){
 for(*Lambda = 1.80; *Lambda < 2.01; *Lambda += 0.01){
 for(*p = 0.85; *p < 0.91; *p+=0.01){
		Inputs.Print(save, PrintSpec, " ");
		fprintf(save, "\n");
 }
 }
 }
}
