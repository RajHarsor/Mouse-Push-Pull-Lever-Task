function [outputArg1,outputArg2] = trajectoryAnalysis(multi_behavior_outfile, names, method)
%UNTITLED Summary of this function goes here
%   Detailed explanation goes here
    % Constant
    MinPeakDistance = 500;
    MinPeakProminence = 10;

    switch (method)
        case "centroid"
            for i = 1:length(names)
                for j = 1:length(multi_behavior_outfile.(names{i}).coordinates_tables)
                    currTable = multi_behavior_outfile.(names{i}).coordinates_tables{j, 2};

                end
                    for k = 1:length(currTable.("XSmooth"))
                        [posPeaks, posPeakIndices] = findpeaks(currTable.XSmooth, MinPeakDistance = MinPeakDistance, MinPeakProminence = MinPeakProminence, MinPeakHeight=)
                    end
            end
        case "array"
    end
end