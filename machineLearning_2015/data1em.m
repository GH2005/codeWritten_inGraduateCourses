%% Yang Wang - EM unsupervised training
%% using all 4 features; no shared covariance matrix

%% preparation
clear; close;
load('data1.mat');
y = logical(data(:, 5)); % for testing only
cIt = 5000; % count of iterations

%% arbitrarily assigned labels: randomly initialized
yhat = rand(100, 1) > 0.5;

%% training
misRate = zeros(cIt, 1); % for plotting
for i = 1:cIt
    % maximization
    x = data(:, 1:4);
    x0 = data(yhat, 1:4); x1 = data(~yhat, 1:4);
    mu0 = mean(x0); mu1 = mean(x1);
    phi0 = mean(yhat); phi1 = mean(~yhat);
    
    % expectation
    % the following matrix operations are results of my trying
    % to use formula 1.39 in  Classification2.pdf and trying to
    % avoid any additional use of loops.
    c0inv = inv(cov(x0)); c1inv = inv(cov(x1));
    mu0p = mu0(ones(100, 1), :);
    mu1p = mu1(ones(100, 1), :);
    likelihood0 = -0.5 .* (x * c0inv * x' + mu0p * c0inv * mu0p')...
        + x * c0inv * mu0p' + log(phi0);
    likelihood1 = -0.5 .* (x * c1inv * x' + mu1p * c1inv * mu1p')...
        + x * c1inv * mu1p' + log(phi1);
    tmp = diag(zeros(1, 100) + 1);
    yhat = (likelihood0 - likelihood1) * tmp * (zeros(100, 1) + 1)...
        > 0;
    misRate(i, 1) = mean(yhat ~= y);
end

%% plot the learning curve
plot( (1:cIt)', misRate );
title('Learning Curve');
xlabel('Iteration'); ylabel('Misclassification Rate');