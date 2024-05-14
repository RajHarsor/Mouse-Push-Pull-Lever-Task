function [correctRTArray, correctRTMeans, correctPushRTArray, correctPushRTMeans, correctPullRTArray, correctPullRTMeans] = histRTCorrectTrials(multi_behavior_outfile, names, type)
    switch (type)
        case "all"
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
    
           correctRTMeans = cell2mat(correctRTMeans);
    
            % Line graph of RTs
            figure;
            plot(correctRTMeans, '--o');
            title("Days vs Mean RT Time for All Correct Trials");
            xlabel("Days");
            ylabel("Reaction Time (ms)")
            legend(names, Interpreter="none");
       
        case "push"
            correctPushRTArray = {};
            for i = 1:length(names) % Iterating through the mouse structs
                for j = 1:length(multi_behavior_outfile.(names{i}).trials_tables) %Iterating through trials_tables
                    currTable = multi_behavior_outfile.(names{i}).trials_tables{j, 2};
                    
                    currentCorrectRTArray = [];
                    for k = 1:length(currTable.("Reaction Time")) %Iterating through
                        if currTable.("Push/Pull/Timeout")(k) == "Push" && currTable.("Correct/Incorrect")(k) == "Correct"
                           currentCorrectRTArray(end+1) = currTable.("Reaction Time")(k);
                        end 
                    end
                    correctPushRTArray{j, i} = currentCorrectRTArray';
                end
            end

            correctPushRTMeans = {};
            for i = 1:size(correctPushRTArray, 1)
                for j = 1:size(correctPushRTArray, 2)
                    correctPushRTMeans{i,j} = mean(correctPushRTArray{i,j});
                end
            end

            correctPushRTMeans = cell2mat(correctPushRTMeans);
            
            % Histogram
            figure;
            for i = 1:size(correctPushRTArray, 1) * size(correctPushRTArray, 2)
              % Check if current element is a 0x0 double
              if ~isempty(correctPushRTArray{i})
                subplot(size(correctPushRTArray, 2), size(correctPushRTArray, 1), i)
                histfit(correctPushRTArray{i}, 20, 'kernel');
                hold on;
                xline(correctPushRTMeans(i), Label={round(correctPushRTMeans(i))});
                xlim([0 2000]);
              else
                % Create a blank subplot for empty arrays
                subplot(size(correctPushRTArray, 2), size(correctPushRTArray, 1), i);
                axis off;  % Turn off axes for empty subplot
              end
            
              if i <= (size(correctPushRTArray, 1))
                title(sprintf("Day %d", i))
              end
            
              for k=1:length(names)
                out=~rem(i-1, size(correctPushRTArray,1))*((i-1)/size(correctPushRTArray,1));
                if i == 1 | out ~= 0
                  ylabel(names{k}, Interpreter="none", FontSize=16)
                end
              end
            
              subtitle(sprintf("Count = %d", length(correctPushRTArray{i})));
            end
            
            sgtitle("Correct Push Trials RT")
            linkaxes;

            %Line plot of means
            figure;
            plot(correctPushRTMeans, '--o');
            title("Days vs Mean RT Time for Push Correct Trials");
            xlabel("Days");
            ylabel("Reaction Time (ms)")
            legend(names, Interpreter="none");

        case "pull"
            correctPullRTArray = {};
            for i = 1:length(names) % Iterating through the mouse structs
                for j = 1:length(multi_behavior_outfile.(names{i}).trials_tables) %Iterating through trials_tables
                    currTable = multi_behavior_outfile.(names{i}).trials_tables{j, 2};
                    
                    currentCorrectRTArray = [];
                    for k = 1:length(currTable.("Reaction Time")) %Iterating through
                        if currTable.("Push/Pull/Timeout")(k) == "Pull" && currTable.("Correct/Incorrect")(k) == "Correct"
                           currentCorrectRTArray(end+1) = currTable.("Reaction Time")(k);
                        end 
                    end
                    correctPullRTArray{j, i} = currentCorrectRTArray';
                end
            end
            
            correctPullRTMeans = {};
            for i = 1:size(correctPullRTArray, 1)
                for j = 1:size(correctPullRTArray, 2)
                    correctPullRTMeans{i,j} = mean(correctPullRTArray{i,j})
                end
            end
            
            correctPullRTMeans = cell2mat(correctPullRTMeans);
            
            figure;
            for i = 1:size(correctPullRTArray, 1) * size(correctPullRTArray, 2)
              % Check if current element is a 0x0 double
              if ~isempty(correctPullRTArray{i})
                subplot(size(correctPullRTArray, 2), size(correctPullRTArray, 1), i)
                histfit(correctPullRTArray{i}, 20, 'kernel');
                hold on;
                xline(correctPullRTMeans(i), Label={round(correctPullRTMeans(i))});
                xlim([0 2000]);
              else
                % Create a blank subplot for empty arrays
                subplot(size(correctPullRTArray, 2), size(correctPullRTArray, 1), i);
                axis off;  % Turn off axes for empty subplot
              end
            
              if i <= (size(correctPullRTArray, 1))
                title(sprintf("Day %d", i))
              end
            
              for k=1:length(names)
                out=~rem(i-1, size(correctPullRTArray,1))*((i-1)/size(correctPullRTArray,1));
                if i == 1 | out ~= 0
                  ylabel(names{k}, Interpreter="none", FontSize=16)
                end
              end
            
              subtitle(sprintf("Count = %d", length(correctPullRTArray{i})));
            end
            
            sgtitle("Correct Pulls RT");
            linkaxes;

            %Line plot of means
            figure;
            plot(correctPullRTMeans, '--o');
            title("Days vs Mean RT Time for Pull Correct Trials");
            xlabel("Days");
            ylabel("Reaction Time (ms)")
            legend(names, Interpreter="none");
    end
end