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

 const char *PrintSpec = "%.13lf";
 const char *SaveName = "Inputspy.txt";

 *L = 1024.0;
 *p = 0.9;
 *t_init = 100.;
 *NThermalMeans = 10000.;
 *tau = 0.05;
 *NDisConf = 200000.;

 FILE *save = fopen(SaveName, "a+");
 if(save == NULL) exit(1);
 for(*L=512; *L<1025; *L*=2){
 for(*Lambda = 3.11; *Lambda < 3.20; *Lambda += 0.01){
 for(*p = 0.59274605079210; *p < 0.60; *p+=0.001){
		Inputs.Print(save, PrintSpec, " ");
		fprintf(save, "\n");
 }
 }
 }
}
