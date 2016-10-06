function yfit = predfunSVM(xtrain, ytrain, xtest)
    model = fitcsvm(xtrain, ytrain);
    yfit = predict(model, xtest);
end