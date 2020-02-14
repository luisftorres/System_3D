import numpy as np

def CILINDRO_ESFERICO(a,b,c,ra,rb,lz,u,v):
	F = np.zeros((3,1))
	F = [[(a - ra*np.cos(u))*160000],[(c + v)*800000],[(b - rb*np.sin(u))*800000]]
	F = [int(F[0][0]),int(F[1][0]),int(F[2][0])]
	return F
	
def LINEA_RECTA(P1,P2,t):
	F = np.zeros((3,1))
	F = [[(P1[0] + t*(P2[0]-P1[0]))*160000],[(P1[1] + t*(P2[1]-P1[1]))*800000],[(P1[2] + t*(P2[2]-P1[2]))*800000]]
	F = [int(F[0][0]),int(F[1][0]),int(F[2][0])]
	return F
	
def CIRCULO(a,b,c,r,t):
	F = np.zeros((3,1))
	F = [[a + r*np.cos(t)],[],[a + r*np.cos(t)]]
	F = [int(F[0][0]),int(F[1][0]),int(F[2][0])]
	return F
	
	
	
# def d_CILINDRO_ESFERICO(a,b,c,ra,rb,lz,NO_DOTS,LAPS,u,v):
	# dF = np.zeros((3,1))
	# dF = [[(a - ra*np.cos(u))*160000],[(c + v)*800000],[(b - rb*np.sin(u))*800000]]
	# dF = [int(F[0][0]),int(F[1][0]),int(F[2][0])]
	# return F
	
# a = 0.05
# b = 0.05
# c = 0.05

# ra = 0.03
# rb = 0.03
# lz = 0.06

# NO_DOTS = 10
# LAPS = 1

# u = np.transpose(np.linspace(0,2*LAPS*np.pi,NO_DOTS))
# v = np.transpose(np.linspace(0,lz,NO_DOTS))

# for i in range(0,NO_DOTS):
	# D = CILINDRO_ESFERICO(a,b,c,ra,rb,lz,u[i],v[i])
	# print(D)
