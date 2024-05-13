%% User Inputs
rootdir = 'C:\Users\rdh92-adm\Box\Oldenburg-Shared\Behavioral Testing\Cohort 14';
waterLogPath = "C:\Users\rdh92-adm\Box\Oldenburg-Shared\Water Log.xlsx";
SheetName = "Cohort 14 - VC"; % Name of the sheet in the waterLog file

%% Makes the Outfile 
% Please note this takes a long time as there is a lot of data usually in
% the directories
warning('off');
[multi_behavior_outfile, names] = makeOutfile(rootdir, waterLogPath, SheetName);
warning('on');

%% Correct Trials Over Days (All)

[correctTrialsArray] = correctTrialsFigAll(multi_behavior_outfile, names);

%% Correct Pushes and Pulls Over Days 

[pushArray, pullArray] = pushPullBarPlot(multi_behavior_outfile, names);

%% RT Over Days, All Correct Trials

[correctRTArray, correctRTMeans] = histRTCorrectTrials(multi_behavior_outfile, names);