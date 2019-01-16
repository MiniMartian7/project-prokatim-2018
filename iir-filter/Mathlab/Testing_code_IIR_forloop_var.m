%omg=3.051592653 // but what happens if omega > pi?
f=100; %Hz (frequency from 100 to 10000 Hz)
omg=2*3.1415*f
x = zeros(1000, 'double'); % vector of type double, for storing values
% initialization
y1 = 1; %sin(omg) if y1=1 then the amplitude of the signal is different for 
        %each frequency. If y1 = sin(omg) the amplitude is 1 for all
        %frequency
y2 = 0;
i=0; 
a=1;
Nes=length(x); %you can modify the number of elements in vector x
%for i = 1.0:0.1:100 
for i=1:1:Nes   %if you change the number of elements in x the "for" function
                %will be the same
y=2*cos(omg)*y1-y2;
y2=y1;
y1=y;
x(a) = y; % store each iteration of y in vector x
a=a+1; % increment pointer
end
plot (x);
title('Harmonic oscillations for IIR filter with poles on unit circle')
xlabel('y = x + 2*cos(omega)*dy-dy2')