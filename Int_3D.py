# Universidad de Guanajuato
# Lftorresavitia@gmail.com
# Control Code for the measurement 3D system

import TLV493D as tlv			# Measurement sensor function "Made by LFTA"
import FUNC_1 as fun			# Parametris functions function "Made by LFTA"
import serial					# Serial protocolo 
import numpy as np				# Math 
import io						# "add information"
import wiringpi as pi


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
port = serial.Serial("/dev/ttyUSB0", baudrate=115200, timeout=2.0, parity = serial.PARITY_NONE)
sio = io.TextIOWrapper(io.BufferedRWPair(port, port))

rr = b'155'						# if is b'35' is to parametric curves and if is b'155' 
								# is manual for an nxn matrix
aa = b'@'						# to separate words in the serial line to arduiono
# point 1
X1 = b'3694'
Y1 = b'0'
Z1 = b'0'

# point 2
X2 = b'12127'
Y2 = b'0'
Z2 = b'0'

# Resolution
R_Factor = b'10'				# Factor resolution Resolution = R_Factor*motor_resolution (16 for x and 20 for yz)
V_measure = b'150'				# time into enable and disanable motor step to give velocity	
V_manual = b'150'				# 
MODO = b'2'						# Use b'1'	to get the current position and b'2' make measurement parametirc curve			
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
a = 0.09321
b = 0.07241
c = 0.06768

ra = 0.036
rb = 0.036
lz = 0.08
P1 = [0.01,0.01,0.04]
P2 = [0.1,0.1,0.1]
LAPS = 100
NO_DOTS = 10000
t = np.transpose(np.linspace(0,1,NO_DOTS))

u = np.transpose(np.linspace(0,2*LAPS*np.pi,NO_DOTS))
v = np.transpose(np.linspace(0,lz,NO_DOTS))

# Espiral de arquimides
ae = 0.05
be = 0.05
ce = 0.065
t1e = np.transpose(np.linspace(0,0.04,NO_DOTS))
te = np.transpose(np.linspace(0,30*np.pi,NO_DOTS))


# esfera
a_es = 0.09321
b_es = 0.07241
c_es = 0.06768

r_es = 0.03
u_es = np.transpose(np.linspace(0,2*LAPS*np.pi,NO_DOTS))
v_es = np.transpose(np.linspace(-np.pi/2,np.pi/2,NO_DOTS))


tlv.time.sleep(1)
vx = b'40'
vy = b'40'
vz = b'40'
if rr == b'35':
	pi.pinMode(END,1)   # OUTPUT
	pi.digitalWrite(END,1)
	
	if MODO == b'1':
		port.timeout = 300.0
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
	if MODO == b'2':			
		for w in range(0,1):
			for z in range(0,NO_DOTS):
				D = fun.CILINDRO_ESFERICO(a,b,c,ra,rb,lz,u[z],v[z])
				#D = fun.LINEA_RECTA(P1,P2,t[z])
				#D = fun.ESPIRAL_ARQUIMIDES(ae,be,ce,t1e[z],te[z])
				#D = fun.ESFERA(a_es,b_es,c_es,r_es,u_es[z],v_es[z])
				print(D)
				pi.digitalWrite(PIN_END,1)
				Px = b'%d'%D[0]
				Py = b'%d'%D[1]
				Pz = b'%d'%D[2]
				POS = Px + aa + Py + aa + Pz + aa + vx + aa + vy + aa + vz + b'@*\r\n'
				port.write(POS)
				while True:
					if pi.digitalRead(PIN_STAR) == 1:
						pi.digitalWrite(PIN_END,0)
						tlv.time.sleep(0.001)
						data = tlv.READ(5)
						pi.digitalWrite(PIN_END,1)
						for i in range(0,5):
							for j in range(0,7):
								f.write(format(int(data[i][j])) + ",")
							f.write("\n")	
						break
			ce = ce - 0.001
		pi.digitalWrite(END,0)
		tlv.time.sleep(1)

if rr == b'155':
	SS = sio.readline()
	print(SS) 

	# set the point 1 and the point 2
	port.timeout = 300.0
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
