function yfit = predfunLDA(xtrain, ytrain, xtest)
    ytrain = double(ytrain) - 1;
    
    y = ytrain >.5;                                   % logical flags
    phi1=mean(y);  phi2=mean(~y);   % prior
    x1 = xtrain(y, :); x2 = xtrain(~y, :); % feature vectors of two classes
    mu1= mean(x1);  mu2= mean(x2); % means of two classes
    
    c = .5*(cov(x1) + cov(x2));               % the single covariance matrix
    w = inv(c) * (mu1-mu2)';
    a = log(phi2/phi1) - .5*((mu2-mu1)*c^-1*(mu2+mu1)');
    
    yfit = nominal(double(xtest * w > a));
end