#%% Read Files

# Import pandas
import pandas as pd

#Path for Coordinate file
pathCoords = input("Enter the path of the coordinate file: ")
pathCoords = pathCoords.replace('"','')

#Read the csv file
dfCoords = pd.read_csv(pathCoords, sep=',', header=None, on_bad_lines="warn")

#Rename the columns
dfCoords.columns = ['X Coordinate', 'Y Coordinate', 'Time (us)', 'Phase']

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

#Delete any rows in the X or Y Coordinate columns that are over 1000
dfCoords = dfCoords[dfCoords['X Coordinate'] < 1000]
dfCoords = dfCoords[dfCoords['Y Coordinate'] < 1000]

#Reset the index
dfCoords = dfCoords.reset_index(drop=True)
#%% Cut the dataframes by trials

#Create a list of the indexes where the phase changes from Stage 3 to Stage 1
indexListN = []

previous_stage = None

for index, row in dfCoords.iterrows():
    current_stage = row['Phase']
    
    if previous_stage == ' Stage 4: ISI ':
        indexListN.append(index)
        
    previous_stage = current_stage

# %% Cut the dataframe into trials
index_start = 0
trial_number = 1
trial_list = []

for i in range(len(indexListN)):
    trial_var_name = f"trial_{trial_number}"
    trial_df = dfCoords.iloc[index_start:indexListN[i]]  # Slice the dataframe
    #Put dataframe into list
    trial_list.append(trial_var_name)
    print(f"Creating {trial_var_name} with indexes: {index_start}:{indexListN[i]}")
    
    if not trial_df.empty:
        locals()[trial_var_name] = trial_df
    else:
        print(f"{trial_var_name} is empty")
        
    index_start = indexListN[i]
    trial_number += 1
    # Add the last trial
    if i == len(indexListN) - 1:
        trial_var_name = f"trial_{trial_number}"
        trial_df = dfCoords.iloc[index_start:]
        trial_list.append(trial_var_name)
        print(f"Creating {trial_var_name} with indexes: {index_start}:end")
        if not trial_df.empty:
            locals()[trial_var_name] = trial_df
        else:
            print(f"{trial_var_name} is empty")

# %% 

#
