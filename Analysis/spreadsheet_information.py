#Import pandas
import pandas as pd

#Import path for file
pathTrial = input("Enter the path of the file containing the trial information: ")

#Take out the quotes from the path and don't replace them with anything
pathTrial=pathTrial.replace('"','')

#Read the file
dfTrial = pd.read_csv(pathTrial, sep=',', header=None, names=['Trial Number', 'Reaction Time', 'Current Array', 'Current Array Push/Pull Ratio', 'Total Push/Pull Ratio', 'Solenoid Open Time', 'Push/Pull', 'ISI Delay'], on_bad_lines="warn")

#Drop the rows that have NaN values
dfTrial = dfTrial.dropna()

#Reset the index
dfTrial = dfTrial.reset_index(drop=True)

#Change the Reaction Time, Trial Number, and ISI Delay column to a float
dfTrial['Reaction Time'] = dfTrial['Reaction Time'].astype(float)
#dfTrial['Trial Number'] = dfTrial['Trial Number'].astype(float)
dfTrial['ISI Delay'] = dfTrial['ISI Delay'].astype(float)


"""## Information to put on the Spreadsheet"""

#Count the number of "Push" in the Push/Pull column
PushCount = dfTrial['Push/Pull'].str.count('Push').sum()

#Count the number of "Pull" in the Push/Pull column
PullCount = dfTrial['Push/Pull'].str.count('Pull').sum()

#Add PushCount and PullCount together
TotalPushPullCount = PushCount + PullCount

#Calculate average reaction time
AverageReactionTime = dfTrial['Reaction Time'].mean()

#Calculate the average ISI Delay
AverageISIDelay= dfTrial['ISI Delay'].mean()

# Put information above into a dataframe
dfTrialInfo = pd.DataFrame({'Total Push Count': [PushCount], 'Total Pull Count': [PullCount], 'Total Push/Pull Count': [TotalPushPullCount], 'Average Reaction Time (ms)': [AverageReactionTime], 'Average ISI Delay': [AverageISIDelay]})

#Make sure we can see all the dataframes in the table below
pd.set_option('display.max_columns', None)
pd.set_option('display.max_rows', None)

print(dfTrialInfo)
