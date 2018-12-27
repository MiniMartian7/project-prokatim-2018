omg=3.051592653; % omega, which controls our frequency
x = zeros(100, 'double'); % vector of type double, for storing values
% initialization
y1 = 1;
y2 = 0;
i=0;
a=1;
for i = 1:1:100
y=2*cos(omg)*y1-y2;
y2=y1;
y1=y;
x(a) = y; % store each iteration of y in vector x
a=a+1; % increment pointer
end
plot (x);
title('Harmonic oscillations for IIR filter with poles on unit circle')
xlabel('y = x + 2*cos(omega)*dy-dy2')