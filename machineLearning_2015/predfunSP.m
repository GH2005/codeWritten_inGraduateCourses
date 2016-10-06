function yfit = predfunSP(xtrain, ytrain, xtest)
    ytrain = double(ytrain) - 1;
    xtrain = [xtrain 1+zeros(size(xtrain, 1), 1)];
    xtest = [xtest 1+zeros(size(xtest, 1), 1)];
    w = rand(1, size(xtrain, 2));
    for i = 1:10000
        yout = 1 ./ (1 + exp(- xtrain * w'));
        w = w + 0.1 .* ( yout .* (1 - yout) .* (ytrain - yout) )' * xtrain; 
    end
    yfit = double(1 ./ (1 + exp(- xtest * w')) > 0.5);
    yfit = nominal(yfit);
end