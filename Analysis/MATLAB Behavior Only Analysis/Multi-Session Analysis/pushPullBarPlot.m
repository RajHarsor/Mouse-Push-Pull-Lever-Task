function [pushArray, pullArray] = pushPullBarPlot(multi_behavior_outfile, names)
%UNTITLED7 Summary of this function goes here
%   Detailed explanation goes here

    pushArray = {};
    pullArray = {};
    correctSizedTables = 0;

    for i = 1:length(names)
        for j = 1:length(multi_behavior_outfile.(names{i}).trials_tables)
                 currTable = multi_behavior_outfile.(names{i}).trials_tables{j, 2};
             if size(currTable, 2) == 15
                 correctSizedTables = correctSizedTables + 1;
                 pushPullRatio = currTable{end, 'Push/Pulls'};
                 pushTrials = strsplit(pushPullRatio{1}, ' / ');
                 pushTrials = str2double(pushTrials{1});
                 pullTrials = strsplit(pushPullRatio{1}, ' / ');
                 pullTrials = str2double(pullTrials{2});
                 pushArray{j, i} = pushTrials;
                 pullArray{j, i} = pullTrials;
             end
             if size(currTable, 2) == 17
                pushCounter = 0;
                pullCounter = 0;
                correctSizedTables = correctSizedTables + 1;
                for k = 1:length(currTable.("Push/Pull/Timeout"))
                    if currTable.("Push/Pull/Timeout")(k) == "Push"
                        pushCounter = pushCounter + 1;
                    elseif currTable.("Push/Pull/Timeout")(k) == "Pull"
                        pullCounter = pullCounter + 1;
                    end
                end
                pushArray{j, i} = pushCounter;
                pullArray{j, i} = pullCounter;
             end
        end
    end

    pushArray = cell2mat(pushArray);
    pullArray = cell2mat(pullArray);

    NumStacksPerGroup = length(names);
    NumGroupsPerAxis = correctSizedTables / size(names, 1);
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