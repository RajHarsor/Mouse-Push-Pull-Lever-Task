# File Cleanup + Import
def importandclean():
    #import packages
    import openpyxl as xl
    import re
    import inquirer
    import pandas as pd
    #Paths for the files we are going to use
    pathCoords= input("Enter the path of the file containing the coordinates information: ")
    pathTrial= input("Enter the path of the file containing the trial information: ")
    pathGraphValues= input("Enter the path of the water log file: ")

    #Take out the quotes from the path and don't replace them with anything
    pathGraphValues=pathGraphValues.replace('"','')
    pathCoords=pathCoords.replace('"','')
    pathTrial=pathTrial.replace('"','')
    
    #Select the name of the sheet where the information is located in the water log file
    wb = xl.load_workbook(pathGraphValues)

    SheetNames = []
    sheet_names = wb.sheetnames

    import inquirer

    questions = [
        inquirer.List('sheet',
                    message="What sheet is your mouse located in?",
                    choices = sheet_names,
        ),
    ]

    answers = inquirer.prompt(questions)
    SheetName = answers['sheet']
    
    #Read the files
    dfCoords = pd.read_csv(pathCoords, sep=',', header=None, on_bad_lines="warn")
    dfTrial = pd.read_csv(pathTrial, sep=',', header=None, names=['Trial Number', 'Reaction Time', 'Current Array', 'Current Array Push/Pull Ratio', 'Total Push/Pull Ratio', 'Solenoid Open Time', 'Push/Pull', 'ISI Delay'], on_bad_lines="warn")
    xls = pd.ExcelFile(pathGraphValues)
    dfGraphValues = pd.read_excel(xls, SheetName)

    #Drop the rows that we don't need in the dfGraphValues dataframe
    dfGraphValues = dfGraphValues.drop(['Start Weight (g)', 'Current Weight (g)', 'Percent', 'Water to 85%', '25mg/kg', '50mg/kg', '10mg/kg', 'Recommend (ml)', 'Give (ml)', 'Earned (ml)', 'Program', 'Time Started', 'ISI Upper Range (ms)', 'ISI Upper Range (ms)', 'ISI Lower Range (ms)'], axis=1)

    #Get the Animal Name and date from the path
    import re

    animalNamePattern = r'\\(F\d+\.\d+-\d+)\\'
    match1 = re.search(animalNamePattern, pathTrial)
    animalName = match1.group(1)

    datePattern = r'\\(\d{6})\\'
    match2 = re.search(datePattern, pathTrial)
    date = int(match2.group(1))

    #Filter the dfGraphValues dataframe by the Animal Name
    dfGraphValues = dfGraphValues[dfGraphValues['Animal'] == animalName]

    #Find the row where the date is the same as the date in the path
    dfGraphValues = dfGraphValues[dfGraphValues['Date'] == date]

    #Get information from the water log file
    XUpperBound = dfGraphValues['Rest Upper Value (X Coordinate)'].iloc[0]
    XLowerBound = dfGraphValues['Rest Lower Value (X Coordinate)'].iloc[0]
    PushValue = dfGraphValues['Push Value (X Coordinate)'].iloc[0]
    PullValue = dfGraphValues['Pull Value (X Coordinate)'].iloc[0]

    #Edit index 0, row 0 to delete the values before the t including the t
    #dfCoords.at[0, 0] = dfCoords.at[0, 0].split('\t')[1]

    # Rename the columns
    dfCoords.columns = ['X Coordinate', 'Y Coordinate', 'Time (us)', 'Phase']

    #Get rid of the even numbered rows
    #dfCoords = dfCoords.iloc[::2, :]

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