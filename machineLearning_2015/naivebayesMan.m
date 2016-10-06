%% Yang Wang - 20 news groups, NB
clear; close;

%% prepare data
load('train.data');
% only count whether a word exists or not
xtrain = double(logical(spconvert(train)));
load('train.label'); ytrain = train;
load('test.data');
xtest = double(logical(spconvert(test)));
load('test.label'); ytest = test;

%% Laplace Smoothing
% 61188 -> number of features in the test data
% 53975 -> number of features in the training data
xtrain = [xtrain zeros(size(xtrain, 1), 61188-53975)];
xtrain = [xtrain; zeros(20, 61188); ones(20, 61188)];
ytrain = [ytrain; (1:20)'; (1:20)'];

%% maximum likelihood estimation
phiYJ = zeros(20, 61188); % 20 labels, 61188 features
phiY = zeros(20, 1); % probability of getting each class
for label = 1:20
    filter = (ytrain == label);
    phiYJ(label, :) = mean(xtrain(filter, :));
    phiY(label, 1) = mean(filter);
end

%% prediction

% If the probability of a word's presence within a class
% is phi, then I try to count a word's absence as having
% probability (1-phi). This can run for hours.
% for ob = 1:7505
%     for lb = 1:20
%         for ft = 1:61188
%             logProb(ob, lb) = logProb(ob, lb) + ...
%                 log( (phiYJ(lb, ft).^xtest(ob, ft)) .* ...
%                 (1-phiYJ(lb, ft).^(1-xtest(ob, ft))) );
%         end
%         logProb(ob, lb) = logProb(ob, lb) + log(phiY(lb, 1));
%     end
% end

logPhiYJ = log(phiYJ);
% probability of each testing case belonging to each label
% summation of log-probabilities for present words only
logProb = xtest * logPhiYJ';

% maximum a posteriori
[noUse, yhat] = max(logProb, [], 2);
fprintf('Accuracy = %.4f\n', mean(yhat == ytest));