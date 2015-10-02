cpp:
	@clear
	@clear
	mpiCC -Ofast -lm -Wall -ansi -lsqlite3 -std=c++11 -I../GeneralMonteCarlo -I../SFMT -msse2 -DHAVE_SSE2 -DSFMT_MEXP=607 -o ../2DDC teste.cpp
teste2:
	@clear
	@clear
	mpiCC -Ofast -lm -Wall -ansi -lsqlite3 -std=c++11 -I../GeneralMonteCarlo -I../SFMT -msse2 -DHAVE_SSE2 -DSFMT_MEXP=607 -o ../2DDC teste2.cpp
