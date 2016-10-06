clear; close;

load('data1.mat');
model = fitcsvm(data(1:100, 1:4), data(1:100, 5))
cvmodel = crossval(model)
fl10 = kfoldLoss(cvmodel)