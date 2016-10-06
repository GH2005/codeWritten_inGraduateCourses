clear; close;

cIt = 10000;
x = [0 0 1;
       0 1 1;
       1 0 1;
       1 1 1]; %every row is an observation.
y = [0; 1; 1; 0];
wh = rand(4, 3); %the hidden layer has 4 nodes.
wo = rand(1, 4); %one node output.
mse = zeros(cIt, 1);

for i=1: cIt
    h = 1 ./ (1 + exp(- x * wh'));
    yo = 1 ./ (1 + exp(- h * wo'));
    mse(i, 1) = mean((y - yo) .^ 2);
    
    do = yo .* (1 - yo) .* (y - yo);
    dh = h .* (1 - h) .* (do * wo);
    wo = wo + 0.1 .* (do' * h);
    wh = wh + 0.1 .* (dh' * x);
end

rmse = mse ./ max(mse);
plot((1: cIt)', rmse);
title('Learning Curve');
xlabel('Iteration'); ylabel('Relative Mean Square Error');