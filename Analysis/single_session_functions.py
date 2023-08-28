# File Cleanup + Import
def importandclean_Push_Down():
    #import packages
    import pandas as pd
    #Paths for the files we are going to use
    global pathCoords
    pathCoords= input("Enter the path of the file containing the coordinates information: ")
    global pathTrial
    pathTrial= input("Enter the path of the file containing the trial information: ")

    #Take out the quotes from the path and don't replace them with anything
    pathCoords=pathCoords.replace('"','')
    pathTrial=pathTrial.replace('"','')
    
    #Read the files
    global dfCoords
    dfCoords = pd.read_csv(pathCoords, sep=',', header=None, on_bad_lines="warn")
    global dfTrial
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
    global pathCoords
    pathCoords= input("Enter the path of the file containing the coordinates information: ")
    global pathTrial
    pathTrial= input("Enter the path of the file containing the trial information: ")

    #Take out the quotes from the path and don't replace them with anything
    pathCoords=pathCoords.replace('"','')
    pathTrial=pathTrial.replace('"','')
    
    #Read the files
    global dfCoords
    dfCoords = pd.read_csv(pathCoords, sep=',', header=None, on_bad_lines="warn")
    global dfTrial
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

def CoordsOverTime():
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

    import inquirer
    questions = [
        inquirer.List('Save',
                        message = "Would you like to save this graph? (saves as a .svg file)",
                        choices = ['Yes', 'No'],
        ),
    ]
    answers = inquirer.prompt(questions)
    if answers['Save'] == 'Yes':
        fig2.savefig('Coordinates vs Time.svg', format='svg', dpi=1200)
    else:
        pass

def CoordsOverTime_Smoothed():
    #Apply some smoothing to the X and Y coordinates using the Savitzky-Golay filter
    from scipy.signal import savgol_filter

    #Create a new column for the smoothed X Coordinate
    dfCoords['X Coordinate Smoothed'] = savgol_filter(dfCoords['X Coordinate'], 51, 3)

    #Create a new column for the smoothed Y Coordinate
    dfCoords['Y Coordinate Smoothed'] = savgol_filter(dfCoords['Y Coordinate'], 51, 3)

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
    
    import inquirer
    questions = [
        inquirer.List('Save',
                        message = "Would you like to save this graph? (saves as a .svg file)",
                        choices = ['Yes', 'No'],
        ),
    ]
    answers = inquirer.prompt(questions)
    if answers['Save'] == 'Yes':
        fig4.savefig('Smoothed Coordinates vs Time.svg', format='svg', dpi=1200)
    else:
        pass
    