# Import Packages
import pandas as pd
import numpy as np
import inquirer
import single_session_functions as ssaf
import multiple_session_functions as msaf

# Ask the user if they want to do single session or multi-session analysis
questions = [
    inquirer.List('Choice',
                  message="Which analysis would you like to perform?",
                  choices=['Single Session', 'Multi Session'],
    ),
]
choice_answers = inquirer.prompt(questions)

if choice_answers['Choice'] == 'Single Session':
    # Ask the user whether they want Push/Down or Push/Pull Analysis
    questions = [
        inquirer.List('SingleAnalysis',
                      message="Which analysis would you like to perform?",
                      choices=['Push/Down', 'Push/Pull'],
        ),
    ]
    single_analysis_answers = inquirer.prompt(questions)

    # Single Session Analysis Pathways
    if single_analysis_answers['SingleAnalysis'] == 'Push/Down':
        ssaf.importandclean_Push_Down()
    elif single_analysis_answers['SingleAnalysis'] == 'Push/Pull':
        ssaf.importandclean_Push_Pull()
    
    # Ask for the type of graph for Single Session Analysis
    questions = [
        inquirer.List('GraphType',
                      message="What graph would you like to make?",
                      choices=['Information for Spreadsheet', 'Line Plot (Coordinates over Time)', 'Line Plot (Coordinates over Time) Smoothed'],
        ),
    ]
    graph_type_answers = inquirer.prompt(questions)

    # Single Session Analysis Options
    if graph_type_answers['GraphType'] == 'Line Plot (Coordinates over Time)':
        ssaf.CoordsOverTime()
    elif graph_type_answers['GraphType'] == 'Line Plot (Coordinates over Time) Smoothed':
        ssaf.CoordsOverTime_Smoothed()
    elif graph_type_answers['GraphType'] == 'Information for Spreadsheet':
        ssaf.spreadsheetInformation_Single()
        print("Please note if you need other information like Push Amounts or Down Amounts, you can find that information in the text file")

elif choice_answers['Choice'] == 'Multi Session':
    # Ask the user what type of Multi Session Analysis they want to perform
    questions = [
        inquirer.List('MultiAnalysis',
                      message="Which analysis would you like to perform?",
                      choices=['Multi Session Spreadsheet Analysis'],
        ),
    ]
    multi_analysis_answers = inquirer.prompt(questions)

    # Multi Session Analysis Pathways
    if multi_analysis_answers['MultiAnalysis'] == 'Multi Session Spreadsheet Analysis':
        msaf.multi_session_spreadsheet_analysis()