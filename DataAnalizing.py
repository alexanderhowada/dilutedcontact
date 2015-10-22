import numpy as np
import matplotlib.pyplot as plt
import sys

sys.path.insert(0, '../GeneralMonteCarlo')
import _SQLite_Database_ as SQLite

Data = SQLite._SQLite_Database_('DCP.db', 'DilutedContact')
PrimaryKey = [128.0,1.64872,1.0,20000.0,5000.0,2.0]
Delta = [       1.0,0.00001,0.1,    0.1,   0.1,0.1]
L = [128, 256];

for x in L:
	PrimaryKey[0] = x
	u = Data.SearchforAll(PrimaryKey, Delta)
	rho2 = u[0][7]*u[0][7]
	rho22 = u[0][8]
	print PrimaryKey[0], (rho22-rho2)/rho2
