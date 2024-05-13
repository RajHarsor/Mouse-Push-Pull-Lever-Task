function [correctRTArray, correctRTMeans] = histRTCorrectTrials(multi_behavior_outfile, names)

    % Finding all the correct trials throughout the trials_tables
    correctRTArray = {};
    for i = 1:length(names) % Iterating through the mouse structs
        for j = 1:length(multi_behavior_outfile.(names{i}).trials_tables) %Iterating through trials_tables list
            currTable = multi_behavior_outfile.(names{i}).trials_tables{j, 2};
            
            currentCorrectRTArray = [];
            for k = 1:length(currTable.("Reaction Time")) %Iterating through the trials table
                if (currTable.("Correct/Incorrect")(k) == "Correct")
                   currentCorrectRTArray(end+1) = currTable.("Reaction Time")(k); % Adding the RT to the list
                end 
            end
            correctRTArray{j, i} = currentCorrectRTArray';
        end
    end
    
    % Calculating the mean of the RTArray
    correctRTMeans = {};
    for i = 1:size(correctRTArray, 1)
        for j = 1:size(correctRTArray, 2)
            correctRTMeans{i,j} = mean(correctRTArray{i,j});
        end
    end

    % Making the histogram
    figure;
    for i = 1:size(correctRTArray, 1) * size(correctRTArray, 2)
        subplot(size(correctRTArray, 2), size(correctRTArray, 1), i)
        histfit(correctRTArray{i}, 20, 'kernel');
        hold on;
        xline(correctRTMeans{i}, Label={round(correctRTMeans{i})});
        xlim([0 2000]);
        if i <= (size(correctRTArray, 1)) 
            title(sprintf("Day %d", i))
        end
        for k=1:length(names)
            out=~rem(i-1, size(correctRTArray,1))*((i-1)/size(correctRTArray,1));
            if i == 1 | out ~= 0
                ylabel(names{k}, Interpreter="none", FontSize=16)
            end
        end
        subtitle(sprintf("Count = %d", length(correctRTArray{i})));
    end
    sgtitle("Correct Trials RT");
    linkaxes;


    % Line graph of RTs
    figure;
    plot(correctRTMeans, '--o');
    title("Days vs Mean RT Time for All Correct Trials");
    xlabel("Days");
    legend(names, Interpreter="none");

end