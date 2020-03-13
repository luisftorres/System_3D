import smbus
import time
import numpy as np
bus = smbus.SMBus(3)
address = 0x1f          # 0x5e esta direccion de sensor se otorga despues del hard reset
adrress_set = 0x5e      # 0x4a esta direccion de sensor se fija en la configuracion 
LPM = 0x05 
FPM = 0x06
MCM = 0x07              # 0xff Master Controled Mode
t_wait = 0.0005;
mode_sensor = MCM
cols = 7
fils = 10
data = np.zeros((fils,cols))


def HardReset():
    bus.write_quick(0x00)
    return -1
    
def Configure(addr,value):
    bus.write_byte_data(addr,0x00,value)
    return -1

def read_1(addr):
        data = bus.read_i2c_block_data(addr,0x00,7)
        return data


HardReset()
time.sleep(t_wait)
Configure(address, mode_sensor)
time.sleep(t_wait)
i = 0
a = 0
while (i < fils):
        
        data[i] = read_1(adrress_set)      #this returns the value as a byte between 0 and 255. 
        time.sleep(t_wait)
        if all(data[i] == data[i-1]):
            HardReset()
            Configure(address,mode_sensor)
            a = a + 1                       # No. de veces que el sensor se traba
        else:
            i = i + 1


i = 0
while (i < fils):
    print(data[i])
    i = i + 1

print(a)
