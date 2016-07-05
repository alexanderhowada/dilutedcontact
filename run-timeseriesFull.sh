mpirun -n 27 --hostfile nos-bcdc.txt ./../2DDC-timeseriesFull DCP-timeseriesFull.db DilutedContact Inputspy.txt &> run-timeseriesFull.txt
mpirun -n 126 --hostfile nos.txt ./../2DDC-timeseriesFull DCP-timeseriesFull005.db DilutedContact InputsTimeseriesFull.txt &> run-timeseriesFull005.txt
mpirun -n 126 --hostfile nos.txt ./../2DDC-timeseriesFull DCP-timeseriesFull001.db DilutedContact InputsTimeseriesFull.txt &> run-timeseriesFull001.txt
