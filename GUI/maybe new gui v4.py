import tkinter as tk
from tkinter import ttk, scrolledtext, simpledialog, filedialog
import serial
from datetime import datetime
from serial.tools import list_ports
import threading
import multiprocessing
import matplotlib.pyplot as plt
from matplotlib.gridspec import GridSpec
import os
import time
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg


#Figure formatting
plt.rcParams['pdf.fonttype'] = 42
plt.rcParams['ps.fonttype'] = 42

class EntryDialog(simpledialog.Dialog):
    def __init__(self, parent, title, fields, monitor_types):
        self.fields = fields
        self.monitor_types = monitor_types
        super().__init__(parent, title=title)

    def body(self, master):
        for i, field in enumerate(self.fields):
            tk.Label(master, text=field).grid(row=i, column=0, sticky="e")
            if field == "Monitor Type":
                setattr(self, f"{field}_entry", ttk.Combobox(master, values=self.monitor_types))
            else:
                setattr(self, f"{field}_entry", tk.Entry(master))
            getattr(self, f"{field}_entry").grid(row=i, column=1)
        return getattr(self, f"{self.fields[0]}_entry")  # Return the first entry widget

    def apply(self):
        self.result = [getattr(self, f"{field}_entry").get() for field in self.fields]


class EntryDialog2(simpledialog.Dialog):
    def __init__(self, parent, title, fields, monitor_types):
        self.fields = fields
        self.monitor_types = monitor_types
        super().__init__(parent, title=title)

    def body(self, master):
        for i, field in enumerate(self.fields):
            tk.Label(master, text=field).grid(row=i, column=0, sticky="e")
            if field == "Monitor Type":
                setattr(self, f"{field}_entry", ttk.Combobox(master, values=self.monitor_types))
            else:
                setattr(self, f"{field}_entry", tk.Entry(master))
            getattr(self, f"{field}_entry").grid(row=i, column=1)
        return getattr(self, f"{self.fields[0]}_entry")  # Return the first entry widget

    def apply(self):
        self.result = [getattr(self, f"{field}_entry").get() for field in self.fields]

class EntryDialog3(simpledialog.Dialog):
    def __init__(self, parent, title, fields, monitor_types):
        self.fields = fields
        self.monitor_types = monitor_types
        super().__init__(parent, title=title)

    def body(self, master):
        for i, field in enumerate(self.fields):
            tk.Label(master, text=field).grid(row=i, column=0, sticky="e")
            if field == "Monitor Type":
                setattr(self, f"{field}_entry", ttk.Combobox(master, values=self.monitor_types))
            else:
                setattr(self, f"{field}_entry", tk.Entry(master))
            getattr(self, f"{field}_entry").grid(row=i, column=1)
        return getattr(self, f"{self.fields[0]}_entry")  # Return the first entry widget

    def apply(self):
        self.result = [getattr(self, f"{field}_entry").get() for field in self.fields]

class SerialMonitorGUI:
    def __init__(self, root):
        self.root = root
        self.root.title("Serial Monitor")

        #Initialize Notebook Format
        self.notebook = ttk.Notebook(self.root)
        self.notebook.pack(fill='both', expand=True)

        #Initialize Frames
        self.frame1 = ttk.Frame(self.notebook)
        self.frame2 = ttk.Frame(self.notebook)
        self.frame3 = ttk.Frame(self.notebook)

        #Add Frames to Notebook
        self.notebook.add(self.frame1, text='Trials')
        self.notebook.add(self.frame2, text='Coordinates')
        self.notebook.add(self.frame3, text='Timer')

        #Variables for program type
        self.program_type = None

        # Additional variables for mouse information
        self.mouse_name = ""
        self.date = ""
        self.paradigm = ""
        self.training_day = ""
        self.rig_name = ""
        self.monitor_type = ""

        # Additional variables for threshold information
        self.push_threshold = None
        self.pull_threshold = None
        self.rest_lowerbound = None
        self.rest_upperbound = None

        # Additional variables for push/down
        self.down_threshold = None
        self.lowerboundX = None
        self.upperboundX = None
        self.lowerboundY = None
        self.upperboundY = None

        #Variables for ISI information
        self.isi_lowerbound = None
        self.isi_upperbound = None

        self.create_widgets()

# Create figures for live plotting (Coordinates)
        self.fig1, self.ax1 = plt.subplots()
        self.canvas1 = FigureCanvasTkAgg(self.fig1, master=self.frame2)
        self.canvas1.get_tk_widget().place(x = 1125, y=0)
        self.canvas1.draw()

        self.fig2, self.ax2 = plt.subplots()
        self.canvas2 = FigureCanvasTkAgg(self.fig2, master=self.frame2)
        self.canvas2.get_tk_widget().place(x = 1125, y=500)
        self.canvas2.draw()

        #Create figures for live plotting (Trials)
        #Reaction Times
        self.fig3, self.ax3 = plt.subplots(figsize = (7, 3))
        self.canvas3 = FigureCanvasTkAgg(self.fig3, master=self.frame1)
        self.canvas3.get_tk_widget().place(x = 1125, y=70)
        self.canvas3.draw()

        self.fig4, self.ax4 = plt.subplots(figsize = (7, 3))
        self.canvas4 = FigureCanvasTkAgg(self.fig4, master=self.frame1)
        self.canvas4.get_tk_widget().place(x = 1125, y=380)
        self.canvas4.draw()