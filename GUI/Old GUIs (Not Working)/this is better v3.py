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
        
        #Additional variables for ISI information
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
        
        #Arrays plotting
        self.fig6, self.ax6 = plt.subplots(figsize = (7, 3))
        self.canvas6 = FigureCanvasTkAgg(self.fig6, master=self.frame1)
        self.canvas6.get_tk_widget().place(x = 1125, y=690)
        self.canvas6.draw()

        # Initialize empty lists for plotting data
        self.x_data_plot1 = []
        self.y_data_plot1 = []
        self.y1_data_plot2 = []
        self.y2_data_plot2 = []
        self.x_data_plot2 = []
        self.x_data_plot2_mins = []
        self.rt_data_plot3 = []
        self.ratio_array_plot6 = []
        self.ratio_array_numerical_plot6 = []
        self.trial_number_plot6 = []
        

    def create_widgets(self):
        # Serial port selection (Coordinates)
        self.port_label = tk.Label(self.frame2, text="Select Coordinates Serial Port:")
        self.port_label.place(x = 420, y = 0)

        self.port_var = tk.StringVar()
        self.port_combobox = ttk.Combobox(self.frame2, textvariable=self.port_var)
        self.port_combobox.place(x = 425, y=35)

        #Create another serial port selection for another teensy
        self.port_label2 = tk.Label(self.frame1, text="Select Trials Serial Port:")
        self.port_label2.place(x = 425, y = 0)

        self.port_var2 = tk.StringVar()
        self.port_combobox2 = ttk.Combobox(self.frame1, textvariable=self.port_var2)
        self.port_combobox2.place(x = 420, y=35)

        # Populate the combobox with available serial ports
        self.populate_serial_ports()
        
        # Populate the other combobox with available serial ports
        self.populate_serial_ports2()

        # Create a button to connect to the COM port
        self.connect_button = tk.Button(self.frame2, text="Connect To Coordinates", command=self.connect_to_port_coordinates)
        self.connect_button.place(x = 435, y=70)
        
        #Create another button to connect to the trials COM port
        self.connect_button2 = tk.Button(self.frame1, text="Connect to Trials", command=self.connect_to_port_trials)
        self.connect_button2.place(x = 435, y=70)

        # Create a text area for displaying the serial data
        self.text_area = scrolledtext.ScrolledText(self.frame2, wrap=tk.WORD, width=80, height=50)
        self.text_area.place(x = 160, y=110)
        
        #Create a text area for displaying the trials serial data
        self.text_area2 = scrolledtext.ScrolledText(self.frame1, wrap=tk.WORD, width=80, height=50)
        self.text_area2.place(x = 160, y=110)

        # Create a button to start capturing data (Coordinates)
        self.start_button = tk.Button(self.frame2, text="Start Capture Coordinates", state=tk.DISABLED, command=self.start_capture1)
        self.start_button.place(x = 435, y=930)
        
        #Create another button to start capturing data (Trials)
        self.start_button2 = tk.Button(self.frame1, text="Start Capture Trials", state=tk.DISABLED, command=self.start_capture2)
        self.start_button2.place(x = 435, y=930)

        # Create a button to stop capturing data and save to a file
        self.stop_button = tk.Button(self.frame3, text="Stop Capture & Save Both", state=tk.DISABLED, command=self.stop_capture)
        self.stop_button.pack(pady=10)
        
        #Create a timer widget that starts counting up when the "Start Capture Coordinates" button is pressed
        self.timer_label = tk.Label(self.frame3, text="Timer:")
        self.timer_label.pack()
        
        #Create button for generating a report
        self.report_button = tk.Button(self.frame3, text="Generate Mouse Report", state =tk.DISABLED , command=self.generate_report)
        self.report_button.pack(pady=10)
        
        #Create a ratio label that displays the says "Some Trial Info"
        self.header_label = tk.Label(self.frame1, text="Some Trial Info")
        self.header_label.place(x = 1400, y= 0)
        
        #Create a ratio label that displays the ratio of push to total trials
        self.ratio_label = tk.Label(self.frame1, text="p(push):")
        self.ratio_label.place(x = 1400, y= 20)
        
        #Create a label that says "Average Reaction Time"
        self.avg_rt_label = tk.Label(self.frame1, text="Average Reaction Time:")
        self.avg_rt_label.place(x = 1400, y= 40)

        # Serial port configuration
        self.serial_port = None
        self.serial_port2 = None
        self.is_capturing = False
        self.is_capturing2 = False
        self.capture_data_flag = False
        self.capture_data_flag2 = False
        self.data_buffer = []
        self.data_buffer2 = []

    def populate_serial_ports(self):
        # Get a list of available serial ports
        ports = [port.device for port in list_ports.comports()]

        # Populate the combobox with available serial ports
        self.port_combobox['values'] = ports

        if ports:
            # Set the default selection to the first available port
            self.port_combobox.set(ports[0])
    
    def populate_serial_ports2(self):
        # Get a list of available serial ports
        ports = [port.device for port in list_ports.comports()]

        # Populate the combobox with available serial ports
        self.port_combobox2['values'] = ports

        if ports:
            # Set the default selection to the first available port
            self.port_combobox2.set(ports[0])

    def connect_to_port_coordinates(self):
        # Connect to the selected COM port and start displaying data
        selected_port = self.port_combobox.get()
        self.serial_port = serial.Serial(selected_port, 115200, timeout=1)

        # Enable the start button and disable the connect button
        self.start_button.config(state=tk.NORMAL)
        self.connect_button.config(state=tk.DISABLED)

        # Start capturing data and live plotting in separate threads
        threading.Thread(target=self.display_data).start()
        threading.Thread(target=self.live_plotting).start()
    
    def connect_to_port_trials(self):
        # Connect to the selected COM port and start displaying data
        selected_port = self.port_combobox2.get()
        self.serial_port2 = serial.Serial(selected_port, 115200, timeout=1)

        # Enable the start button and disable the connect button
        self.start_button2.config(state=tk.NORMAL)
        self.connect_button2.config(state=tk.DISABLED)

        # Start capturing data and live plotting in separate threads
        threading.Thread(target=self.display_data).start()

    def start_capture1(self):
        # Start capturing data
        self.is_capturing = True
        self.capture_data_flag = True
        
        self.get_threshold_info()
        
        #Start the timer
        threading.Thread(target=self.timer).start()

        # Disable the start button and enable the stop button
        self.start_button.config(state=tk.DISABLED)
        self.stop_button.config(state=tk.NORMAL)

        # Start capturing data in a separate thread
        threading.Thread(target=self.capture_data).start()
    
    def start_capture2(self):
        # Start capturing data
        self.is_capturing2 = True
        self.capture_data_flag2 = True
        
        self.get_user_input()
        self.get_isi_info()
        
        # Disable the start button and enable the stop button
        self.start_button2.config(state=tk.DISABLED)

        # Start capturing data in a separate thread
        threading.Thread(target=self.capture_data2).start()

    def stop_capture(self):
        # Stop capturing data
        self.is_capturing = False
        self.capture_data_flag = False

        # Enable the start button and disable the stop button
        self.start_button.config(state=tk.NORMAL)
        self.start_button2.config(state=tk.NORMAL)
        self.stop_button.config(state=tk.DISABLED)

        # Close the serial port
        if self.serial_port or self.serial_port2:
            self.serial_port.close()
            self.serial_port2.close()

        # Save captured data to a text file
        self.save_to_file()
        
        #Enable the report button
        self.report_button.config(state=tk.NORMAL)

    def capture_data(self):
        line_count = 0  # Variable to count the number of lines read

        push_line = self.ax2.axhline(y=self.push_threshold, color='r', linestyle='--', label="Push Threshold")
        pull_line = self.ax2.axhline(y=self.pull_threshold, color='r', linestyle='--', label="Pull Threshold")

        while self.is_capturing:
            # Read data from the serial port
            data = self.serial_port.readline().decode('utf-8')

            #If data looks messed up, delete the line. If it looks good, append the line to data. The format is X, Y, Time, Stage, Lick Status (Example: 523 , 523 , 647881 , Stage 1: Rest , Lick ;  )
            if len(data.strip().split(',', 4)) != 5:
                continue
            else:
                data = data.strip().split(',', 4)[0] + "," + data.strip().split(',', 4)[1] + "," + data.strip().split(',', 4)[2] + "," + data.strip().split(',', 4)[3] + "," + data.strip().split(',', 4)[4] 

            # Increment the line count
            line_count += 1

            # Skip the first 10 lines
            if line_count <= 10:
                continue

            # Add timestamp to the data
            timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
            data = data.rstrip(';')  # Remove semicolon from the end of data2
            data_with_timestamp = f"[{timestamp}] {data}\n"

            # Display data in the text area if capture_data_flag is set, make sure the each new data is on a new line (so that the data is not all on one line, each line ends with a semicolon)
            if self.capture_data_flag:
                self.text_area.after(0, self.update_text_area, data_with_timestamp)

                # Save data to the buffer
                self.data_buffer.append(data_with_timestamp)

                # Extract X and Y values for plotting
                x, y = map(int, data.strip().split(',', 2)[:2])
                self.x_data_plot1.append(x)
                self.y_data_plot1.append(y)

                # Extract Y1, Y2, and X values for plotting
                y1, y2, x = map(int, data.strip().split(',', 3)[:3])
                self.y1_data_plot2.append(y1)
                self.y2_data_plot2.append(y2)
                self.x_data_plot2.append(x)
                
                #Make another array that has x_data_plot2 but converted to minutes from milliseconds
                self.x_data_plot2_mins = [i/60000 for i in self.x_data_plot2]

                # Update the live plots
                self.ax1.clear()
                self.ax1.plot(self.y_data_plot1, self.x_data_plot1, alpha = 0.5)
                self.ax1.set_xlabel('Y Coordinates')
                self.ax1.set_ylabel('X Coordinates')
                self.ax1.set_title('X vs Y Coordinates')


                self.ax2.clear()
                self.ax2.plot(self.x_data_plot2_mins, self.y1_data_plot2, label = "X Coordinates", alpha = 0.5)
                self.ax2.plot(self.x_data_plot2_mins, self.y2_data_plot2, label = "Y Coordinates", alpha = 0.5)
                self.ax2.set_xlabel('Time (mins)')
                self.ax2.set_ylabel('Coordinates')
                self.ax2.set_title('Time (ms) vs Coordinates')
                self.ax2.legend()
                # Update the lines' data without clearing the axis
                push_line.set_ydata([self.push_threshold] * len(self.x_data_plot2))
                pull_line.set_ydata([self.pull_threshold] * len(self.x_data_plot2))

                self.canvas1.draw()
                self.canvas2.draw()
    
    def capture_data2(self):
        while self.is_capturing2:
            # Read data from the serial port
            data2 = self.serial_port2.readline().decode('utf-8')

            #If data looks messed up, delete the line. If it looks good, append the line to data2. The format is trial number, reaction time, array, current push/pull ratio, total ratio array, solenoid open time, push/pull, ISI (Example: 1 , 8502 , [1 0 0 0 0 0 ] ,  1 / 0 , 1 / 0 , 44.00 , Push , 0 ; )
            if len(data2.strip().split(',', 7)) != 8:
                continue
            else:
                data2 = data2.strip().split(',', 7)[0] + "," + data2.strip().split(',', 7)[1] + "," + data2.strip().split(',', 7)[2] + "," + data2.strip().split(',', 7)[3] + "," + data2.strip().split(',', 7)[4] + "," + data2.strip().split(',', 7)[5] + "," + data2.strip().split(',', 7)[6] + "," + data2.strip().split(',', 7)[7] 

            # Add timestamp to the data only if the data is not empty, all data should be stored in separate lines so that the data is not all on one line
            if data2:
                timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
                data2 = data2.rstrip(';')  # Remove semicolon from the end of data2
                data2_with_timestamp = f"[{timestamp}] {data2}\n"
            else:
                # Check again for data if the data is empty
                continue

            # Display data in the text area if capture_data_flag is set, make sure the each new data is on a new line (so that the data is not all on one line, each line ends with a semicolon)
            if self.capture_data_flag2:
                self.text_area2.after(0, self.update_text_area2, data2_with_timestamp)

                # Save data to the buffer
                self.data_buffer2.append(data2_with_timestamp)
                
                #Extract reaction time for plotting (can be found in the second column of the data)
                rt = int(data2.strip().split(',', 2)[1])
                self.rt_data_plot3.append(rt)
                
                #Find the average reaction time
                avg_rt = sum(self.rt_data_plot3)/len(self.rt_data_plot3)
                #Update the average reaction time label (rounded to 3 decimal places)
                self.avg_rt_label.config(text=f"Average Reaction Time: {round(avg_rt, 3)}")
                
                #Extract the number of pushes done (5th column of data, first number - format is number / number for that column)
                push = int(data2.strip().split(',', 5)[4].split('/', 1)[0])
                #Extract the number of trials done (1st column of data)
                trial = int(data2.strip().split(',', 2)[0])
                #Calculate the ratio of push to total trials
                ratio = push/trial
                #Update the ratio label (rounded to 3 decimal places)
                self.ratio_label.config(text=f"p(push): {round(ratio, 3)}")
                
                #Extract the current ratio of pushes to pulls in the array (4rd column of data) - this will be a string (don't want to convert it)
                ratio_array = data2.strip().split(',', 4)[3]

                #Append the ratio array to the ratio array plot
                self.ratio_array_plot6.append(ratio_array)
                
                #Map the ratio array to a numerical values (0 to 6)
                if ratio_array == "0/6":
                    self.ratio_array_numerical_plot6.append(0)
                elif ratio_array == "1/5":
                    self.ratio_array_numerical_plot6.append(1)
                elif ratio_array == "2/4":
                    self.ratio_array_numerical_plot6.append(2)
                elif ratio_array == "3/3":
                    self.ratio_array_numerical_plot6.append(3)
                elif ratio_array == "4/2":
                    self.ratio_array_numerical_plot6.append(4)
                elif ratio_array == "5/1":
                    self.ratio_array_numerical_plot6.append(5)
                elif ratio_array == "6/0":
                    self.ratio_array_numerical_plot6.append(6)
                else:
                    self.ratio_array_numerical_plot6.append(None)

                #Extract the current trial number (1st column of data)
                trial_number = int(data2.strip().split(',', 2)[0])
                self.trial_number_plot6.append(trial_number)
                
                #Update the live plot
                self.ax3.clear()
                for i in range(len(self.rt_data_plot3)):
                    self.ax3.axvline(x = self.rt_data_plot3[i], color = 'r')
                self.ax3.set_xlabel('Reaction Time (ms)')
                self.ax3.set_title('Reaction Times')
                self.ax3.set_xlim(0, 10000)
                self.ax3.set_xticks([i for i in range(0, max(self.ax3.get_xticks().astype(int)) + 1, 1000)])
                self.ax3.axes.yaxis.set_ticks([])
                
                #Another live plot for reaction times that are histogrammed (bin = 10)
                self.ax4.clear()
                self.ax4.hist(self.rt_data_plot3, bins = [0, 1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000], edgecolor = 'black', histtype='bar')
                self.ax4.set_xlabel('Reaction Time (ms)')
                self.ax4.set_ylabel('Frequency')
                self.ax4.set_title('Reaction Times')
                self.ax4.set_yticks([i for i in range(0, max(self.ax4.get_yticks().astype(int)) + 1, 5)])
                self.ax4.set_xlim(0, 10000)
                self.ax4.set_xticks([i for i in range(0, max(self.ax4.get_xticks().astype(int)) + 1, 1000)])
                
                self.ax6.clear()
                #Only plot from line 6 and onwards a scatterplot of the ratio array and the trial number (trial number starts at 1)
                if len(self.ratio_array_numerical_plot6) >= 6:
                    self.ax6.scatter(self.trial_number_plot6[5:], self.ratio_array_numerical_plot6[5:])
                self.ax6.set_xlabel('Trial Number')
                self.ax6.set_ylabel('Ratio of Pushes to Pulls')
                self.ax6.set_title('Push to Pull Ratio')
                self.ax6.set_yticks([0, 1, 2, 3, 4, 5, 6])
                self.ax6.set_yticklabels(['0 / 6', '1 / 5', '2 / 4', '3 / 3', '4 / 2', '5 / 1', '6 / 0'])
                
                self.fig3.tight_layout()
                self.fig4.tight_layout()
                self.fig6.tight_layout()
                
                self.canvas3.draw()
                self.canvas4.draw()
                self.canvas6.draw()
                
                # Update the GUI
                self.root.update()
    def update_text_area(self, data):
        # Display data in the text area
        self.text_area.insert(tk.END, data)

        # Auto-scroll to the bottom
        self.text_area.yview(tk.END)

        # Update the GUI
        self.root.update()
    
    def update_text_area2(self, data2):
        # Display data in the text area
        self.text_area2.insert(tk.END, data2)

        # Auto-scroll to the bottom
        self.text_area2.yview(tk.END)

        # Update the GUI
        self.root.update()

    def live_plotting(self):
        while not self.is_capturing:
            # Display data from the serial port before starting the capture
            data = self.serial_port.readline().decode('utf-8')

            # Add timestamp to the data
            timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
            data_with_timestamp = f"[{timestamp}] {data}"

            # Schedule an update of the text area in the main thread
            self.text_area.after(0, self.update_text_area, data_with_timestamp)

            # Update the GUI
            self.root.update()

    def save_to_file(self):
        #Open file explorer to select the folder to save the files in
        path = filedialog.askdirectory()
        
        # Set the filename based on the naming scheme
        filename = f"{self.mouse_name}_{self.date}_Coordinates.txt"
        filename2 = f"{self.mouse_name}_{self.date}_Trials.txt"
        filename3 = f"{self.mouse_name}_{self.date}_Info.txt"
        
        # Construct the full paths
        filepath = os.path.join(path, filename)
        filepath2 = os.path.join(path, filename2)
        filepath3 = os.path.join(path, filename3)
        
        # Open the file for writing
        with open(filepath, 'w') as file:
            for data in self.data_buffer:
                file.write(data.split('] ', 1)[-1])
        
        with open(filepath2, 'w') as file:
            for data2 in self.data_buffer2:
                file.write(data2.split('] ', 1)[-1])
        
        with open(filepath3, 'w') as file:
            file.write(f"Mouse Name: {self.mouse_name}\nDate: {self.date}\nTraining Day: {self.training_day}\nRig Name: {self.rig_name}\nParadigm: {self.paradigm}\nPush Threshold: {self.push_threshold}\nPull Threshold: {self.pull_threshold}\nRest Lowerbound: {self.rest_lowerbound}\nRest Upperbound: {self.rest_upperbound}\nISI Upperbound: {self.isi_upperbound}\nISI Lowerbound: {self.isi_lowerbound}\np(push): {round(sum(self.rt_data_plot3)/len(self.rt_data_plot3), 3)}\nAverage Reaction Time: {round(sum(self.rt_data_plot3)/len(self.rt_data_plot3), 3)}")
        
        #Save the plots as pdfs in the same folder
        self.fig1.savefig(os.path.join(path, f"{self.mouse_name}_{self.date}_XYCoordinates.pdf"))
        self.fig2.savefig(os.path.join(path, f"{self.mouse_name}_{self.date}_CoordinatesVsTime.pdf"))
        self.fig3.savefig(os.path.join(path, f"{self.mouse_name}_{self.date}_ReactionTimesLines.pdf"))
        self.fig4.savefig(os.path.join(path, f"{self.mouse_name}_{self.date}_ReactionTimesHistogram.pdf"))

    def get_user_input(self):
        # Prompt user for mouse information and monitor type
        fields = ["Mouse Name", "Date", "Training Day", "Rig Name", "Paradigm"]
        monitor_types = ["Type 1", "Type 2", "Type 3"]  # Replace with the actual monitor types
        dialog = EntryDialog(self.root, title="User Information", fields=fields, monitor_types=monitor_types)
        user_input = dialog.result

        if not user_input:
            # If the user cancels the dialog, close the application
            self.root.destroy()

        self.mouse_name, self.date, self.training_day, self.rig_name ,self.paradigm = user_input
        
        #Update the gui to have mouse name and date on the top left
        self.root.title(f"{self.mouse_name} {self.date}")

    def get_threshold_info(self):
        # Prompt user for threshold information
        fields = ["Push Threshold", "Pull Threshold", "Rest Lowerbound", "Rest Upperbound"]
        monitor_types = ["Type 1", "Type 2", "Type 3"]  # Replace with the actual monitor types
        dialog = EntryDialog2(self.root, title="Threshold Information", fields=fields, monitor_types=monitor_types)
        user_input = dialog.result

        if not user_input:
            # If the user cancels the dialog, close the application
            self.root.destroy()
        self.push_threshold, self.pull_threshold, self.rest_lowerbound, self.rest_upperbound = user_input
        
        #Write the threshold information to the serial port
        self.serial_port.write(f"{self.push_threshold},{self.pull_threshold},{self.rest_lowerbound},{self.rest_upperbound}".encode('utf-8'))
    
    def get_isi_info(self):
        # Prompt user for isi information
        fields = ["ISI Lowerbound", "ISI Upperbound"]
        monitor_types = ["Type 1", "Type 2", "Type 3"]
        dialog = EntryDialog3(self.root, title="ISI Information", fields=fields, monitor_types=monitor_types)
        user_input = dialog.result
        
        if not user_input:
            # If the user cancels the dialog, close the application
            self.root.destroy()
        self.isi_lowerbound, self.isi_upperbound = user_input
        
        #Write the isi information to the serial port
        self.serial_port2.write(f"{self.isi_lowerbound},{self.isi_upperbound}".encode('utf-8'))
        
    def timer(self):
        #Create a timer in hours, mins, and seconds that starts when the "Start Capture Coordinates" button is pressed
        hours = 0
        mins = 0
        seconds = 0
        while self.is_capturing:
            if seconds < 59:
                seconds += 1
            elif seconds == 59:
                seconds = 0
                mins += 1
            elif mins == 59:
                mins = 0
                hours += 1
            self.timer_label.config(text=f"Timer: {hours}:{mins}:{seconds}")
            self.root.update()
            time.sleep(1)
        self.timer_label.config(text=f"Timer: {hours}:{mins}:{seconds}")
    
    def generate_report(self):
        #Create a new figure for the report
        fig7 = plt.subplots(layout = "constrained", figsize = (8.5, 11))
        gs = GridSpec(4, 2, figure = fig7)
        
        #Add a title to the report
        fig7.suptitle("Daily Mouse Report", fontsize = 16)
        
        #Information about the mouse
        ax7 = fig7.add_subplot(gs[0, 0])
        ax7.axis('off')
        ax7.text(0.2, 1, 'Trial Information', fontsize = 12)
        ax7.text(0, 0.8, f"Mouse ID: {self.mouse_name}", fontsize = 10)
        ax7.text(0, 0.7, f"Date: {self.date}", fontsize = 10)
        ax7.text(0, 0.6, f"Paradigm: {self.paradigm}", fontsize = 10)
        ax7.text(0, 0.5, f"ISI Upperbound: {self.isi_upperbound}", fontsize = 10)
        ax7.text(0, 0.4, f"ISI Lowerbound: {self.isi_lowerbound}", fontsize = 10)
        ax7.text(0, 0.3, f"Push Threshold: {self.push_threshold}", fontsize = 10)
        ax7.text(0, 0.2, f"Pull Threshold: {self.pull_threshold}", fontsize = 10)
        ax7.text(0, 0.1, f"Rest Lowerbound: {self.rest_lowerbound}", fontsize = 10)
        ax7.text(0, 0, f"Rest Upperbound: {self.rest_upperbound}", fontsize = 10)
    
        


if __name__ == "__main__":
    root = tk.Tk()
    root.state('zoomed')
    app = SerialMonitorGUI(root)
    root.mainloop()
