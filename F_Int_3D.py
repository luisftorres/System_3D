# Universidad de Guanajuato
# Lftorresavitia@gmail.com
# Control Code for the measurement 3D system

#Python Libraries
import numpy as np
import serial
import io
import wiringpi as pi
# LFTA Functions
import F_TLV493D as tlv
import F_FUNC_1 as PFUN

# class
TLV = tlv.tlv_sensor()		# TLV493D sensor class
PARF = PFUN.pfunc()			# Parametric Functions class


class system_3d():
	def __init__(self):
		
		# GPIO
		self.PIN_STAR 	= 	17	#
		self.PIN_END 	= 	27	#
		self.END 		= 	22	#
		
		# port configuration
		self.port = serial.Serial("/dev/ttyUSB0", baudrate=115200, timeout=2.0, parity = serial.PARITY_NONE)
		self.sio = io.TextIOWrapper(io.BufferedRWPair(port, port))
		
		#Configure GPIO
		pi.wiringPiSetupGpio()
		pi.pinMode(self.PIN_STAR,0)		#INPUT
		pi.pinMode(self.PIN_END,1)		#OUTPUT
		pi.pinMode(self.END,0)			#INPUT
		pi.digitalWrite(self.PIN_END,0)	# SET PIN TO 0 for not have errors
		
		## Strings to make the WORD 
		self.rr 		= b'35'		# b'35' and b'155'
		self.MODO 		= b'2'		# b'1' and b'2'
		self.aa 		= b'@'		# divide the strings in the word that go to the arduino by serial

		# punto 1
		self.X1 		= b'3694'
		self.Y1 		= b'0'
		self.Z1 		= b'0'

		# punto 2
		self.X2 		= b'12127'
		self.Y2 		= b'0'
		self.Z2 		= b'0'

		# Resolution
		self.R_Factor 	= b'10'
		self.V_measure 	= b'150'
		self.V_manual 	= b'150'
		
		# Word to send arduino
		self.WORD 		= self.rr + b'@' + self.MODO + b'@' \
						+ self.X1 + b'@' + self.Y1 + b'@' + self.Z1 \
						+ b'@' + self.X2 + b'@' + self.Y2 + b'@' \
						+ self.Z2 + b'@' + self.R_Factor + b'@' \
						+ self.V_measure + b'@' + self.V_manual + b'@*'
		
		# Number of data to read
		self.NDR = 5
		
		# Velocity in parametric curve
		self.vx = b'40'
		self.vy = b'40'
		self.vz = b'40'
		
		
	def create_file(self,name):
		# Create a File.txt
		print("Input the file name:	")
		name = input()
		self.f = open(name + ".csv","w+")	# Object file
		self.f.write("REG0,REG1,REG2,REG3,REG4,REG5,REG6\n")	#header
	
	def rwup(self,s_timewait = 150):	#read word until plus 
		self.port.timeout = s_timewait
		while True:
			P = self.port.read_until(b'*',s_timewait)
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
		return -1
		
	def RMDS(self):		# Read Matrix Data Sensor
		i = 0	
		# Read Matriz Data Sensor
		while True:
			pi.digitalWrite(self.PIN_END,1)
			tlv.time.sleep(0.0005)
			while True:
				if the.digitalRead(self.END) == 1:
					break
				if pi.digitalRead(self.PIN_STAR) == 1:
					pi.digitalWrite(self.PIN_END,0)
					tlv.time.sleep(0.010)
					data = TLV.READ(self.NDR)
					for i in range(0,self.NDR):
						for j in range(0,7):
							self.f.write(format(int(data[i][j])) + ",")
						self.f.write("\n")
					tlv.time.sleep(0.10)	
					break
			if pi.digitalRead(self.END) == 1:
			break
					
	def Parametric_curve(self):
		for w in range(0,1):
			for z in range(0,PARF.NO_DOTS):
				D = fun.ESFERA(z)
				print(D)
				
				pi.digitalWrite(self.PIN_END,1)
				Px = b'%d'%D[0]
				Py = b'%d'%D[1]
				Pz = b'%d'%D[2]
				
				# position word "POS" to send by serial port
				POS = Px + b'@' + Py + b'@' + Pz + b'@' + vx + b'@' + vy + b'@' + vz + b'@*\r\n'
				self.port.write(POS)
				while True:
					if pi.digitalRead(self.PIN_STAR) == 1:
						pi.digitalWrite(self.PIN_END,0)
						tlv.time.sleep(0.001)
						data = tlv.READ(self.NDR)
						pi.digitalWrite(self.PIN_END,1)
						for i in range(0,self.NDR):
							for j in range(0,7):
								self.f.write(format(int(data[i][j])) + ",")
							self.f.write("\n")	
						break
			ce = ce - 0.001
		pi.digitalWrite(END,0)
		tlv.time.sleep(1)

	# System process
	def system_process(self):
		# Configure sensor
		tlv.CONF()
		
		# Start presentation system
		for i in range(0,1):
			Introduction = sio.readline()
			print(Introduction)
		
		# Start System
		pi.pinMode(END,1)   # OUTPUT
		pi.digitalWrite(END,1)
		port.write(WORD + b'\r\n')
		tlv.time.sleep(1)
		
		
		
		# if Mode = 1 is to move the system manually and Mode = 2 is to make the parametric curve
		if self.rr == b'35':
			
			pi.pinMode(END,1)   # OUTPUT
			pi.digitalWrite(END,1)
			
			if self.MODO == b'1':
				self.rwup(300)
			if self.MODO == b'2':
		
		# if Mode = 1 set the points manually and Mode = 2 the points are given by the user
		if self.rr == b'155':
			SS = self.sio.readline()
			print(SS) 	
			
			self.rwup(300)
			self.RMDS()
		
		port.timeout = 1.0
		f.close()	
		port.close()	




