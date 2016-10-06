function yfit = predfunEM(xtrain, ytrain, xtest)
    %% Yang Wang - EM unsupervised training
    % ytrain is never used.
    cIt = 5000; % count of iterations
    normal= @(x,mu,var) exp(-(x-mu).^2/(2*var))/sqrt(2*pi*var);
    
    %% data: only the 3rd dimension
    x = xtrain(:, 3); xt = xtest(:, 3);
    
    %% arbitrarily assigned parameters: fixed
    var1=1; var2=1; mu1 = -2; mu2 = 2;
    
    %% training
    for i = 1:cIt
        % expectation
        c = normal(x, mu1, var1) > normal(x, mu2, var2);
        % maximization
        mu1=sum(x(c>0.5))/sum(c);
        var1=sum((x(c>0.5)-mu1).^2)/sum(c);
        mu2=sum(x(c<0.5))/(90-sum(c));
        var2=sum((x(c<0.5)-mu2).^2)/(90-sum(c));
    end

    %% prediction
    % I choose to use a less-than sign '<' here.
    yfit = nominal(double(...
        normal(xt, mu1, var1) < normal(xt, mu2, var2)));
end