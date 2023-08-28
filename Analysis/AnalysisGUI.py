#%%

#Import Packages
import pandas as pd
import numpy as np
import inquirer
import single_session_functions as ssaf

#Ask the user if they want to do the single session analysis or the multi session analysis
questions = [
    inquirer.List('Analysis',
                    message="Which analysis would you like to perform?",
                    choices=['Single Session', 'Multi Session'],
    ),
]
answers = inquirer.prompt(questions)
#%%
if answers['Analysis'] == 'Single Session':
#Ask the user whether they want either the Push/Down or Push/Pull Analysis
    questions = [
        inquirer.List('Analysis',
                        message="Which analysis would you like to perform?",
                        choices=['Push/Down', 'Push/Pull'],
        ),
    ]
    answers = inquirer.prompt(questions)
#%%
#Import and Clean Data
if answers['Analysis'] == 'Push/Down':
    ssaf.importandclean_Push_Down()
    
if answers['Analysis'] == 'Push/Pull':
    ssaf.importandclean_Push_Pull()