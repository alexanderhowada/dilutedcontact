import numpy as np
import matplotlib.pyplot as plt
import sys

sys.path.insert(0, '../GeneralMonteCarlo')
import _SQLite_Database_ as SQLite

Data = SQLite._SQLite_Database_('DCP_new.db', 'DilutedContact')
PrimaryKey = []
PrimaryKey.append([128.0,1.64872,1.0,1500.0, 500.0,2.0])
PrimaryKey.append([256.0,1.64872,1.0,3000.0,1000.0,2.0])
Delta = [       1.0,0.00001,0.1,    0.1,   0.1,0.1]

for x in PrimaryKey:
#	PrimaryKey[0] = x
	u = Data.SearchforAll(x, Delta)
	rho2 = u[0][7]*u[0][7]/(u[0][13]*u[0][13])
	rho22 = u[0][8]/u[0][13]
	print x, (rho22-rho2)/rho2
