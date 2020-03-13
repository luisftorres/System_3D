import TLV493D as tlv
import serial
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
PIN_STAR = 	19
PIN_END = 	26
END = 20
pi.pinMode(PIN_STAR,0)
pi.pinMode(PIN_END,1)
pi.pinMode(END,0)
pi.digitalWrite(PIN_END,0)


# Configure sensor
tlv.CONF()


port = serial.Serial("/dev/ttyUSB0", baudrate=115200, timeout=1.0, parity = serial.PARITY_NONE)
sio = io.TextIOWrapper(io.BufferedRWPair(port, port))

rr = b'155'
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
V_measure = b'30'
V_manual = b'100'
MODO = b'1'
WORD = rr + aa + MODO + aa + X1 + aa + Y1 + aa + Z1 + aa + X2 + aa + Y2 + aa + Z2 + aa + R_Factor + aa + V_measure + aa + V_manual + b'@*'
print(WORD)
# Start presentation system

for i in range(0,5):
	Introduction = sio.readline()
	print(Introduction)


# Start System
port.write(WORD + b'\r\n')
tlv.time.sleep(1)
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
	#tlv.time.sleep(0.0005)
	while True:
		if pi.digitalRead(END) == 1:
			break
		if pi.digitalRead(PIN_STAR) == 1:
			pi.digitalWrite(PIN_END,0)
			#tlv.time.sleep(0.010)
			data = tlv.READ(5)
			for i in range(0,5):
				for j in range(0,7):
					f.write(format(int(data[i][j])) + ",")
				f.write("\n")	
			break
	
	if pi.digitalRead(END) == 1:
		break
port.timeout = 1.0
f.close()	
port.close()
