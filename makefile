cpp:
	@clear
	@clear
	mpiCC teste.cpp -o teste -lm -Wall -ansi -lsqlite3 -std=c++11 -I../GeneralMonteCarlo
