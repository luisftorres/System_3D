from tkinter import *
def Menubar(window):
		
	
	def donothing():
		filewin = Toplevel(window)
		button = Button(filewin, text="Do nothing button")
		button.pack()
		
		
	 
	menubar = Menu(window)
	filemenu = Menu(menubar, tearoff=0)
	filemenu.add_command(label = "New", command = donothing,font=("Times New Roman",10))
	filemenu.add_command(label = "Open", command = donothing, font=("Times New Roman",10))
	filemenu.add_command(label = "Save", command = donothing, font=("Times New Roman",10))
	filemenu.add_command(label = "Save as...", command = donothing, font=("Times New Roman",10))
	filemenu.add_command(label = "Close", command = donothing, font=("Times New Roman",10))

	filemenu.add_separator() 

	filemenu.add_command(label="Exit", command=window.quit, font=("Times New Roman",10), background = "red")
	menubar.add_cascade(label="File", menu=filemenu, font=("Times New Roman",10))
	
	
	editmenu = Menu(menubar, tearoff=0)
	editmenu.add_command(label="Undo", command=donothing, font=("Times New Roman",10))

##
	editmenu.add_separator()

	editmenu.add_command(label="Cut", command=donothing, font=("Times New Roman",10))
	editmenu.add_command(label="Copy", command=donothing, font=("Times New Roman",10))
	editmenu.add_command(label="Paste", command=donothing, font=("Times New Roman",10))
	editmenu.add_command(label="Delete", command=donothing, font=("Times New Roman",10))
	editmenu.add_command(label="Select All", command=donothing, font=("Times New Roman",10))

	menubar.add_cascade(label="Edit", menu=editmenu, font=("Times New Roman",10))
	
##	

	confmenu = Menu(menubar, tearoff=0)
	menubar.add_cascade(label="Configuration", menu=confmenu, font=("Times New Roman",10))

##	
	optionsmenu = Menu(menubar, tearoff=0)
	menubar.add_cascade(label="Options", menu=optionsmenu, font=("Times New Roman",10))
	
##
	toolsmenu = Menu(menubar, tearoff=0)
	menubar.add_cascade(label="Tools", menu=toolsmenu, font=("Times New Roman",10))

##
	
	helpmenu = Menu(menubar, tearoff=0)
	helpmenu.add_command(label="Help Index", command=donothing, font=("Times New Roman",10))
	helpmenu.add_command(label="About...", command=donothing, font=("Times New Roman",10))
	menubar.add_cascade(label="Help", menu=helpmenu, font=("Times New Roman",10))

	window.config(menu=menubar)
