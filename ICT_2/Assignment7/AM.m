% MATLAB script for 
% Demonstration script for envelope detection. The message signal

clear
echo on
t0=.3;                                	% signal duration
ts=0.001;                              	% sampling interval
fc=200;                                	% carrier frequency
fm=10
a=0.25;                                	% modulation index
fs=1/ts;                               	% sampling frequency
t=[0:ts:t0];                           	% time vector
df=0.25;                               	% required frequency resolution
% message signal
m=cos(2*pi*fm.*t);
c=cos(2*pi*fc.*t);                     	% carrier signal
m_n=m/max(abs(m));                     	% normalized message signal
[M,m,df1]=fftseq(m,ts,df);             	% Fourier transform 
f=[0:df1:df1*(length(m)-1)]-fs/2;      	% frequency vector
u=(1+a*m_n).*c;                        	% modulated signal
[U,u,df1]=fftseq(u,ts,df);             	% Fourier transform 
env=env_phas(u);                       	% Find the envelope. 
dem1=(env-1)/a;                      	% Remove dc and rescale.
pause  % Press any key to see a plot of the message.
subplot(2,1,1)
plot(t,m(1:length(t)))
axis([0 0.3 -2.1 2.1])
xlabel('Time')
title('The message signal')
pause  % Press any key to see a plot of the modulated signal.
subplot(2,1,2)
plot(t,u(1:length(t)))
axis([0 0.3 -2.1 2.1])
xlabel('Time')
title('The modulated signal')
pause  % Press a key to see the envelope of the modulated signal.
clf
subplot(2,1,1)
plot(t,u(1:length(t)))
axis([0 0.3 -2.1 2.1])
xlabel('Time')
title('The modulated signal')
subplot(2,1,2)
plot(t,env(1:length(t)))
xlabel('Time')
title('Envelope of the modulated signal')
pause  % Press a key to compare the message and the demodulated signal.
clf
subplot(2,1,1)
plot(t,m(1:length(t)))
axis([0 0.3 -2.1 2.1])
xlabel('Time')
title('The message signal')
subplot(2,1,2)
plot(t,dem1(1:length(t)))
axis([0 0.3 -2.1 2.1])
xlabel('Time')
title('The demodulated signal')
