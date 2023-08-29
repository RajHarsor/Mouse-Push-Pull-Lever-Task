def multi_session_spreadsheet_analysis():
    import os
    import pandas as pd

    # Specify the directory you want to search in
    directory_path = input("Enter the directory path: ")

    # Define the partial search term
    partial_search_term1 = "Trials"

    # Initialize a list to store matching file paths
    matching_files = []

    # Walk through the directory and its subdirectories
    for root, _, files in os.walk(directory_path):
        for file in files:
            file_name = file.lower()  # Convert to lowercase for case-insensitive search
            if partial_search_term1.lower() in file_name:
                matching_files.append(os.path.join(root, file))

    # Create individual numbered dataframes from matching files
    dfs = []

    for idx, file in enumerate(matching_files, start=1):
        df = pd.read_csv(file, sep=',', header=None, names=['Trial Number', 'Reaction Time', 'Current Array', 'Current Array Push/Pull Ratio', 'Total Push/Pull Ratio', 'Solenoid Open Time', 'Push/Pull', 'ISI Delay'], on_bad_lines="warn")
        dfs.append(df)
        # Rename the DataFrame
        globals()[f'df{idx}'] = df

    #For each dataframe, drop the rows that have NaN values
    for idx, df in enumerate(dfs, start=1):
        df = df.dropna()
        dfs[idx-1] = df
        
    #For each dataframe, reset the index
    for idx, df in enumerate(dfs, start=1):
        df = df.reset_index(drop=True)
        dfs[idx-1] = df
        
    #For each dataframe, change the Reaction Time, Trial Number, and ISI Delay column to a float
    for idx, df in enumerate(dfs, start=1):
        df['Reaction Time'] = df['Reaction Time'].astype(float)
        #df['Trial Number'] = df['Trial Number'].astype(float)
        df['ISI Delay'] = df['ISI Delay'].astype(float)
        dfs[idx-1] = df

    #For each dataframe, calculate average reaction time
    AverageReactionTime = []
    for idx, df in enumerate(dfs, start=1):
        AverageReactionTime.append(df['Reaction Time'].mean())
        
    #For each dataframe, calculate the average ISI Delay
    AverageISIDelay = []
    for idx, df in enumerate(dfs, start=1):
        AverageISIDelay.append(df['ISI Delay'].mean())

    #For matching files, make a new list with only the string after the last slash
    matching_files = [x.split('/')[-1] for x in matching_files]

    #For each dataframe, put information above into a dataframe with the imported file name
    dfTrialInfo = []
    for idx, df in enumerate(dfs, start=1):
        dfTrialInfo.append(pd.DataFrame({'Average Reaction Time (ms)': [AverageReactionTime[idx-1]], 'Average ISI Delay': [AverageISIDelay[idx-1]]}, index=[matching_files[idx-1]]))

    #Print the dataframe
    print(dfTrialInfo)
