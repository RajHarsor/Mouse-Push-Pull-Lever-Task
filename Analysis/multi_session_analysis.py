#%% Import the necessary packages and file

#Import pandas
import pandas as pd

#Give the path of the file containing the overall information
pathOverall= input("Enter the path of the file containing the overall information: ")

#Take out the quotes from the path and don't replace them with anything
pathOverall=pathOverall.replace('"','')

#Read the excel file
xls = pd.ExcelFile(pathOverall)
df = pd.read_excel(xls, 'Cohort 3')

print(df)

#Remove the rows that have NaN in the Days on Rig column
df = df.dropna(subset=['Days on Rig'])
#%% Number of Rewards per Day
#Import matplotlib
import matplotlib.pyplot as plt

#Make a line plot with Days on the Rig as the x-axis, Total Trials as the y-axis, and Animal as each different line
plt.plot(df['Days on Rig'], df['Total Trials'], marker='o', color='blue', linewidth=2, label='Total Trials')



#Show the plot
plt.show()
# %%
