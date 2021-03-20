import smbus
import time
import numpy as np
#bus = smbus.SMBus(3)
#address = 0x5e     #0x5e    # 0x5e esta direccion de sensor se otorga despues del hard reset
#adrress_set = 0x5e  #0x5e    # 0x4a esta direccion de sensor se fija en la configuracion 
#LPM = 0x05 
#FPM = 0x06
#MCM = 0x07              # 0xff Master Controled Mode
#t_wait = 0.001;
#mode_sensor = MCM
cols = 7

class tlv_sensor():
    def __init__(self, addr_1 = 0x1f, addr_2 = 0x1f, mode_sensor = 0x07, t_wait = 0.001,Bus = 3):
        self.addr_1     = addr_1
        self.addr_2     = addr_2
        self.mode_sensor= mode_sensor
        self.t_wait     = t_wait
        self.bus        = smbus.SMBus(Bus)
        
        
    def HardReset(self):
        self.bus.write_quick(0x00)
        return -1
    
    def Configure(self, addr,value):
        self.bus.write_byte_data(addr,0x00,value)
        return -1

    def read_1(self,addr):
        data = self.bus.read_i2c_block_data(addr,0x00,7)
        return data

    def CONF(self):
        self.HardReset()
        time.sleep(self.t_wait)
        self.Configure(self.addr_1, self.mode_sensor)
        time.sleep(self.t_wait)


    def READ(self,no_read):
        
        data = np.zeros((no_read + 1,cols))
        i = 0
        a = 0
        while (i < no_read + 1):
                
            data[i] = self.read_1(self.addr_2)      #this returns the value as a byte between 0 and 255. 
            time.sleep(self.t_wait)
            if all(data[i] == data[i-1]):
                self.HardReset()
                self.Configure(self.addr_1,self.mode_sensor)
                a = a + 1                       # No. de veces que el sensor se traba
            else:
                i = i + 1
        return data[1:no_read+1]

    def CONV(self,data):
        aux = 0
        M = np.zeros((7,8))
        V12 = np.zeros((3,12))
        
        for z in range(0, 7):
            for k in range(0,8):
                if (data[0][z] !=0):
                    aux = data[0][z]%2
                    data[0][z] = data[0][z]/2
                    M[z][k] = int(aux)
                else:
                    M[z][k] = 0
        V12[0] = [M[4][4],M[4][5],M[4][6],M[4][7],M[0][0],M[0][1],M[0][2],M[0][3],M[0][4],M[0][5],M[0][6],M[0][7]]
        V12[1] = [M[4][0],M[4][1],M[4][2],M[4][3],M[1][0],M[1][1],M[1][2],M[1][3],M[1][4],M[1][5],M[1][6],M[1][7]]
        V12[2] = [M[5][0],M[5][1],M[5][2],M[5][3],M[2][0],M[2][1],M[2][2],M[2][3],M[2][4],M[2][5],M[2][6],M[2][7]]
        
        XYZ = np.zeros((1,3))
        for z in range(0,3):
            r = 0
            for k in range(0,12):
                if(V12[z][k]==0):
                    r = 0
                else:
                    r = 2**k 
                XYZ[0][z] = XYZ[0][z] + r
        for z in range(0,3):
            if(XYZ[0][z]>2047):
                XYZ[0][z] = XYZ[0][z] - 4096 
                XYZ[0][z] = XYZ[0][z]*0.98  
            else:
                XYZ[0][z] = XYZ[0][z]*0.98   
        return XYZ

# Sen = tlv_sensor()
# Sen.CONF()
# X = Sen.READ(5)
# print(X)
