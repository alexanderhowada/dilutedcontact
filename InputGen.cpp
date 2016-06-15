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
// *p = 0.592746050792;
 *p = 1.0;
 *t_init = 0.;
 *NThermalMeans = 100000.;
 *tau = 4.;
// *tau = 1.0;
 *NDisConf = 16.;

 FILE *save = fopen(SaveName, "a+");
 if(save == NULL) exit(1);
 for(*L=1024; *L<2049; *L*=2){
 for(*Lambda = 1.0; *Lambda < 2.1; *Lambda += 0.1){
// for(*p = 0.592746050792; *p < 1.0; *p+=0.025){
		Inputs.Print(save, PrintSpec, " ");
		fprintf(save, "\n");
// }
 }
 }
}
