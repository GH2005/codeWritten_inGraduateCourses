clear; close;
load('data1.mat')

y = data(:,5)>.5;                                    % logical flags
phi1=mean(y);  phi2=mean(~y);      % prior
x1 = data(y,1:4); x2 = data(~y,1:4);    % feature vectors of two classes
mu1= mean(x1);  mu2= mean(x2);  % means of two classes

c = .5*(cov(x1) + cov(x2));                  % the single covariance matrix
w = inv(c) * (mu1-mu2)';                   
%a = log(phi2/phi1) - .5*(mu2*c^-1*mu2' - mu1*c^-1*mu1');
a = log(phi2/phi1) - .5*((mu2-mu1)*c^-1*(mu2+mu1)');

yhat = data(1:20,1:4) * w > a;
fprintf('Acc: %.4f\n', mean(y==yhat));

