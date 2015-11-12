import numpy as np
import matplotlib.pyplot as plt
import sys

sys.path.insert(0, '../GeneralMonteCarlo')
import _SQLite_Database_ as SQLite

DatabaseName = ['DCP.db', 'DCP_new.db']

#Data = SQLite._SQLite_Database_('DCP_new.db', 'DilutedContact')
PrimaryKey = []
#PrimaryKey.append([128.0,1.64872,1.0,    500.0, 500.0,2.0])
L=[32, 64, 128, 256]
PrimaryKey.append([256.0,1.64872,1.0,  1000.0,  1000.0  ,2.0])
Delta = [            1.0,0.2    ,0.1,100000.0,100000.0,1.0]


plt.figure(1)
DatabaseName = ['DCP_new.db', 'DCP.db']
for Name in DatabaseName:
	Data = SQLite._SQLite_Database_(Name, 'DilutedContact')
	for LL in L:
		PrimaryKey[0][0] = LL
		y = Data.SearchforAll(PrimaryKey[0], Delta)
		Lamb = []
		rho2 = []
		rho22 = []
		for element in y:
			Lamb.append(element[1])
			rho2.append(element[7]*element[7]/(element[13]*element[13]))
			rho22.append(element[8]/element[13])
		Lamb = np.asarray(Lamb)
		rho2 = np.asarray(rho2)
		rho22 = np.asarray(rho22)
		plt.plot(Lamb, (rho22 - rho2)/rho2, '-x', label=Name+"L="+str(LL))
plt.legend()
plt.grid()
plt.axis((1.55, 1.75, 0.0, 1.2))

plt.show()
	

#for x in DatabaseName:
#	print x
#	Data = SQLite._SQLite_Database_(x, 'DilutedContact')
#	y = Data.SearchforAll(PrimaryKey[0], Delta)
#	for u in y:
#		rho2 = u[7]/u[13]
#		rho22 = u[8]/u[13]
#		rho2 = rho2*rho2
#		print u[0:7], (rho22-rho2)/rho2

#for x in PrimaryKey:
#	PrimaryKey[0] = x
#	u = Data.SearchforAll(x, Delta)
#	rho2 = u[0][7]/u[0][13]
#	rho2 = rho2*rho2
#	rho22 = u[0][8]/u[0][13]
#	print x, (rho22-rho2)/rho2
