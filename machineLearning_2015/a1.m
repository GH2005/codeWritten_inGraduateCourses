% Assignment 1, coded by Yang Wang, B00711141
cIterations = 30;

x = rand(1, 10);
y = 0.5 * x + 1 + 0.1 * randn(size(x));
% according to the code, the regression target should be y = 0.5 * x + 1.

mList = 1: 1: cIterations + 1;
bList = 1: 1: cIterations + 1;
RSEs = 1: 1: cIterations + 1;
m = 0; b = 0;

for i = 1: cIterations + 1
    mList(1, i) = m;
    bList(1, i) = b;
    yr = m .* x + b; ye = mean(y);
    rse = sum((y - yr) .^ 2) ./ sum((y - ye) .^ 2);
    % I can't figure out what exactly a "relative mean square error is via
    % Google. Here I intend to compute a "relative square error" instead of
    % a "mean square error".
    RSEs(1, i) = rse;
    
    m = m + 0.1 .* sum((y - m .* x - b) .* x);
    b = b + 0.1 .* sum(y - m .* x - b);
end

figure(1);
plot(x, y, '*', x, m .* x + b);
title('Data Points & Final Regression Line');
xlabel('x'); ylabel('y');
legend('Data Points', 'Final Regression Line');

figure(2);
vsed = 0: 1: cIterations;
plot(vsed, RSEs, '-', vsed, mList, '--', vsed, bList, '-.');
title('Evolution of Relative Square Error & m & b');
xlabel('Iteration'); ylabel('Value');
legend('RSE', 'm', 'b');
axis([0 30 0 2]);