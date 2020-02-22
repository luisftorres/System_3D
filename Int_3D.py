import TLV493D as tlv
import FUNC_1 as fun
import serial
import numpy as np
import io
import wiringpi as pi
import get


# Create a File.txt
print("Input the file name:	")
name = input()
f = open(name + ".csv","w+")
f.write("REG0,REG1,REG2,REG3,REG4,REG5,REG6\n")

#Configure GPIO
pi.wiringPiSetupGpio()
PIN_STAR = 	17
PIN_END = 	27
END = 22
pi.pinMode(PIN_STAR,0)
pi.pinMode(PIN_END,1)
pi.pinMode(END,0)
pi.digitalWrite(PIN_END,0)


# Configure sensor
tlv.CONF()


port = serial.Serial("/dev/ttyUSB0", baudrate=115200, timeout=1.0, parity = serial.PARITY_NONE)
sio = io.TextIOWrapper(io.BufferedRWPair(port, port))

rr = b'35'
aa = b'@'
# punto 1
X1 = b'3694'
Y1 = b'0'
Z1 = b'0'

# punto 2
X2 = b'12127'
Y2 = b'0'
Z2 = b'0'

# Resolution
R_Factor = b'10'
V_measure = b'150'
V_manual = b'150'
MODO = b'1'
WORD = rr + aa + MODO + aa + X1 + aa + Y1 + aa + Z1 + aa + X2 + aa + Y2 + aa + Z2 + aa + R_Factor + aa + V_measure + aa + V_manual + b'@*'
#print(WORD)
# Start presentation system

for i in range(0,1):
	Introduction = sio.readline()
	print(Introduction)


# Start System
pi.pinMode(END,1)   # OUTPUT
pi.digitalWrite(END,1)
port.write(WORD + b'\r\n')
#tlv.time.sleep(1)
	

# Parametrics curves 
a = 0.1
b = 0.06
c = 0.005

ra = 0.05
rb = 0.05
lz = 0.05
P1 = [0.01,0.01,0.01]
P2 = [0.1,0.1,0.1]
LAPS = 10
NO_DOTS = 10000
t = np.transpose(np.linspace(0,1,NO_DOTS))

u = np.transpose(np.linspace(0,2*LAPS*np.pi,NO_DOTS))
v = np.transpose(np.linspace(0,lz,NO_DOTS))

tlv.time.sleep(1)
vx = b'40'
vy = b'40'
vz = b'40'
if rr == b'35':
	pi.pinMode(END,1)   # OUTPUT
	pi.digitalWrite(END,1)
	
	#Home = port.read_until(b'*',150)
	#print(Home)
	for z in range(0,NO_DOTS):
		#D = fun.CILINDRO_ESFERICO(a,b,c,ra,rb,lz,u[z],v[z])
		D = fun.LINEA_RECTA(P1,P2,t[z])
		print(D)
		pi.digitalWrite(PIN_END,1)
		Px = b'%d'%D[0]
		Py = b'%d'%D[1]
		Pz = b'%d'%D[2]
		POS = Px + aa + Py + aa + Pz + aa + vx + aa + vy + aa + vz + b'@*'
		port.write(POS + b'\r\n')
		while True:
			if pi.digitalRead(PIN_STAR) == 1:
				pi.digitalWrite(PIN_END,0)
				#tlv.time.sleep(0.1)
				data = tlv.READ(5)
				pi.digitalWrite(PIN_END,1)
				for i in range(0,5):
					for j in range(0,7):
						f.write(format(int(data[i][j])) + ",")
					f.write("\n")	
				break
	pi.digitalWrite(END,0)
	tlv.time.sleep(1)

if rr == b'155':
	SS = sio.readline()
	print(SS) 

	# set the point 1 and the point 2
	port.timeout = 100.0
	while True:
		
		P = port.read_until(b'*',150)
		i = 0
		h = ''
		while (i < len(P)):
			if chr(P[i]) == '@':
				print(h)
				h = ''
			else:
				h = h + chr(P[i])
			if chr(P[i]) == '*':
				break
			i = i + 1
		if chr(P[i-1]) == '+':
			break
	i = 0	
	# Read Matriz Data Sensor
	while True:
		pi.digitalWrite(PIN_END,1)
		tlv.time.sleep(0.0005)
		while True:
			if pi.digitalRead(END) == 1:
				break
			if pi.digitalRead(PIN_STAR) == 1:
				pi.digitalWrite(PIN_END,0)
				tlv.time.sleep(0.010)
				data = tlv.READ(5)
				for i in range(0,5):
					for j in range(0,7):
						f.write(format(int(data[i][j])) + ",")
					f.write("\n")
				tlv.time.sleep(0.10)	
				break
		
		if pi.digitalRead(END) == 1:
			break
port.timeout = 1.0
f.close()	
port.close()
