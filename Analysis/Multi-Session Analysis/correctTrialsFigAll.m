function [correctTrialsArray, CorrectTrialFig] = correctTrialsFigAll(multi_behavior_outfile, names)
%UNTITLED5 Summary of this function goes here
%   Detailed explanation goes here
    correctTrialsArray = {};

    for i = 1:length(names) % Iterating through structs of mice
        for j = 1:length(multi_behavior_outfile.(names{i}).trials_tables) %Iterating through trials_tables
            currTable = multi_behavior_outfile.(names{i}).trials_tables{j, 2};
            correctTrialsRatio = currTable{end, 'Correct/Total Trials'};
            correctTrials = strsplit(correctTrialsRatio{1}, ' / ');
            correctTrials = str2double(correctTrials{1});
            correctTrialsArray{j, i} = correctTrials;
        end
    end

    correctTrialsArray = cell2mat(correctTrialsArray);

    CorrectTrialFig = figure;
    plot(correctTrialsArray, '--o');
    title("Days vs Correct Trials")
    xlabel("Days");
    ylabel("# of Correct Trials");
    legend(names, Interpreter="none");
end