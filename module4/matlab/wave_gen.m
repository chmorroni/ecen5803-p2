F = 1004;
Fs = 20000;
Ts = 1/Fs;
num_periods = 251;
%num_periods = 1;
len = Fs / F * num_periods;

t = [0:len] * Ts;
wave = 1.65 * sin(2 * pi * F * t) + 1.65;

array = num2str(wave, '%1.6ff, %1.6ff, %1.6ff, %1.6ff, %1.6ff, %1.6ff, %1.6ff, %1.6ff, %1.6ff, %1.6ff,\n');
join(['{' array '}'])