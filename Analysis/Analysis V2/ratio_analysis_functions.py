import seaborn as sns
import matplotlib.pyplot as plt
import pandas as pd

#Line plot of movement A ratio over days on rig for each mouse in the cohort
def ratios_line_plot(data, benchmark, title):
    '''
    args:
    data = dataframe containing the water log data
    benchmark = the benchmark value for the Movement A ratio. This range is shaded in the plot and calulated
    from benchmark to 1-benchmark. So for 0.25, the shaded area is 0.25 to 0.75.
    title = the title of the plot (string)
    '''
    #Plot the Movement A ratio over Days on Rig for each mouse in the cohort in individual subplots with markers, if the ratio is above 0.25 or below 0.75, circle the data point
    sns.FacetGrid(data, col='Animal', col_wrap=3, height=4, aspect=1.2, palette="pastel").map(plt.plot, 'Days on Rig', 'Movement A %', marker='o', markersize=5, color='black', linewidth=1, alpha=0.5)
    #Shade the area between the the benchmark and 1 for each mouse on the y axis for each subplot
    for i, ax in enumerate(plt.gcf().axes):
        ax.axhspan(benchmark, 1-benchmark, color='grey', alpha=0.2)
    #Make the title of the overall plot the name of the data
    plt.suptitle(title, fontsize=16, y=1.05)
    sns.despine()
    plt.show()

#Line plot of movement A ratio Z Scored (Z Scored within cohort) over days on rig for each mouse in the cohort
def ratios_line_plot_Z(data, title):
    '''
    args:
    data = dataframe containing the water log data
    title = the title of the plot (string)
    '''
    #Plot the Movement A ratio over Days on Rig for each mouse in the cohort in individual subplots with markers, if the ratio is above 0.25 or below 0.75, circle the data point
    sns.FacetGrid(data, col='Animal', col_wrap=3, height=4, aspect=1.2, palette="pastel").map(plt.plot, 'Days on Rig', 'Movement A % Z Score', marker='o', markersize=5, color='black', linewidth=1, alpha=0.5)
    #Make the title of the overall plot the name of the data
    plt.suptitle(title, fontsize=16, y=1.05)
    sns.despine()
    plt.show()

#Calculates the average ratio, standard deviation, and the percentage of days between 0.25 and 0.75 for each day in the cohort
def ratio_statistics(data):
    '''
    args:
    data = dataframe containing the water log data
    '''
    #Calculate the average ratio per day per mouse
    ratio_mean = data.groupby(['Days on Rig'])['Movement A %'].mean()
    ratio_std = data.groupby(['Days on Rig'])['Movement A %'].std()
    #Calculate how many days in the cohort had above between 0.25 and 0.75
    ratio_between_25_75 = data[(data['Movement A %'] > 0.25) & (data['Movement A %'] < 0.75)].groupby(['Days on Rig'])['Movement A %'].count()
    #Divide the number of days between 0.25 and 0.75 by the total number of days to get the percentage of days between 0.25 and 0.75
    ratio_between_25_75_percent = ratio_between_25_75 / data.groupby(['Days on Rig'])['Movement A %'].count()
    #Calculate the total nyumber of data points per day
    ratio_count = data.groupby(['Days on Rig'])['Movement A %'].count()
    #Put the data into a dataframe
    ratio_stats = pd.DataFrame({'# of sessions' : ratio_count,'Mean Ratio': ratio_mean, 'Std': ratio_std, '# of sessions between 0.25 and 0.75': ratio_between_25_75, '# of sessions between 0.25 and 0.75 %': ratio_between_25_75_percent})
    #Round the numbers
    ratio_stats = ratio_stats.round(2)
    #Export the data to a csv file
    ratio_stats.to_csv('ratio_stats.csv')
    return ratio_stats