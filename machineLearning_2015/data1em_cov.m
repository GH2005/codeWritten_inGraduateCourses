% Yang Wang - EM unsupervised training
% using all 4 features; using shared covariance

% preparation
clear; close;
load('data1.mat');
y = logical(data(:, 5)); % for testing only
cIt = 5000; % count of iterations

% arbitrarily assigned labels: fixed
yhat = logical([zeros(50, 1); zeros(50, 1) + 1]);

% training
misRate = zeros(cIt, 1); % for plotting
for i = 1:cIt
    % maximization
    phi0 = mean(yhat); phi1 = mean(~yhat);
    x0 = data(yhat, 1:4); x1 = data(~yhat, 1:4);
    mu0 = mean(x0); mu1 = mean(x1);
    c = 0.5 * (cov(x0) + cov(x1));
    w = inv(c) * (mu0 - mu1)';
    a = log(phi1/phi0) ...
        - 0.5 .* ( (mu1-mu0) * inv(c) * (mu1+mu0)' );
    
    % expectation
    yhat = data(:, 1:4) * w > a;
    misRate(i, 1) = mean(y~=yhat);
end

% plot the learning curve
plot( (1:cIt)', misRate );
title('Learning Curve');
xlabel('Iteration'); ylabel('Misclassification Rate');