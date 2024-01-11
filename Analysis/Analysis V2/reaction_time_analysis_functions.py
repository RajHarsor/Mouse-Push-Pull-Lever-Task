import pandas as pd
import polars as pl
import matplotlib.pyplot as plt
import seaborn as sns
import numpy as np
import os
import itertools

#This is only for one day
def load_trials_data_for_rt_and_plot(path, day):
    '''
    args:
        path: path to the trials.txt file
        day: whatever day the mouse is on for training
    
    returns:
        numpy array of reaction times (rt)
        numpy array of reaction times under 10000 ms (rt_appended)
        histogram of reaction times under 10000 ms
        # of trials under 10000 ms out of total trials
    '''
    df = pd.read_csv(path, names=["Trials Number", 'Reaction Time', 'Array', 'Array Counts', 'Total Counts', 'Solenoid Open Time', 'Movement', 'ISI'])

    #Transfer the reaction time data to a numpy array
    rt = df['Reaction Time'].to_numpy()

    rt_appended = []

    #If the reaction time is greater than 10000, append it to the empty list
    for i in rt:
        if i < 10000:
            rt_appended.append(i)

    rt_appended = np.array(rt_appended)

    #Plot the reaction time data
    fig, ax = plt.subplots()

    #Coplot kde plot and histogram on the same graph using the kdeplot function and the hist function
    sns.histplot(rt_appended, ax=ax, kde=True, bins=20, kde_kws={'cut' : 0, 'bw_adjust' : 0.6})
    #Only show whole numbers on the y axis
    plt.gca().get_yaxis().set_major_locator(plt.MaxNLocator(integer=True))

    #Increment the x axis by 1000 ms
    plt.xticks(np.arange(0, 10000, 1000))

    sns.despine()

    #Set a title and labels
    plt.title(f"Reaction Time Distribution Day {day}")
    plt.xlabel("Reaction Time (ms)")
    plt.ylabel("Number of Trials")

    print(f"Out of {len(rt)} trials, {len(rt_appended)} trials were less than 10000 ms.")

def get_trials_files_paths(overall_path):
    '''
    args:
        overall_path: path to Cohort folder
    returns:
        multiple lists of paths to trials.txt files (seperated by animal) - path{i}
        one list of all the reaction times in the cohort (list of arrays) - data_list
    '''
    d = {}
    paths_dict = {}
    
    #Find the trials.txt files in the folder and its subfolders
    for root, dirs, files in os.walk(overall_path):
        for file in files:
            if file.endswith("Trials.txt"):
                #Get the path to the file
                path = os.path.join(root, file)
                #Get the folder two levels above the file
                parent_folder = os.path.basename(os.path.dirname(os.path.dirname(path)))
                #Put the path and parent folder into the dictionary
                d[path] = parent_folder
    
    #Split the paths based on the unique values in the dictionary into a list of lists
    paths = [list(g) for _, g in itertools.groupby(d, d.get)]

    #Seperate each list in the list of lists into a seperate list
    for i in range(len(paths)):
        paths_dict[f"paths{i}"] = paths[i]

    paths_dict
    
    #Return the dictionary and the lists I made
    return d, paths_dict


def data_list_from_paths_list(dict_key):
    df_dict = {}
    rt_dict = {}
    rt_appended_dict = {}

    for i in paths_dict['dict_key']:
        df_dict[i] = pd.read_csv(i, names=['Trials Number', 'Reaction Time', 'Array', 'Array Counts', 'Total Counts', 'Solenoid Open Time', 'Movement', 'ISI'])
        rt_dict[i] = df_dict[i]['Reaction Time'].to_numpy()
        rt_appended_dict[i] = [j for j in rt_dict[i] if j < 10000]
        rt_appended_dict[i] = np.array(rt_appended_dict[i])
    
    return rt_appended_dict

def subplot_reaction_time():
    '''
    Should only be used after data_list_from_paths_list
    '''
    string = chosen_path_list[0]
    parts = string.split("\\")
    mouse_name = parts[7]
    
    number_of_plots = len(path_list_index)
    fig, axs = plt.subplots(1, number_of_plots, figsize=(20, 5))
    fig.suptitle(f"{mouse_name} Reaction Time Distribution")

    for i, ax in enumerate(axs):
        sns.histplot(data_list[i], ax=ax, kde=True, bins=20, kde_kws={'cut' : 0, 'bw_adjust' : 0.6})
        ax.set_title(f"Reaction Time Distribution Day {path_list_index[i] + 1}")
        ax.get_yaxis().set_major_locator(plt.MaxNLocator(integer=True))

    #Set x axis title
    fig.text(0.5, 0.04, 'Reaction Time (ms)', ha='center')