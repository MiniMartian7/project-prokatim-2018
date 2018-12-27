syms y(x) %symbolic declaration
dy=diff(y); % using our declaration, we define our first and second order "y"s
dy2=diff(y,2);
omega=6; % omega is 2*pi*f0/fs, and should therefore not exceed the value pi
ode=y == x + 2*cos(omega)*dy-dy2 ; %expression of our function. note that "ode" is also a symbolic function
vars = y(x);
V = odeToVectorField(ode); %conversion of our equation to a system of first order diff equations
M = matlabFunction(V,'vars', {'x','Y'}); %M is our new function handle (from the symbolic expresison)
interval = [0 100];  %time interval
y0 = [0 0];      %initial conditions
ySol = ode45(M,interval,y0); %ode45 is a function used for solving differential equations
%its description states that it is very flexible, and intended primarily
%for nonstiff diff eq, whatever the fuck those are
%it is apparently reccomended to use this function as a first attempt for
%any diff eq we need to solve
tValues = linspace(interval(1),interval(2),1000); %generates linearly spaced vectors
yValues = deval(ySol,tValues,1); %number 1 denotes first solution 
%https://www.mathworks.com/help/matlab/ref/deval.html - used to evaluate a
%solution
plot(tValues,yValues)
hold on
yValues = deval(ySol,tValues,2);
plot(tValues,yValues, 'r')
