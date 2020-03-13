from tkinter import *
import INT_Menubar as Mb
import F_TLV493D as tlv

window = Tk()

TLV = tlv.tlv_sensor()
icon = PhotoImage(file = "3DINTERSYS.ico")
window.title("3DINTERSYS")
#window.iconbitmap(icon)
window.iconphoto(False, icon)
window.geometry('800x500')


Mb.Menubar(window)

def measures_tlv():
	T_MX.delete(CURRENT,END)
	Nomeasures = E_NM.get()
	data_m = TLV.READ(int(Nomeasures))
	V12 = TLV.CONV(data_m)
	T_MX.insert(INSERT,"Bx = " + str(V12[0][0]) + " Gauss \n")
	T_MX.insert(INSERT,"By = " + str(V12[0][1]) + " Gauss \n")
	T_MX.insert(INSERT,"Bz = " + str(V12[0][2]) + " Gauss \n")
	return -1
	

# lbl = Label(window, text = "Hello",font=("Times New Roman",10))
# lbl.grid(column=0,row=0)

# Resolution


# Movement buttons
Bx_l = Button(window, text = "L", fg = "blue", font=("Times New Roman",10))
Bx_l.place(x = 70, y = 90, height = 20, width = 20)
Bx_r = Button(window, text = "R", font=("Times New Roman",10))
Bx_r.place(x = 110, y = 90, height = 20, width = 20)
By_u = Button(window, text = "U", font=("Times New Roman",10))
By_u.place(x = 90, y = 70, height = 20, width = 20)
By_d = Button(window, text = "D", font=("Times New Roman",10))
By_d.place(x = 90, y = 110, height = 20, width = 20)
Bz_b = Button(window, text = "B", font=("Times New Roman",10))
Bz_b.place(x = 70, y = 130, height = 20, width = 20)
Bz_f = Button(window, text = "F", font=("Times New Roman",10))
Bz_f.place(x = 110, y = 130, height = 20, width = 20)

# Movement input 
lx = Label(window, text = "X (mm)", font=("Times New Roman",10))
lx.place(x = 140, y = 70, height = 20, width = 50)
ly = Label(window, text = "Y (mm)", font=("Times New Roman",10))
ly.place(x = 140, y = 100, height = 20, width = 50)
lz = Label(window, text = "Z (mm)", font=("Times New Roman",10))
lz.place(x = 140, y = 130, height = 20, width = 50)

Sbx_l = Spinbox(window, from_=-200, to=200, font=("Times New Roman",10))
Sbx_l.icursor(15)
Sbx_l.place(x = 200, y = 70, height = 20, width = 50)
Sby_l = Spinbox(window, from_=-200, to=200, font=("Times New Roman",10))
Sby_l.place(x = 200, y = 100, height = 20, width = 50)
Sbz_l = Spinbox(window, from_=-200, to=200, font=("Times New Roman",10))
Sbz_l.place(x = 200, y = 130, height = 20, width = 50)

# Configuration
CONFG = Label(window, text = "CONFIGURATION", font=("Times New Roman",12))
CONFG.place(x = 80, y = 160, height = 20, width = 150)

L_MD = Label(window, text = "Mode", font=("Times New Roman",10))
L_MD.place(x = 80, y = 190, height = 20, width = 50)
MD = Spinbox(window, text = "Mode", values = ("mode 1","mode 2"), font=("Times New Roman",10))
MD.place(x = 200, y = 190, height = 20, width = 70)

L_R = Label(window, text = "Resolution (mm)", font=("Times New Roman",10))
L_R.place(x = 80, y = 220, height = 20, width = 110)
R = Spinbox(window,from_=0.1, to = 10, increment = 0.1, font=("Times New Roman",10))
R.place(x = 200, y = 220, height = 20, width = 70)

L_R = Label(window, text = "Velocity (mm)", font=("Times New Roman",10))
L_R.place(x = 80, y = 250, height = 20, width = 100)
R = Spinbox(window,values = ("Ultra Low","Low","Normal","High","Ultra High"), font=("Times New Roman",10))
R.place(x = 200, y = 250, height = 20, width = 90)

# Magnetic Field Measure
L_MFM = Label(window,text = "Magnetic Field Measurement", font=("Times New Roman",12))
L_MFM.place(x = 80, y = 280, height = 20, width = 200)
L_NM = Label(window,text = "No. Measures", font=("Times New Roman",10))
L_NM.place(x = 80, y = 300, height = 20, width = 90)
E_NM = Entry(window,font=("Times New Roman",10))
E_NM.insert(0,1)
E_NM.place(x = 170, y = 300, height = 20, width = 40)
B_M = Button(window,text = "Measure", fg = "blue",font=("Times New Roman",10), command = measures_tlv)
B_M.place(x = 210, y = 300, height = 20, width = 50)

T_MX = Text(window,font=("Times New Roman",10))
T_MX.place(x = 80, y = 330, height = 60, width = 180)


# Parametric curve or matrix 
RB_PC = Radiobutton(window, text = "Parametric curves", font=("Times New Roman",10), value=1)
RB_PC.place(x = 320, y = 70, height = 20, width = 120)
RB_MM = Radiobutton(window, text = "Manual mode", font=("Times New Roman",10), value=2)
RB_MM.place(x = 320, y = 100, height = 20, width = 100)
RB_D = Radiobutton(window, text = "Dianamic", font=("Times New Roman",10), value=3)
RB_D.place(x = 320, y = 130, height = 20, width = 80)


# Run Pause Stop Buttons 
Run = Button(window, text = "Run", fg = "green", font=("Times New Roman",10))
Run.place(x = 600, y = 350, height = 20, width = 50)
Pause = Button(window, text = "Pause", fg = "blue", font=("Times New Roman",10))
Pause.place(x = 650, y = 350, height = 20, width = 50)
Stop = Button(window, text = "Stop", fg = "Red", font=("Times New Roman",10))
Stop.place(x = 700, y = 350, height = 20, width = 50)


window.mainloop()




