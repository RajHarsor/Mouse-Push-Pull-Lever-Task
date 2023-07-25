#%% Initialize the program
%matplotlib widget
#%% Import the necessary packages and files
#Import pandas
import pandas as pd

#Paths for the files we are going to use
pathCoords= input("Enter the path of the file containing the coordinates information: ")
pathTrial= input("Enter the path of the file containing the trial information: ")

#pathCoords = r'C:\Users\rdh92-adm\Downloads\CoordinatesExample.txt'
#pathTrial = r"C:\Users\rdh92-adm\Box\Oldenburg-Shared\Cohort 3 Behavioral Testing\F3.4-39\6.13.23\F3.4-39_6.13.23_Trials"

#Take out the quotes from the path and don't replace them with anything
pathCoords=pathCoords.replace('"','')
pathTrial=pathTrial.replace('"','')

#%% Input the values of the upper and lower X bounds and what is considered a push and pull

upperXBound= int(input("Enter the upper X bound: "))
LowerXBound= int(input("Enter the lower X bound: "))
Push= int(input("Enter the value of a push: "))
Pull= int(input("Enter the value of a pull: "))

#%% Read the files
#Read the files
dfCoords = pd.read_csv(pathCoords, sep=',', header=None, on_bad_lines="warn")
dfTrial = pd.read_csv(pathTrial, sep=',', header=None, names=['Trial Number', 'Reaction Time', 'Current Array', 'Current Array Push/Pull Ratio', 'Total Push/Pull Ratio', 'Solenoid Open Time', 'Push/Pull', 'ISI Delay'], on_bad_lines="warn")

#%% Clean up the Coordinates Code
#Edit index 0, row 0 to delete the values before the t including the t
#dfCoords.at[0, 0] = dfCoords.at[0, 0].split('\t')[1]

# Rename the columns
dfCoords.columns = ['X Coordinate', 'Y Coordinate', 'Time (us)', 'Phase']

#Get rid of the even numbered rows
#dfCoords = dfCoords.iloc[::2, :]

#Reset the index
dfCoords = dfCoords.reset_index(drop=True)

#Convert the time to minutes
dfCoords['Time (min)'] = dfCoords['Time (us)']/60000000

#Reorder the columns
dfCoords = dfCoords[['X Coordinate', 'Y Coordinate', 'Phase', 'Time (us)', 'Time (min)']]

#Filter rows where the specific column does not contain a numerical value
dfCoords = dfCoords[pd.to_numeric(dfCoords['X Coordinate'], errors='coerce').notnull()]
dfCoords = dfCoords[pd.to_numeric(dfCoords['Y Coordinate'], errors='coerce').notnull()]
dfCoords = dfCoords[pd.to_numeric(dfCoords['Time (min)'], errors='coerce').notnull()]

#Make thew columns X and Y coordinates and Time(us) and Time(min) into floats
dfCoords['X Coordinate'] = dfCoords['X Coordinate'].astype(int)
dfCoords['Y Coordinate'] = dfCoords['Y Coordinate'].astype(int)
dfCoords['Time (us)'] = dfCoords['Time (us)'].astype(float)
dfCoords['Time (min)'] = dfCoords['Time (min)'].astype(float)

#%% Line Graph - Coordinates over Time
#Import plotly go
import plotly.graph_objects as go

#Create a line graph with Time (min) as the x axis and X Coordinate as the y axis on top of each other
fig1 = go.Figure()
fig1.add_trace(go.Scatter(x=dfCoords['Time (min)'], y=dfCoords['X Coordinate'], name='X Coordinate'))
fig1.add_trace(go.Scatter(x=dfCoords['Time (min)'], y=dfCoords['Y Coordinate'], name='Y Coordinate'))

#Add a title and axis labels
fig1.update_layout(title='Coordinates vs Time', xaxis_title='Time (min)', yaxis_title='Coordinates')

#Show the graph
fig1.show()

#%% Matplotlib Graph (for exporting purposes)
#Make matplotlib graph with Time (min) on the x axis and X and Y coordinates on the Y axis
import matplotlib.pyplot as plt

#Create a figure
fig2, ax = plt.subplots()

#Plot the X and Y coordinates on the same graph
ax.plot(dfCoords['Time (min)'], dfCoords['X Coordinate'], label='X Coordinate')
ax.plot(dfCoords['Time (min)'], dfCoords['Y Coordinate'], label='Y Coordinate')

#Add a title and axis labels
ax.set_title('Coordinates vs Time')
ax.set_xlabel('Time (min)')
ax.set_ylabel('Coordinates')

#Despine the graph
ax.spines['right'].set_visible(False)
ax.spines['top'].set_visible(False)

#Zoom in to a certain area if you want
#ax.set_xlim(23.5, 24.5)

#Add a legend
ax.legend()

#Show the graph
plt.show()

#%% Line Graph - Coordinates over Time Smoothed
# Apply some smoothing to the X and Y coordinates using the Savitzky-Golay filter
from scipy.signal import savgol_filter

#Create a new column for the smoothed X Coordinate
dfCoords['X Coordinate Smoothed'] = savgol_filter(dfCoords['X Coordinate'], 51, 3)

#Create a new column for the smoothed Y Coordinate
dfCoords['Y Coordinate Smoothed'] = savgol_filter(dfCoords['Y Coordinate'], 51, 3)

#Create a line graph with Time (min) as the x axis and X Coordinate Smoothed as the y axis on top of each other
fig3 = go.Figure()
fig3.add_trace(go.Scatter(x=dfCoords['Time (min)'], y=dfCoords['X Coordinate Smoothed'], name='X Coordinate Smoothed'))
fig3.add_trace(go.Scatter(x=dfCoords['Time (min)'], y=dfCoords['Y Coordinate Smoothed'], name='Y Coordinate Smoothed'))

#Add a title and axis labels
fig3.update_layout(title='Smoothed Coordinates vs Time', xaxis_title='Time (min)', yaxis_title='Coordinates')

#Show the graph
fig3.show()

#%% Matplotlib Graph (for exporting purposes)
#Make matplotlib graph with Time (min) on the x axis and X and Y coordinates smoothed on the Y axis
import matplotlib.pyplot as plt

#Create a figure
fig4, ax = plt.subplots()

#Plot the X and Y coordinates smoothed on the same graph
ax.plot(dfCoords['Time (min)'], dfCoords['X Coordinate Smoothed'], label='X Coordinate Smoothed')
ax.plot(dfCoords['Time (min)'], dfCoords['Y Coordinate Smoothed'], label='Y Coordinate Smoothed')

#Add a title and axis labels
ax.set_title('Smoothed Coordinates vs Time')
ax.set_xlabel('Time (min)')
ax.set_ylabel('Coordinates')

#Despine the graph
ax.spines['right'].set_visible(False)
ax.spines['top'].set_visible(False)

#Zoom in to a certain area if you want
#ax.set_xlim(23.5, 24.5)

#Add a legend
ax.legend()

#Show the graph
plt.show()

#%% 3D Plot of Coordinates over Time
# FIXME This 3D plot is not working for some reason, test again later
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt

#Create Figure
fig = plt.figure()
ax = Axes3D(fig)

#Create Plot
dfig = ax.scatter(dfCoords['X Coordinate'], dfCoords['Y Coordinate'], dfCoords['Time (min)'], color = 'green', cmap='viridis', linewidth=0.5)

plt.show()

#%% X Coordinates with Rewards Over Time
#Import matplotlib
import matplotlib.pyplot as plt

#Find the values where it goes from Stage 2 to Stage 3
x_values = []
Time = []

previous_stage = None

for index, row in dfCoords.iterrows():
    current_stage = row['Phase']

    if previous_stage == ' Stage 2: Push/Pull ' and current_stage == ' Stage 3: Water Retrival ':
        x_values.append(row['X Coordinate'])
        Time.append(row['Time (min)'])

    previous_stage = current_stage

#Drop columns that are duplicates in the Time (min) column
dfCoords = dfCoords.drop_duplicates(subset='Time (min)', keep='first')


#Plot X Coordinate vs Time (min)
plt.plot(dfCoords['Time (min)'], dfCoords['X Coordinate'], color='black')

#Draw dotted red lines at the pull and push coordinate values
plt.axhline(y=Pull, color='red', linestyle='--', alpha = 0.5)
plt.axhline(y=Push, color='red', linestyle='--', alpha = 0.5)

#Make the image longer
plt.gcf().set_size_inches(20, 5)

#Set labels
plt.xlabel('Time (min)')
plt.ylabel('X Coordinate')

#Remove top and right spines
plt.gca().spines['top'].set_visible(False)
plt.gca().spines['right'].set_visible(False)


#Make a list with just the int 550 in it that is the same length as the length of Time
y = [dfCoords['X Coordinate'].max() + 5] * len(Time)

#Make a scatter plot of the X and Y coordinates where it goes from Stage 2 to Stage 3
plt.scatter(Time, y, color='blue', marker='|', alpha = 0.5)

#Add Title
plt.title('X Coordinate vs Time (min)')

#Zoom into min 10 - 20 on the graph
#plt.xlim(40, 50)
#plt.ylim(490, 550)

#Show the plot
plt.show()

#%% Pie Graph of Time Spent in Each Phase
#Import plotly go
import plotly.graph_objects as go

#Calculate the difference in time in us between each subsequent row in the dataframe
dfCoords['Time (us) Difference'] = dfCoords['Time (us)'].diff()

#Make new dataframe with information
phase_times = dfCoords.groupby("Phase")["Time (us) Difference"].sum().reset_index()

#Make the Pie Graph
fig5 = go.Figure(data=[go.Pie(labels=phase_times['Phase'], values=phase_times['Time (us) Difference'])])
fig5.update_layout(title='Time Spent in Each Phase')

# Show the plot
fig5.show()