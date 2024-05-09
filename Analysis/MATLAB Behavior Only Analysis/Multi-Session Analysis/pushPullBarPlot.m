function [pushArray, pullArray] = pushPullBarPlot(multi_behavior_outfile, names)
%UNTITLED7 Summary of this function goes here
%   Detailed explanation goes here

    pushArray = {};
    pullArray = {};

    for i = 1:length(names)
        for j = 1:length(multi_behavior_outfile.(names{i}).trials_tables)
             currTable = multi_behavior_outfile.(names{i}).trials_tables{j, 2};
             pushPullRatio = currTable{end, 'Push/Pulls'};
             pushTrials = strsplit(pushPullRatio{1}, ' / ');
             pushTrials = str2double(pushTrials{1});
             pullTrials = strsplit(pushPullRatio{1}, ' / ');
             pullTrials = str2double(pullTrials{2});
             pushArray{j, i} = pushTrials;
             pullArray{j, i} = pullTrials;
        end
    end

    pushArray = cell2mat(pushArray);
    pullArray = cell2mat(pullArray);

    NumStacksPerGroup = length(names);
    NumGroupsPerAxis = length(multi_behavior_outfile.(names{i}).trials_tables);
    NumStackElements = 2;

    for i = 1:length(multi_behavior_outfile.(names{i}).trials_tables)
        groupLabels{i} = i;
    end

    stackData = reshape(cat(NumStacksPerGroup, pushArray, pullArray), NumGroupsPerAxis, NumStacksPerGroup, NumStackElements);

    plotBarStackGroups(stackData, groupLabels);
    xlabel("Days");
    ylabel("Count");
    title("Push and Pulls over Days");

end