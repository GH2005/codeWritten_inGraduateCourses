function yfit = predfunMLP(xtrain, ytrain, xtest)
    ytrain = double(ytrain) - 1;
    xtrain = [xtrain 1+zeros(size(xtrain, 1), 1)];
    xtest = [xtest 1+zeros(size(xtest, 1), 1)];
    wh = rand(4, size(xtrain, 2));
    wo = rand(1, 4);

    for i=1: 10000
        h = 1 ./ (1 + exp(- xtrain * wh'));
        yo = 1 ./ (1 + exp(- h * wo'));

        do = yo .* (1 - yo) .* (ytrain - yo);
        dh = h .* (1 - h) .* (do * wo);
        wo = wo + 0.1 .* (do' * h);
        wh = wh + 0.1 .* (dh' * xtrain);
    end
    
    hfit = 1 ./ (1 + exp(- xtest * wh'));
    yfit = double(1 ./ (1 + exp(- hfit * wo')) > 0.5);
    yfit = nominal(yfit);
end

