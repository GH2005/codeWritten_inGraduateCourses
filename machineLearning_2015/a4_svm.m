cd d:\ku\docs\docs\ml\a4
clear; close;
load('mnist.mat');

tic;
xtrain_h = reshape(xtrain, [28.*28 60000])';
xtrain_h = xtrain_h ./ 255;
model = fitcecoc(xtrain_h, ytrain');
xtest_h = reshape(xtest, [28.*28 10000])';
xtest_h = xtest_h ./ 255;
yout = predict(model, xtest_h);
fprintf('%f\n', sum(yout ~= ytest') ./ 10000);
toc;