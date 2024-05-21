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

#TODO Add a way to read the excel file without having to specify the sheet name

#Remove columns that are not needed (all except for Date, Animal, Days on Rig, Forward, Reverse, Total Trials,  and Average Rxn Time (ms))
df=df.drop(['Start Weight (g)', 'Current Weight (g)', 'Percent', 'Water to 85%', '25mg/kg', '50mg/kg', '10mg/kg', 'Recommend (ml)', 'Give (ml)', 'Earned (ml)', 'Program', 'Time Started', 'Rest Upper Value (X Coordinate)', 'Rest Lower Value (X Coordinate)', 'Push Value (X Coordinate)', 'Pull Value (X Coordinate)', 'ISI Upper Range (ms)', 'ISI Upper Range (ms)', 'ISI Lower Range (ms)'], axis=1)

#Delete any rows that have NaN values in the Days on Rig column
df=df.dropna(subset=['Days on Rig'])

#Print the first 5 rows of the dataframe
print(df.head())
#%% Number of Rewards per Day
#Import plotly go
import plotly.express as px

#Calculate mean number of rewards per day
meanRewards=df.groupby(['Days on Rig']).mean()

#Plot a line graph Days on Rig on the X axis, and Total Trials on the Y axis and color by Animal
fig = px.line(df, x="Days on Rig", y="Total Trials", color="Animal", title='Total Trials per Day')

#Plot a line graph of mean number of rewards per day
fig.add_scatter(x=meanRewards.index, y=meanRewards['Total Trials'], mode='lines', name='Mean')

#Adjust the X scale to go up by increments of 1
fig.update_xaxes(dtick=1)

#Show the graph
fig.show()
#%% Mean Number of Rewards per Day + Error Bars

#Import plotly go
import plotly.graph_objects as go

#Plot mean numbers of rewards per day with error bars
fig2 = go.Figure(data=go.Scatter(x=meanRewards.index, y=meanRewards['Total Trials'], error_y=dict(type='data', array=df.groupby(['Days on Rig']).std()['Total Trials'], visible=True), mode='lines', name='Mean'))

#Add a title to the graph
fig2.update_layout(title='Mean Total Trials per Day')

#Add a label to the X axis
fig2.update_xaxes(title_text='Days on Rig')

#Add a label to the Y axis
fig2.update_yaxes(title_text='Total Trials')

#Adjust the X scale to go up by increments of 1
fig2.update_xaxes(dtick=1)

#Show the graph
fig2.show()
#%% Number of Rewards per Day Statistics

#Calculate the mean number of rewards per day
meanRewards=df.groupby(['Days on Rig']).mean()

#Calculate the standard deviation of the number of rewards per day
stdRewards=df.groupby(['Days on Rig']).std()

#Calculate the standard error of the number of rewards per day
semRewards=df.groupby(['Days on Rig']).sem()

#Calculate the median number of rewards per day
medianRewards=df.groupby(['Days on Rig']).median()

#Put all of the statistics into a dataframe
statsRewards=pd.DataFrame({'Mean':meanRewards['Total Trials'], 'Standard Deviation':stdRewards['Total Trials'], 'Standard Error':semRewards['Total Trials'], 'Median':medianRewards['Total Trials']})

#Print the statistics
print(statsRewards)
#%% P(push) per Day

#Import plotly go
import plotly.express as px

#Calculate mean P(push) per day
df['P(push)']=df['Forward']/df['Total Trials']

#Make a line graph where P(push) is the y axis, Days on Rig is the x axis, and color by Animal
fig3 = px.line(df, x="Days on Rig", y="P(push)", color="Animal", title='P(push) per Day')

#Calculate mean P(push) per day
meanPpush=df.groupby(['Days on Rig']).mean()

#Plot a line graph of mean P(push) per day
fig3.add_scatter(x=meanPpush.index, y=meanPpush['P(push)'], mode='lines', name='Mean')

#Add a red horizontal line at 0.5
fig3.add_shape(type="line", x0=0, y0=0.5, x1=df['Days on Rig'].max(), y1=0.5, line=dict(color="Red",width=1))

#Adjust the X scale to go up by increments of 1
fig3.update_xaxes(dtick=1)

#Adjust the Y scale to go up by increments of 0.1
fig3.update_yaxes(dtick=0.1)

#Show the graph
fig3.show()
#%% P(push) per Day + Error Bars

#Import plotly go
import plotly.graph_objects as go

#Plot mean P(push) per day with error bars
fig4 = go.Figure(data=go.Scatter(x=meanPpush.index, y=meanPpush['P(push)'], error_y=dict(type='data', array=df.groupby(['Days on Rig']).std()['P(push)'], visible=True), mode='lines', name='Mean'))

#Add a red horizontal line at 0.5
fig4.add_shape(type="line", x0=0, y0=0.5, x1=df['Days on Rig'].max(), y1=0.5, line=dict(color="Red",width=1))

#Add a title to the graph
fig4.update_layout(title='Mean P(push) per Day')

#Add a label to the X axis
fig4.update_xaxes(title_text='Days on Rig')

#Add a label to the Y axis
fig4.update_yaxes(title_text='P(push)')

#Adjust the X scale to go up by increments of 1
fig4.update_xaxes(dtick=1)

#Adjust the Y scale to go up by increments of 0.1
fig4.update_yaxes(dtick=0.1)

#Show the graph
fig4.show()

#%% P(push) per Day Statistics

#Calculate the mean P(push) per day
meanPpush=df.groupby(['Days on Rig']).mean()

#Calculate the standard deviation of the P(push) per day
stdPpush=df.groupby(['Days on Rig']).std()

#Calculate the standard error of the P(push) per day
semPpush=df.groupby(['Days on Rig']).sem()

#Calculate the median P(push) per day
medianPpush=df.groupby(['Days on Rig']).median()

#Put all of the statistics into a dataframe
statsPpush=pd.DataFrame({'Mean':meanPpush['P(push)'], 'Standard Deviation':stdPpush['P(push)'], 'Standard Error':semPpush['P(push)'], 'Median':medianPpush['P(push)']})

#Print the statistics
print(statsPpush)
#%% Average Reaction Time per Day

#Import plotly go
import plotly.express as px

#Convert reaction time to seconds
df['Avg Rxn Time (sec)']=df['Avg Rxn Time (ms)']/1000

#Make a line graph where Avg Rxn Time (sec) is the y axis, Days on Rig is the x axis, and color by Animal   
fig5 = px.line(df, x="Days on Rig", y="Avg Rxn Time (sec)", color="Animal", title='Average Reaction Time per Day')

#Calculate mean Avg Rxn Time (sec) per day
meanRxnTime=df.groupby(['Days on Rig']).mean()

#Plot a line graph of mean Avg Rxn Time (sec) per day
fig5.add_scatter(x=meanRxnTime.index, y=meanRxnTime['Avg Rxn Time (sec)'], mode='lines', name='Mean')

#Add a title to the graph
fig5.update_layout(title='Average Reaction Time per Day')

#Add a label to the X axis
fig5.update_xaxes(title_text='Days on Rig')

#Add a label to the Y axis
fig5.update_yaxes(title_text='Avg Rxn Time (sec)')

#Adjust the X scale to go up by increments of 1
fig5.update_xaxes(dtick=1)

#Show the graph
fig5.show()
#%% Total Trials as a function of Time on Rig

#Import plotly go
import plotly.express as px

#Make a new column that divides the total trials by Time on Rig
df['Total Trials/Time on Rig (min)']=df['Total Trials']/df['Time on Rig (min)']

#Make a line graph where Total Trials/Time on Rig is the y axis, Days on Rig is the x axis, and color by Animal
fig6 = px.line(df, x="Days on Rig", y="Total Trials/Time on Rig (min)", color="Animal", title='Total Trials as a function of Time on Rig')

#Calculate mean Total Trials/Time on Rig per day
meanTrialsTime=df.groupby(['Days on Rig']).mean()

#Plot a line graph of mean Total Trials/Time on Rig per day
fig6.add_scatter(x=meanTrialsTime.index, y=meanTrialsTime['Total Trials/Time on Rig (min)'], mode='lines', name='Mean')

#Show the figure
fig6.show()

# %%
