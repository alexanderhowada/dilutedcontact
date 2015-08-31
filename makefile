cpp:
	@clear
	@clear
	mpiCC teste.cpp -o teste -lm -Wall -ansi -lsqlite3 -g3 -std=c++11 -I../GeneralMonteCarlo -O2
