CC=mpiCC
CFLAGS=-O3 -Wall -ansi -std=c++11 -I../GeneralMonteCarlo -I../SFMT -msse2 -DHAVE_SSE2 -DSFMT_MEXP=607
DEPS=../GeneralMonteCarlo/_Generic_Simulation_.h ../GeneralMonteCarlo/_MPI_vector_.h ../GeneralMonteCarlo/_SQLite_Database_.h _2DDilutedContact_.h ../GeneralMonteCarlo/_Parallelize_Stationary_.h ../SFMT/SFMT.c ../SFMT/SFMT.h
INC_LIB=-lm -lsqlite3

cpp : $(DEPS) teste.cpp
	@clear
	@clear
	$(CC) $(CFLAGS) -o ../2DDC teste.cpp $(INC_LIB)

teste2 : $(DEPS) teste2.cpp
	@clear
	@clear
	$(CC) $(CFLAGS) -o ../2DDC teste2.cpp $(INC_LIB)
