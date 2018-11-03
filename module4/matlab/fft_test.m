F = 984.375;
Fs = 192000;
Ts = 1/Fs;
len = 4096*4;

t = [0:len] * Ts;
wave = cos(2 * pi * F * t);
fft_vals = fft(wave);
power_vals = abs(fft_vals/len);
power_spectrum = power_vals(1:len/2);
freqs = [0:len/2 - 1] * Fs / len;
%plot(freqs, power_spectrum)
%axis([900 1100 0 1])

[pxx, f] = pwelch(wave, 500, 300, 500, Fs);
plot(f, 10*log10(pxx))
axis([500 1500 -80 -20])