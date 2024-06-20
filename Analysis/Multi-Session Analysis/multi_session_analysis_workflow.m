%% User Inputs
rootdir = 'C:\Users\rdh92-adm\Box\Oldenburg-Shared\Behavioral Testing\Cohort 15';
waterLogPath = "C:\Users\rdh92-adm\Box\Oldenburg-Shared\Water Log.xlsx";
SheetName = "Cohort 15 - VC"; % Name of the sheet in the waterLog file

%% Makes the Outfile 
% Please note this takes a long time as there is a lot of data usually in
% the directories
warning('off');
[multi_behavior_outfile, names] = makeOutfile(rootdir, waterLogPath, SheetName);
warning('on');

%% Correct Trials Over Days (All)

[correctTrialsArray] = correctTrialsFigAll(multi_behavior_outfile, names);

%%
% Figure for Ian 240609 - Last Figure for Grant - Correct Trials - Cohort 14
phase1 = correctTrialsArray(1:7, :);
phase2 = correctTrialsArray(8:end, :);
mean_phase1 = mean(phase1, 2);
mean_phase2 = mean(phase2, 2);

ax1 = subplot(1,2,1)
plot(phase1, Color=[0.237, 0.237, 0.237, 0.3]);
hold on;
scatter([1:7], phase1, Marker=".", MarkerFaceColor=[0.237, 0.237, 0.237], MarkerEdgeColor=[0.237, 0.237, 0.237] , MarkerFaceAlpha=0.3, MarkerEdgeAlpha=0.3);
hold on;
plot(mean_phase1, Color = 'black', Marker='.', LineStyle='-');
title("Phase 1");
ylabel("Rewards/Correct Trials");

ax2 = subplot(1,2,2)
plot([8:20], phase2, Color=[0.237, 0.237, 0.237, 0.3]);
hold on;
scatter([8:20], phase2, Marker=".", MarkerFaceColor=[0.237, 0.237, 0.237], MarkerEdgeColor=[0.237, 0.237, 0.237] , MarkerFaceAlpha=0.3, MarkerEdgeAlpha=0.3);
hold on;
plot([8:20],mean_phase2, Color = 'black', Marker='.', LineStyle='-');
xticks([8:20]);
title("Phase 2");

sgtitle('Rewards Over Days (Cohort 14)');

han = axes('Visible','off');
han.XLabel.Visible = 'on';
xlabel(han, 'Days');

axis square;
linkaxes([ax1, ax2],'y');

% Figure for Ian 240610 - Last Figure for Grant - Correct Trials - Cohort 15
mean_correctTrials = mean(correctTrialsArray, 2);
figure;
plot(correctTrialsArray, Color=[0.237, 0.237, 0.237, 0.3])
hold on;
scatter(1:length(correctTrialsArray), correctTrialsArray, Marker=".", MarkerFaceColor=[0.237, 0.237, 0.237], MarkerEdgeColor=[0.237, 0.237, 0.237] , MarkerFaceAlpha=0.3, MarkerEdgeAlpha=0.3)
hold on;
plot(mean_correctTrials, Color='black', Marker='.', LineStyle = '-');
xlabel("Days");
ylabel("Correct Trials");
title('Rewards Over Days (Cohort 15)');
axis square;
%% Correct Pushes and Pulls Over Days 

[pushArray, pullArray] = pushPullBarPlot(multi_behavior_outfile, names);
%%
% Figure for Ian 240610 - Last Figure for grant - Correct Trials - Cohort 14

F_R_Ratio = pushArray ./ correctTrialsArray;
phase1 = F_R_Ratio(1:7, :);
phase2 = F_R_Ratio(8:end, :);
mean_phase1 = mean(phase1, 2);
mean_phase2 = mean(phase2, 2);

ax1 = subplot(1,2,1)
plot(phase1, Color=[0.237, 0.237, 0.237, 0.3]);
hold on;
scatter([1:7], phase1, Marker=".", MarkerFaceColor=[0.237, 0.237, 0.237], MarkerEdgeColor=[0.237, 0.237, 0.237] , MarkerFaceAlpha=0.3, MarkerEdgeAlpha=0.3);
hold on;
plot(mean_phase1, Color = 'black', Marker='.', LineStyle='-');
title("Phase 1");
ylabel("F/F+R");

ax2 = subplot(1,2,2)
plot([8:20], phase2, Color=[0.237, 0.237, 0.237, 0.3]);
hold on;
scatter([8:20], phase2, Marker=".", MarkerFaceColor=[0.237, 0.237, 0.237], MarkerEdgeColor=[0.237, 0.237, 0.237] , MarkerFaceAlpha=0.3, MarkerEdgeAlpha=0.3);
hold on;
plot([8:20],mean_phase2, Color = 'black', Marker='.', LineStyle='-');
xticks([8:20]);
title("Phase 2");

sgtitle('Push/Pull Ratio (Cohort 14)');

han = axes('Visible','off');
han.XLabel.Visible = 'on';
xlabel(han, 'Days');

axis square;
linkaxes([ax1, ax2],'y');

% Figure for Ian 240610 - Last Figure for grant - Correct Trials - Cohort 15
F_R_Ratio = pushArray ./ correctTrialsArray;
mean_F_R_Ratio = mean(F_R_Ratio, 2);

figure;
plot(F_R_Ratio,  Color=[0.237, 0.237, 0.237, 0.3]);
hold on;
scatter(1:length(F_R_Ratio), F_R_Ratio, Marker=".", MarkerFaceColor=[0.237, 0.237, 0.237], MarkerEdgeColor=[0.237, 0.237, 0.237] , MarkerFaceAlpha=0.3, MarkerEdgeAlpha=0.3);
hold on;
plot(mean_F_R_Ratio, Color = 'black', Marker='.', LineStyle='-')
ylabel("F/F+R");
xlabel('Days');
title("Push/Pull Ratio Over Days (Cohort 15)");
axis square;
%% RT Over Days

[correctRTArray, correctRTMeans] = histRTCorrectTrials(multi_behavior_outfile, names, "all");
[correctPushRTArray, correctPushRTMeans] = histRTCorrectTrials(multi_behavior_outfile, names, "push");
[correctPullRTArray, correctPullRTMeans] = histRTCorrectTrials(multi_behavior_outfile, names, "pull");
%%
% Figure for Ian 240610 - Last Figure for grant - RT - Cohort 14

phase1 = correctRTMeans(1:7, :);
phase2 = correctRTMeans(8:end, :);
mean_phase1 = mean(phase1, 2);
mean_phase2 = mean(phase2, 2);

ax1 = subplot(1,2,1)
plot(phase1, Color=[0.237, 0.237, 0.237, 0.3]);
hold on;
scatter([1:7], phase1, Marker=".", MarkerFaceColor=[0.237, 0.237, 0.237], MarkerEdgeColor=[0.237, 0.237, 0.237] , MarkerFaceAlpha=0.3, MarkerEdgeAlpha=0.3);
hold on;
plot(mean_phase1, Color = 'black', Marker='.', LineStyle='-');
title("Phase 1");
ylabel("Mean RT (Correct Trials)");

ax2 = subplot(1,2,2)
plot([8:20], phase2, Color=[0.237, 0.237, 0.237, 0.3]);
hold on;
scatter([8:20], phase2, Marker=".", MarkerFaceColor=[0.237, 0.237, 0.237], MarkerEdgeColor=[0.237, 0.237, 0.237] , MarkerFaceAlpha=0.3, MarkerEdgeAlpha=0.3);
hold on;
plot([8:20],mean_phase2, Color = 'black', Marker='.', LineStyle='-');
xticks([8:20]);
title("Phase 2");

sgtitle('Mean RT Over Days (Cohort 14)');

han = axes('Visible','off');
han.XLabel.Visible = 'on';
xlabel(han, 'Days');

axis square;
linkaxes([ax1, ax2],'y');

% Figure for Ian 240610 - Last Figure for grant - RT - Cohort 15
mean_RTs = mean(correctRTMeans, 2);
figure;
plot(correctRTMeans, Color=[0.237, 0.237, 0.237, 0.3]);
hold on;
scatter(1:length(correctRTMeans), correctRTMeans, Marker=".", MarkerFaceColor=[0.237, 0.237, 0.237], MarkerEdgeColor=[0.237, 0.237, 0.237] , MarkerFaceAlpha=0.3, MarkerEdgeAlpha=0.3);
hold on;
plot(mean_RTs, Color = 'black', Marker='.', LineStyle='-');
title("Mean RT Over Days (Cohort 15)");
ylabel("Mean RT (Correct Trials)");
xlabel("Days");
ylim([0 1250]);
axis square;
%% Trajectory Over Days

