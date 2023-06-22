#%% Import the necessary packages and file

#Import pandas
import pandas as pd

#Give the path of the file containing the overall information
pathOverall= input("Enter the path of the file containing the overall information: ")

#Take out the quotes from the path and don't replace them with anything
pathOverall=pathOverall.replace('"','')

#Read the excel file
xls = pd.ExcelFile(pathOverall)
df = pd.read_excel(xls, 'Cohort 3')

#Show the dataframe
print(df)