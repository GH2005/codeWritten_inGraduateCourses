clear; close;

% prepare data
load('train.data');
xtrain = double(logical(spconvert(train)));
load('train.label'); ytrain = train;
load('test.data');
xtest = double(logical(spconvert(test)));
load('test.label'); ytest = test;

% train a Naive Bayes model
nbModel = fitcnb(xtrain, ytrain);

% predict and test
yhat = predict(nbModel, xtest);
fprintf('Accuracy = %.4f', mean(yhat == ytest));