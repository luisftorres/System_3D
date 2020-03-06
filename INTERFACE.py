from tkinter import *
import INT_Menubar as Mb

window = Tk()

icon = PhotoImage(file = "3DINTERSYS.ico")
window.title("3DINTERSYS")
#window.iconbitmap(icon)
window.iconphoto(False, icon)
window.geometry('500x500')


Mb.Menubar(window)



# lbl = Label(window, text = "Hello",font=("Times New Roman",10))
# lbl.grid(column=0,row=0)



B1 = Button(window, text = "B")
B1.pack()
#B1.pack(anchor = NW, width = 12, height = 12,column = 0,row=1)
window.mainloop()




