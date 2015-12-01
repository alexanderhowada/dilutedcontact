CC=mpiCC
CFLAGS=-O3 -Wall -ansi -std=c++11 -I../GeneralMonteCarlo -I../SFMT -msse2 -DHAVE_SSE2 -DSFMT_MEXP=607 -march=native
DEPS=../GeneralMonteCarlo/_Generic_Simulation_.h ../GeneralMonteCarlo/_MPI_vector_.h ../GeneralMonteCarlo/_SQLite_Database_.h _2DDilutedContact_.h ../GeneralMonteCarlo/_Parallelize_Stationary_.h ../SFMT/SFMT.c ../SFMT/SFMT.h
INC_LIB=-lm -lsqlite3

steadystate : $(DEPS) run-steadystate.cpp
	@clear
	@clear
	$(CC) $(CFLAGS) -o ../2DDC-steadystate run-steadystate.cpp $(INC_LIB)

timeseries : $(DEPS) run-timeseries.cpp
	@clear
	@clear
	$(CC) $(CFLAGS) -o ../2DDC-timeseries run-timeseries.cpp $(INC_LIB)

Inputs: $(DEPS) InputGen.cpp
	@clear
	@clear
	$(CC) -O3 -Wall -ansi -I../GeneralMonteCarlo -std=c++11 -o InputGen InputGen.cpp -lm

teste: $(DEPS) teste.cpp
	@clear
	@clear
	$(CC) $(CFLAGS) -o ../2DDC teste.cpp $(INC_LIB)
