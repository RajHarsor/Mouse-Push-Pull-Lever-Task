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

    #Drop rows in dfTrial that have NaN values
    dfTrial = dfTrial.dropna()

    #Reset the index
    dfTrial = dfTrial.reset_index(drop=True)

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

    #Drop rows in dfTrial that have NaN values
    dfTrial = dfTrial.dropna()
    
    #Reset the index
    dfTrial = dfTrial.reset_index(drop=True)
    
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

def spreadsheetInformation_Single():
    #Import pandas
    import pandas as pd

    #Take out the quotes from the path and don't replace them with anything
    global pathTrial
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

    #Calculate average reaction time
    AverageReactionTime = dfTrial['Reaction Time'].mean()

    #Calculate the average ISI Delay
    AverageISIDelay= dfTrial['ISI Delay'].mean()

    # Put information above into a dataframe
    dfTrialInfo = pd.DataFrame({'Average Reaction Time (ms)': [AverageReactionTime], 'Average ISI Delay': [AverageISIDelay]})

    #Make sure we can see all the dataframes in the table below
    pd.set_option('display.max_columns', None)
    pd.set_option('display.max_rows', None)

    print(dfTrialInfo)
    
def XCoordsWithRewardsOverTime():
    #Import matplotlib
    import matplotlib.pyplot as plt
    #Input the value of the pull and push coordinates
    PullValue = int(input("Enter the value of the pull coordinate: "))
    PushValue = int(input("Enter the value of the push coordinate: "))

    #Find the values where it goes from Stage 2 to Stage 3
    x_values = []
    Time = []

    previous_stage = None

    global dfCoords
    for index, row in dfCoords.iterrows():
        current_stage = row['Phase']

        if previous_stage == ' Stage 2: Push/Pull ' and current_stage == ' Stage 3: Water Retrival ':
            x_values.append(row['X Coordinate'])
            Time.append(row['Time (min)'])

        previous_stage = current_stage

    #Drop columns that are duplicates in the Time (min) column
    dfCoords = dfCoords.drop_duplicates(subset='Time (min)', keep='first')

    #Plot X Coordinate vs Time (min)
    plt.plot(dfCoords['Time (min)'], dfCoords['X Coordinate'], color='black')

    #Draw dotted red lines at the pull and push coordinate values
    plt.axhline(y=PullValue, color='red', linestyle='--', alpha = 0.5)
    plt.axhline(y=PushValue, color='red', linestyle='--', alpha = 0.5)

    #Make the image longer
    plt.gcf().set_size_inches(20, 5)

    #Set labels
    plt.xlabel('Time (min)')
    plt.ylabel('X Coordinate')

    #Remove top and right spines
    plt.gca().spines['top'].set_visible(False)
    plt.gca().spines['right'].set_visible(False)


    #Make a list with just the int 550 in it that is the same length as the length of Time
    y = [dfCoords['X Coordinate'].max() + 5] * len(Time)

    global dfTrial
    #Make dfTrial['Phase'] into a list
    phase = dfTrial['Push/Pull'].tolist()

    phase_colors = []
    #For the values in Phase, if it is a push, make it orange, if it is a pull, make it blue
    for x in phase:
        if x == ' Push ':
            phase_colors.append('orange')
        elif x == ' Pull ':
            phase_colors.append('blue')
        else:
            phase_colors.append('black')

    #Make sure phase_colors is the same length as Time, if it isn't add 'black' until it is
    while len(phase_colors) < len(Time):
        phase_colors.append('black')

    #Make a scatter plot of the X and Y coordinates where it goes from Stage 2 to Stage 3 ands change the color based on phase list
    plt.scatter(Time, y, color=phase_colors, marker='|', alpha = 0.5)

    #Add Title
    plt.title('X Coordinate vs Time (min)')

    #Zoom into min 10 - 20 on the graph
    #plt.xlim(40, 50)
    #plt.ylim(490, 550)

    #Show the plot
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
        plt.savefig('X Coordinate with Rewards Over Time.svg', format='svg', dpi=1200)
    else:
        pass
    
def PieGraphTime():
    #Import matplotlib
    import matplotlib.pyplot as plt

    global dfCoords
    #Calculate the difference in time in us between each subsequent row in the dataframe
    dfCoords['Time (us) Difference'] = dfCoords['Time (us)'].diff()

    #Make new dataframe with information
    phase_times = dfCoords.groupby("Phase")["Time (us) Difference"].sum().reset_index()

    #Initialize the figure
    fig5, ax = plt.subplots()

    #Make the pie graph
    ax.pie(phase_times['Time (us) Difference'], labels=phase_times['Phase'], autopct='%1.1f%%', pctdistance= 1.1, labeldistance= 1.3, startangle=90, counterclock=False)

    # Show the plot
    plt.show()
