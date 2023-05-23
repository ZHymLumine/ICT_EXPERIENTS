% MATLAB script for Fourier Transform
echo on
ts=0.1;                     % set parameters
fs=1/ts;
df=0.01;
x=[ones(1,20)]; % Rectangular wave
plot(x)
[X,x,df1]=fftseq(x,ts,df);          % derive the FFT
X1=X/fs;                                    % scaling
f=[0:df1:df1*(length(x)-1)]-fs/2;           % frequency vector for FFT
f1=[-5.0:0.001:5.0];                        % frequency vector for analytic approach
y= 2*sinc(2*f1)         % exact Fourier transform
pause % Press akey to see the plot of the Fourier Transform derived analytically.
clf
subplot(2,1,1)
plot(f1,abs(y));
xlabel('Frequency')
title('Magnitude-spectrum of x(t) derived analytically')
pause % Press akey to see the plot of the Fourier Transform derived numerically.
subplot(2,1,2)
plot(f,fftshift(abs(X1)));
xlabel('Frequency')
title('Magnitude-spectrum of x(t) derived numerically')
