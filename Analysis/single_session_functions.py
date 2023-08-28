# File Cleanup + Import
def importandclean_Push_Down():
    #import packages
    import pandas as pd
    #Paths for the files we are going to use
    pathCoords= input("Enter the path of the file containing the coordinates information: ")
    pathTrial= input("Enter the path of the file containing the trial information: ")

    #Take out the quotes from the path and don't replace them with anything
    pathCoords=pathCoords.replace('"','')
    pathTrial=pathTrial.replace('"','')
    
    #Read the files
    dfCoords = pd.read_csv(pathCoords, sep=',', header=None, on_bad_lines="warn")
    dfTrial = pd.read_csv(pathTrial, sep=',', header=None, names=['Trial Number', 'Reaction Time', 'Current Array', 'Current Array Push/Pull Ratio', 'Total Push/Pull Ratio', 'Solenoid Open Time', 'Push/Down', 'ISI Delay'], on_bad_lines="warn")
    # Rename the columns
    dfCoords.columns = ['X Coordinate', 'Y Coordinate', 'Time (us)', 'Phase']

    #Reset the index
    dfCoords = dfCoords.reset_index(drop=True)

    #If any of the values in the Time (us) are the same, delete the row
    dfCoords = dfCoords.drop_duplicates(subset='Time (us)', keep='first')

    #Convert the time to minutes
    dfCoords['Time (min)'] = dfCoords['Time (us)']/60000000

    #Reorder the columns
    dfCoords = dfCoords[['X Coordinate', 'Y Coordinate', 'Phase', 'Time (us)', 'Time (min)']]

    #Filter rows where the specific column does not contain a numerical value
    dfCoords = dfCoords[pd.to_numeric(dfCoords['X Coordinate'], errors='coerce').notnull()]
    dfCoords = dfCoords[pd.to_numeric(dfCoords['Y Coordinate'], errors='coerce').notnull()]
    dfCoords = dfCoords[pd.to_numeric(dfCoords['Time (min)'], errors='coerce').notnull()]

    #Any rows in the column dfTrials['Push/Pull'] that do not contain the string 'Push' or 'Pull' will be deleted
    dfTrial = dfTrial[dfTrial['Push/Down'].str.contains('Push|Down')]

    #Make thew columns X and Y coordinates and Time(us) and Time(min) into floats
    dfCoords['X Coordinate'] = dfCoords['X Coordinate'].astype(int)
    dfCoords['Y Coordinate'] = dfCoords['Y Coordinate'].astype(int)
    dfCoords['Time (us)'] = dfCoords['Time (us)'].astype(float)
    dfCoords['Time (min)'] = dfCoords['Time (min)'].astype(float)

    #Delete any rows in the X or Y Coordinate columns that are over 1000
    dfCoords = dfCoords[dfCoords['X Coordinate'] < 1000]
    dfCoords = dfCoords[dfCoords['Y Coordinate'] < 1000]
    

def importandclean_Push_Pull():
    #import packages
    import pandas as pd
    #Paths for the files we are going to use
    pathCoords= input("Enter the path of the file containing the coordinates information: ")
    pathTrial= input("Enter the path of the file containing the trial information: ")

    #Take out the quotes from the path and don't replace them with anything
    pathCoords=pathCoords.replace('"','')
    pathTrial=pathTrial.replace('"','')
    
    #Read the files
    dfCoords = pd.read_csv(pathCoords, sep=',', header=None, on_bad_lines="warn")
    dfTrial = pd.read_csv(pathTrial, sep=',', header=None, names=['Trial Number', 'Reaction Time', 'Current Array', 'Current Array Push/Pull Ratio', 'Total Push/Pull Ratio', 'Solenoid Open Time', 'Push/Pull', 'ISI Delay'], on_bad_lines="warn")
    # Rename the columns
    dfCoords.columns = ['X Coordinate', 'Y Coordinate', 'Time (us)', 'Phase']

    #Reset the index
    dfCoords = dfCoords.reset_index(drop=True)

    #If any of the values in the Time (us) are the same, delete the row
    dfCoords = dfCoords.drop_duplicates(subset='Time (us)', keep='first')

    #Convert the time to minutes
    dfCoords['Time (min)'] = dfCoords['Time (us)']/60000000

    #Reorder the columns
    dfCoords = dfCoords[['X Coordinate', 'Y Coordinate', 'Phase', 'Time (us)', 'Time (min)']]

    #Filter rows where the specific column does not contain a numerical value
    dfCoords = dfCoords[pd.to_numeric(dfCoords['X Coordinate'], errors='coerce').notnull()]
    dfCoords = dfCoords[pd.to_numeric(dfCoords['Y Coordinate'], errors='coerce').notnull()]
    dfCoords = dfCoords[pd.to_numeric(dfCoords['Time (min)'], errors='coerce').notnull()]

    #Any rows in the column dfTrials['Push/Pull'] that do not contain the string 'Push' or 'Pull' will be deleted
    dfTrial = dfTrial[dfTrial['Push/Pull'].str.contains('Push|Pull')]

    #Make thew columns X and Y coordinates and Time(us) and Time(min) into floats
    dfCoords['X Coordinate'] = dfCoords['X Coordinate'].astype(int)
    dfCoords['Y Coordinate'] = dfCoords['Y Coordinate'].astype(int)
    dfCoords['Time (us)'] = dfCoords['Time (us)'].astype(float)
    dfCoords['Time (min)'] = dfCoords['Time (min)'].astype(float)

    #Delete any rows in the X or Y Coordinate columns that are over 1000
    dfCoords = dfCoords[dfCoords['X Coordinate'] < 1000]
    dfCoords = dfCoords[dfCoords['Y Coordinate'] < 1000]