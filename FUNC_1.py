import numpy as np

def CILINDRO_ESFERICO(a,b,c,ra,rb,lz,NO_DOTS,LAPS,u,v):
	F = np.zeros((3,1))
	F = [[(a - ra*np.cos(u))*160000],[(c + v)*800000],[(b - rb*np.sin(u))*800000]]
	F = [int(F[0][0]),int(F[1][0]),int(F[2][0])]
	return F
	
	
a = 0.05
b = 0.05
c = 0.05

ra = 0.03
rb = 0.03
lz = 0.06

NO_DOTS = 10
LAPS = 1

u = np.transpose(np.linspace(0,2*LAPS*np.pi,NO_DOTS))
v = np.transpose(np.linspace(0,lz,NO_DOTS))

for i in range(0,NO_DOTS):
	D = CILINDRO_ESFERICO(a,b,c,ra,rb,lz,NO_DOTS,LAPS,u[i],v[i])
	print(D)
