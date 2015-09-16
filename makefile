cpp:
	@clear
	@clear
	mpiCC -Ofast -lm -Wall -ansi -lsqlite3 -std=c++11 -I../GeneralMonteCarlo -I../SFMT -msse2 -DHAVE_SSE2 -DSFMT_MEXP=19937 -o ../teste teste.cpp
