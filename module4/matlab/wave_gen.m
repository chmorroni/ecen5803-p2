% parameters
F = 1004;
Fs = 192000;
Ts = 1/Fs;
num_periods = 251;
len = Fs / F * num_periods;

% generate wave
t = [0:len] * Ts;
wave = 1.65 * sin(2 * pi * F * t) + 1.65;

% output to file
csvwrite("waveform.csv", transpose(wave));

% generate C array for lookup table
array = num2str(wave, '%1.6ff, %1.6ff, %1.6ff, %1.6ff, %1.6ff, %1.6ff, %1.6ff, %1.6ff, %1.6ff, %1.6ff,\n');
join(['{' array '}'])
len