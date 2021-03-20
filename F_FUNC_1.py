# Universidad de Guanajuato
# Universidad Autónoma de Sinaloa
# Lftorresavitia@gmail.com
# Parametric curves fucntion

import numpy as np

# x (Right and left).  y (front and begind) and z (up and down)
# Parametric Functions
class pfunc():
    def __init__(self):
        #Number of pixels by meter
        self.nopxm = 160000
        self.nopym = 200000
        self.nopzm = 200000
        # rect line in 3D
        self.P1 = [0.01,0.01,0.05]
        self.P2 = [0.1,0.1,0.1]
        # central point or first point "a -> x. b -> z and c -> y"
        self.a = 0.05       
        self.b = 0.05
        self.c = 0.05
        # radious 
        self.ra = 0.03
        self.rb = 0.03
        self.rc = 0.03
        # lenght
        self.l = 0.05
        # Number of laps
        self.LAPS = 50
        # Number of dots measured
        self.NO_DOTS = 1000
        # variable t for longitude
        self.t = np.transpose(np.linspace(0,1,self.NO_DOTS)) # for use in line
        # variable u for laps
        self.u = np.transpose(np.linspace(0,2*self.LAPS*np.pi,self.NO_DOTS))
        # variable v for -pi/2 to pi/2; represent all the esfera
        self.v = np.transpose(np.linspace(-np.pi/2,np.pi/2,self.NO_DOTS))
        
    def CILINDRO_ESFERICO(self,C_NUM):
        F = np.zeros((3,1))
        F = [[(self.a - self.ra*np.cos(self.u[C_NUM]))*self.nopxm],[(self.c + self.l*self.t[C_NUM])*self.nopym],[(self.b - self.rb*np.sin(self.u[C_NUM]))*self.nopzm]]
        F = [int(F[0][0]),int(F[1][0]),int(F[2][0])]
        return F
        
    def LINEA_RECTA(self,C_NUM):
        F = np.zeros((3,1))
        F = [[(self.P1[0] + self.t[C_NUM]*(self.P2[0]-self.P1[0]))*self.nopxm],[(self.P1[1] + self.t[C_NUM]*(self.P2[1]-self.P1[1]))*self.nopym],[(self.P1[2] + self.t[C_NUM]*(self.P2[2]-self.P1[2]))*self.nopzm]]
        F = [int(F[0][0]),int(F[1][0]),int(F[2][0])]
        return F
        
    def CIRCULO(self,C_NUM):
        F = np.zeros((3,1))
        F = [[(self.a + self.ra*np.cos(self.u[C_NUM]))*self.nopxm],[(self.c)*self.nopym],[(self.b + self.ra*np.sin(self.u[C_NUM]))*self.nopzm]]
        F = [int(F[0][0]),int(F[1][0]),int(F[2][0])]
        return F

    def ESPIRAL_ARQUIMIDES(self,C_NUM):
        F = np.zeros((3,1))
        F = [[(self.a + self.l*self.t[C_NUM]*np.cos(self.u[C_NUM]))*self.nopxm],[self.c*self.nopym],[(self.b + self.l*self.t[C_NUM]*np.sin(self.u[C_NUM]))*self.nopzm]]
        F = [int(F[0][0]),int(F[1][0]),int(F[2][0])]
        return F
        
    def ESFERA(self,C_NUM):
        F = np.zeros((3,1))
        F = [[(self.a + self.ra*np.cos(self.u[C_NUM])*np.cos(self.v[C_NUM]))*self.nopxm],[(self.c + self.ra*np.sin(self.v[C_NUM]))*self.nopym],[(self.b + self.ra*np.sin(self.u[C_NUM])*np.cos(self.v[C_NUM]))*self.nopzm]]
        F = [int(F[0][0]),int(F[1][0]),int(F[2][0])]
        return F    

