clear; clc;

%% ---------------------------------------------------------------
%  1. LOAD RAW IR DATA FROM CSV
%% ---------------------------------------------------------------
disp("Loading PPG data...");
ir = readmatrix("C:\Users\ashutosh sriram\OneDrive\Desktop\ppg_raw.csv");


fs = 100;                            % Sampling frequency (Hz)
N = length(ir);
t = (0:N-1)/fs;

%% ---------------------------------------------------------------
%  2. DETREND (removes slow drift, finger pressure drift)
%% ---------------------------------------------------------------
ir_detrend = detrend(ir);

%% ---------------------------------------------------------------
%  3. BANDPASS FILTER (0.5–5 Hz → heartband)
%% ---------------------------------------------------------------
disp("Filtering...");
[b, a] = butter(3, [0.5 5] / (fs/2), 'bandpass');
ir_filt = filtfilt(b, a, ir_detrend);

%% ---------------------------------------------------------------
%  4. NORMALIZATION (for easier viewing)
%% ---------------------------------------------------------------
ir_norm = (ir_filt - min(ir_filt)) / (max(ir_filt) - min(ir_filt));

%% ---------------------------------------------------------------
%  5. PEAK DETECTION (heartbeats)
%% ---------------------------------------------------------------
[pks, locs] = findpeaks(ir_norm, 'MinPeakDistance', 0.4 * fs);

% Heart Rate
if length(locs) > 1
    HR = 60 / mean(diff(locs) / fs);
else
    HR = NaN;
end

%% ---------------------------------------------------------------
%  6. FEATURE EXTRACTION (Core for glucose trend)
%% ---------------------------------------------------------------
AC = mean(pks);        % Pulsatile component
DC = mean(ir);         % Baseline intensity
ratio = AC / DC;       % Glucose-sensitive feature

fprintf("\n=================== FEATURES ===================\n");
fprintf("Heart Rate       : %.2f bpm\n", HR);
fprintf("AC amplitude     : %.6f\n", AC);
fprintf("DC level         : %.2f\n", DC);
fprintf("AC/DC ratio      : %.8f\n", ratio);
fprintf("=================================================\n");

%% ---------------------------------------------------------------
%  7. USER FASTING REFERENCE (SET THIS MANUALLY)
%% ---------------------------------------------------------------
% Replace with your own fasting AC/DC ratio from a fasting scan:
fasting_ratio = 0.011077;     % <----- PUT YOUR OWN FASTING AC/DC HERE

%% ---------------------------------------------------------------
%  8. GLUCOSE TREND PREDICTION (NO CALIBRATION)
%% ---------------------------------------------------------------
change_percent = ((ratio-fasting_ratio) / fasting_ratio) * 100;

fprintf("\n=============== GLUCOSE TREND RESULT ===============\n");

if change_percent > 10
    disp("GLUCOSE STATUS : ↑ Increased (Post-meal spike likely)");
elseif change_percent < -10
    disp("GLUCOSE STATUS : ↓ Decreased (Lower than fasting)");
else
    disp("GLUCOSE STATUS : → Stable (No major change)");
end

fprintf("Relative change vs fasting : %.2f %%\n", change_percent);
fprintf("=====================================================\n\n");

%% ---------------------------------------------------------------
%  9. PLOTTING
%% ---------------------------------------------------------------
figure;
plot(t, ir_norm, 'LineWidth', 1.2); hold on;
plot(t(locs), pks, 'ro', 'MarkerFaceColor', 'r');
grid on;

title("Filtered IR PPG Signal (MAX3010x)");
xlabel("Time (s)");
ylabel("Normalized Amplitude");

legend("Filtered PPG", "Detected Peaks");
