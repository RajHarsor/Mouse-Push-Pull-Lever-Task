# Libraries
import tkinter as tk
from tkinter import ttk, scrolledtext, simpledialog, filedialog
import serial
from datetime import datetime
from serial.tools import list_ports
import threading
import matplotlib.pyplot as plt
from matplotlib.gridspec import GridSpec
import os
import time
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg

# Figure formatting parameters
plt.rcParams['pdf.fonttype'] = 42
plt.rcParams['ps.fonttype'] = 42

#Dialog pop-ups for user input
class EntryDialog(simpledialog.Dialog):
    def __init__(self, parent, title, fields, monitor_types=None):
        self.fields = fields
        self.monitor_types = monitor_types
        super().__init__(parent, title=title)

    def body(self, master):
        for i, field in enumerate(self.fields):
            tk.Label(master, text=field).grid(row=i, column=0, sticky="e")
            if field == "Monitor Type" and self.monitor_types:
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

        #Notebook format (tabs)
        self.notebook = ttk.Notebook(self.root)
        self.notebook.pack(fill="both", expand=True)
        #Tab 1
        self.frame1 = ttk.Frame(self.notebook)
        self.notebook.add(self.frame1, text="Trials")
        #Tab 2
        self.frame2 = ttk.Frame(self.notebook)
        self.notebook.add(self.frame2, text="Coordinates")
        #Tab 3
        self.frame3 = ttk.Frame(self.notebook)
        self.notebook.add(self.frame3, text="Timer + Save Data")

        #Live Data Displays -- Tab 1
        #Reaction Times
        self.fig3, self.ax3 = plt.subplots(figsize = (7, 3))
        self.canvas3 = FigureCanvasTkAgg(self.fig3, master=self.frame1)
        self.canvas3.get_tk_widget().place(x = 1125, y=70)
        self.canvas3.draw()

        self.fig4, self.ax4 = plt.subplots(figsize = (7, 3))
        self.canvas4 = FigureCanvasTkAgg(self.fig4, master=self.frame1)
        self.canvas4.get_tk_widget().place(x = 1125, y=380)
        self.canvas4.draw()

        #Live Data Displays -- Tab 2
        ## X and Y Coordinates over time plot
        self.fig1, self.ax1 = plt.subplots()
        self.canvas1 = FigureCanvasTkAgg(self.fig1, master=self.frame2)
        self.canvas1.get_tk_widget().place(x = 1125, y=0)
        self.canvas1.draw()
        ## X and Y Trajectory plot
        self.fig2, self.ax2 = plt.subplots()
        self.canvas2 = FigureCanvasTkAgg(self.fig2, master=self.frame2)
        self.canvas2.get_tk_widget().place(x = 1125, y=500)
        self.canvas2.draw()

        #Mouse information variables + paradigm
        self.mouse_name = ""
        self.date = ""
        self.paradigm = ""
        self.training_day = ""
        self.rig_name = ""
        self.monitor_type = ""

        #Variables for threshold information - Sent to the Coordinates Teensy
        ## Push/Pull Variables
        self.push_threshold = None
        self.pull_threshold = None
        self.rest_lowerbound = None
        self.rest_upperbound = None
        ## Additional Push/Down Variables
        self.down_threshold = None
        self.rest_lowerboundX = None
        self.rest_upperboundX = None
        self.rest_lowerboundY = None
        self.rest_upperboundY = None

        #Variables for the Behavior Teensy
        self.isi_lowerbound = None
        self.isi_upperbound = None
        self.timeOutTime = None
        self.arraySize = None

        self.create_widgets()


