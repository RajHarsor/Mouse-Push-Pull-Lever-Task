function [multi_behavior_outfile, names] = makeOutfile(filelist, waterLogPath, SheetName)
%UNTITLED3 Summary of this function goes here
%   Detailed explanation goes here

    % Load in the water log
    waterLog = readtable(waterLogPath, Sheet=SheetName);
    waterLog.Date = num2str(waterLog.Date);
    waterLog.Date = string(waterLog.Date);
    waterLog.Animal = string(waterLog.Animal);
    
    for i=1:length(waterLog.Animal)
        waterLog.Animal(i) = replace(waterLog.Animal(i), '.', '_');
    end
    
    for i=1:length(waterLog.Animal)
        waterLog.Animal(i) = replace(waterLog.Animal(i), '-', '_');
    end

    % Make the struct + Organize the data in the struct
    multi_behavior_outfile = struct();

    for i=1:length(filelist)
        filename = filelist(i).name;
        path = filelist(i).folder;
        parts = strsplit(filename, '_');
        name = parts{1};
        name = regexprep(name, '[^\w_]', '_');
        date = parts{2};
        type = parts{3};
    
        if ~isfield(data_struct, name)
            data_struct.(name) = struct();
            data_struct.(name).trials_filelist = {};
            data_struct.(name).coordinates_filelist = {};
            data_struct.(name).trials_tables = {};
            data_struct.(name).coordinates_tables = {};
        end
        
        % Importing in the data for the Coordinates Data and the Trials Data
        if strcmp(type, 'Coordinates.txt')
            data_struct.(name).coordinates_filelist{end + 1, 1} = filename;
            data_struct.(name).coordinates_filelist{end, 2} = path;
            data_struct.(name).coordinates_tables{end + 1, 1} = date;
            data_struct.(name).coordinates_tables{end, 2} = readtable(strcat(path, "\", filename), ReadVariableNames=false, MissingRule="omitrow");
            data_struct.(name).coordinates_tables{end, 2}.Properties.VariableNames = {'X Coordinate', 'Y Coordinate', 'Time (ms)', 'Stage', 'Lick Status (not working atm)'};
            data_struct.(name).coordinates_tables{end, 2}.XSmooth = smoothdata(data_struct.(name).coordinates_tables{end, 2}.("X Coordinate"));
            data_struct.(name).coordinates_tables{end, 2}.YSmooth = smoothdata(data_struct.(name).coordinates_tables{end, 2}.("Y Coordinate"));
        elseif strcmp(type, 'Trials.txt')
            data_struct.(name).trials_filelist{end + 1, 1} = filename;
            data_struct.(name).trials_filelist{end, 2} = path;
            data_struct.(name).trials_tables{end + 1, 1} = date;
            data_struct.(name).trials_tables{end, 2} = readtable(strcat(path, "\", filename), ReadVariableNames=true);
            if size(data_struct.(name).trials_tables{end, 2}, 2) == 15
                data_struct.(name).trials_tables{end, 2}.Properties.VariableNames = {'Trial Number', 'Reaction Time', 'Symbol', 'Push/Pull/Timeout', 'Correct/Incorrect', 'Correct/Total Trials', 'Solenoid Open Time', 'Push/Pulls', 'ISI Delay', 'Debugging Information to the right', 'Lever Hold Time', 'Reaction Time Window', 'Punishment Length', 'ISI Delay Lower Range', 'ISI Delay Upper Range'};
            elseif size(data_struct.(name).trials_tables{end, 2}, 2) == 17
               data_struct.(name).trials_tables{end, 2}.Properties.VariableNames = {'Trial Number', 'Reaction Time', 'Symbol', 'Push/Pull/Timeout', 'Correct/Incorrect', 'Correct/Total Trials', 'Solenoid Open Time,' 'ISI Delay', 'Debugging Information to the right', 'Plus Percentage', 'Vertical Percentage', 'Horizontal Percentage', 'Lever Hold Time', 'Reaction Time Window', 'Punishment Length', 'ISI Delay Lower Range', 'ISI Delay Upper Range'};
            end
        end
    end

    names = fieldnames(data_struct); 

    % Find push and pull values from the waterLog (means that it needs to
    % be updated and correct)
    for i=1:length(names) % Iterate through mouse names
        mouse_name = names{i};
        for j = 1:length(data_struct.(names{i}).coordinates_tables(:, 1)) %Iterate through coordinate tables
            waterDate = data_struct.(names{i}).coordinates_tables{j, 1};
            
            for k = 1:length(waterLog.Date)
                if (mouse_name == waterLog.Animal(k) && waterDate == waterLog.Date(k))
                    data_struct.(names{i}).coordinates_tables{j, 3} = waterLog.PushValue_XCoordinate_(k);
                    data_struct.(names{i}).coordinates_tables{j, 4} = waterLog.PullValue_XCoordinate_(k);
                end
            end
        end
    end
end