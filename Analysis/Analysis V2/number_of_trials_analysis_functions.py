import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import scipy.stats as stats
import numpy as np

'''
The first thing that should be done when using these functions is to import them into your script.
Following, you will need to read the water log data into a dataframe using the fuction read_waterlog_data.
'''

def read_waterlog_data(path, sheet_name):
    '''
    args:
    path: path to the excel file containing the water log data
    sheet_name: name of the sheet within the excel file that contains the water log data
    '''
    import pandas as pd
    df = pd.read_excel(path, sheet_name=sheet_name)
    if 'Reverse' in df.columns:
        df = df[['Date', 'Animal', 'Program', 'Days on Rig', 'Time on Rig (min)', 'Forward', 'Reverse', 'Total Trials', 'Avg Rxn Time (ms)']]
    elif 'Down' in df.columns:
        df = df[['Date', 'Animal', 'Program', 'Days on Rig', 'Time on Rig (min)', 'Forward', 'Down', 'Total Trials', 'Avg Rxn Time (ms)']]
    elif 'Pull' in df.columns:
        df = df[['Date', 'Animal', 'Program', 'Days on Rig', 'Time on Rig (min)', 'Push', 'Pull', 'Total Trials', 'Avg Rxn Time (ms)']]
    else:
        pass
    if 'Forward' in df.columns:
        df = df.rename(columns={'Forward' : 'Movement A'})
    if 'Push' in df.columns:
        df = df.rename(columns={'Push' : 'Movement A'})
    if 'Reverse' in df.columns:
        df = df.rename(columns={'Reverse' : 'Movement B'})
    if 'Down' in df.columns:
        df = df.rename(columns={'Down' : 'Movement B'})
    if 'Pull' in df.columns:
        df = df.rename(columns={'Pull' : 'Movement B'})
    #df['Days on Rig'] = df['Days on Rig'].astype(int)
    df = df.dropna(subset = ['Days on Rig'])
    if df['Days on Rig'].max() > 9:
        df = df[df['Days on Rig'] < 10]
    else:
        pass
    #Calculate movement A divided by tiotal trials
    df['Movement A %'] = df['Movement A'] / df['Total Trials']
    #Calculate Z score for movement A %
    df['Movement A % Z Score'] = stats.zscore(np.array(df['Movement A %'], dtype = np.float64), nan_policy='omit')
    return df

'''
After reading in the data, any of the following functions can be used to analyze the data.
'''

#This function will plot the total trials for each animal over days on rig with a regression line
def individual_mouse_regression_total_trials(data):
    '''
    args:
    data: dataframe containing water log data (Cohort data)
    '''
    ax = sns.lmplot(data = data, x = 'Days on Rig', y = 'Total Trials', hue = 'Animal', col = 'Animal', col_wrap = 3, palette="pastel", height=4, aspect=1.2, scatter_kws={"s": 10, "alpha": 1}, ci=None, order=1)
    sns.despine()
    plt.show()

#This function will find the r value and p value for the regression line for an animal
#To do this for all animals in the dataframe, use a for loop:
    #for animal in data['Animal'].unique():
        #regression_individual_mice_total_trials(data, animal)
def regression_individual_mice_total_trials(data, animal):
    '''
    args:
    data: dataframe containing water log data (Cohort data)
    animal: name of the animal to perform the regression on
    '''
    x = data[data['Animal'] == animal]['Days on Rig']
    y = data[data['Animal'] == animal]['Total Trials']
    #If there is a NaN value in the y data, drop it and the corresponding x value (the one with the same index)
    indexlist = []
    if y.isnull().values.any():
        #Get the index for the NaN value
        indexlist = y[y.isnull()].index.tolist()
        y = y.dropna()
        x = x.drop(indexlist)
    slope, intercept, r_value, p_value, std_err = stats.linregress(x, y)
    print('animal name:', animal)
    print('r: ', round(r_value, 5))
    print('p value: ', round(p_value, 5))


#This function will produce a bar plot of the total trials for each animal over days on rig
#Additonally, it will add a regression line to the plot with the confidence interval and the r value and p value
def cohort_total_trials_barplot(data):
    '''
    args:
    data: dataframe containing water log data (Cohort data)
    '''
    # Make total trials an integer
    #data['Total Trials'] = data['Total Trials'].astype(int)

    # Create the bar plot with a regression line
    fig, ax = plt.subplots()
    sns.barplot(data = data, x = 'Days on Rig', y = 'Total Trials', hue = 'Animal', palette="pastel", ax=ax)
    
    # Add regression line
    sns.regplot(data = data, x = 'Days on Rig', y = 'Total Trials', scatter=False, ax=ax)
    sns.despine()
    ax.set_ylim(bottom=0)
    plt.show()
    
    #Calculate the regression line
    x = data['Days on Rig']
    y = data['Total Trials']
    #If there is a NaN value in the y data, drop it and the corresponding x value (the one with the same index)
    indexlist = []
    if y.isnull().values.any():
        #Get the index for the NaN value
        indexlist = y[y.isnull()].index.tolist()
        y = y.dropna()
        x = x.drop(indexlist)
    slope, intercept, r_value, p_value, std_err = stats.linregress(x, y)
    print('r: ', round(r_value, 5))
    print('p value: ', round(p_value, 5))
    print('standard error: ', round(std_err, 5))

#This function will produce a dataframe with some statistics about the cohort you are analyzing
    #Number of sessions per day
    #Mean Trials per day
    #Std of trials per day
    #Number of sessions with more than 250 trials
    #Percentage of sessions with more than 250 trials
#The dataframe will be exported to a csv file
#TODO: Add an argument so the number of trials used as a benchmark can be changed
def total_trials_stats(data):
    '''
    args:
    data: dataframe containing water log data (Cohort data)
    '''
    #Calculate the average total trials per day per mouse
    total_trials_mean = data.groupby(['Days on Rig'])['Total Trials'].mean()
    total_trials_std = data.groupby(['Days on Rig'])['Total Trials'].std()
    #Calculate how many days in the cohort had above 250 trials
    total_trials_above_250 = data[data['Total Trials'] > 250].groupby(['Days on Rig'])['Total Trials'].count()
    #Divide the number of days above 250 by the total number of days to get the percentage of days above 250
    total_trials_above_250_percent = total_trials_above_250 / data.groupby(['Days on Rig'])['Total Trials'].count()
    #Calculate the total nyumber of data points per day
    total_trials_count = data.groupby(['Days on Rig'])['Total Trials'].count()
    #Put the data into a dataframe
    total_trials_stats = pd.DataFrame({'# of sessions' : total_trials_count,'Mean Trials': total_trials_mean, 'Std': total_trials_std, '# of sessions above 250 trials': total_trials_above_250, '# of sessions above 250 trials %': total_trials_above_250_percent})
    #Round the numbers
    total_trials_stats = total_trials_stats.round(2)
    #Export the data to a csv file
    total_trials_stats.to_csv('total_trials_stats.csv')
    print(total_trials_stats)
    return total_trials_stats