omega_zero = 3.051592653;%cum s-a ajuns la valoarea asta
y = zeros(100,'double');
y(2) = 1;
y(1) = 0;

for n = 3 : 1 : 100
    y(n) = 2*cos(omega_zero)*y(n-1) - y(n-2);
end
plot(y);
title('Harmonic oscillations for IIR filter with poles on unit circle')
ylabel('y = x + 2*cos(omega)*dy-dy2')