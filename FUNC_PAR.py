import numpy as np

class parametric_function():
	def __init__(self, a, b, c, nopxm = 160000, nopym = 200000, nopzm = 200000):
		self.a = a
		self.b = b
		self.c = c
		self.nopxm = nopxm
		self.nopym = nopym
		self.nopzm = nopym
		
		
	def CILINDRO_ESFERICO(self,ra,rb,lz,u,v):
		F = np.zeros((3,1))
		F = [[(self.a - ra*np.cos(u))*self.nopxm],[(self.c + v)*self.nopym],[(self.b - rb*np.sin(u))*self.nopzm]]
		F = [int(F[0][0]),int(F[1][0]),int(F[2][0])]
		return F
    
	def LINEA_RECTA(self,P1,P2,t):
		F = np.zeros((3,1))
		F = [[(P1[0] + t*(P2[0]-P1[0]))*self.nopxm],[(P1[1] + t*(P2[1]-P1[1]))*self.nopym],[(P1[2] + t*(P2[2]-P1[2]))*self.nopzm]]
		F = [int(F[0][0]),int(F[1][0]),int(F[2][0])]
		return F
    
	def CIRCULO(self,r,t):
		F = np.zeros((3,1))
		F = [[self.a + r*np.sin(t)],[self.c*self.nopxm],[self.b + r*np.cos(t)]]
		F = [int(F[0][0]),int(F[1][0]),int(F[2][0])]
		return F

	def ESPIRAL_ARQUIMIDES(self,t1,t):
		F = np.zeros((3,1))
		F = [[(self.a + t1*np.cos(t))*self.nopxm],[self.c*self.nopym],[(self.b + t1*np.sin(t))*self.nopzm]]
		F = [int(F[0][0]),int(F[1][0]),int(F[2][0])]
		return F
    
	def ESFERA(self,r,u,v):
		F = np.zeros((3,1))
		F = [[(self.a + r*np.cos(u)*np.cos(v))*self.nopxm],[(self.c + r*np.sin(v))*self.nopym],[(self.b + r*np.sin(u)*np.cos(v))*self.nopzm]]
		F = [int(F[0][0]),int(F[1][0]),int(F[2][0])]
		return F 

