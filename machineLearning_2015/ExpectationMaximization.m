%% 1d example EM algorithm
 clear; hold on; x0=-10:0.1:10;
 var1=1; var2=1; mu1=-2; mu2=2;     % initialize parameters
 normal= @(x,mu,var) exp(-(x-mu).^2/(2*var))/sqrt(2*pi*var);
 while 1
%%plot distribution
    clf; hold on;
    plot(x0, normal(x0,-1,4),'k:');
    plot(x0, normal(x0,4,0.25),'k:');
    plot(x0, normal(x0,mu1,var1),'r');
    plot(x0, normal(x0,mu2,var2),'b');
    waitforbuttonpress;
%% data
    x=[2*randn(50,1)-1;0.5*randn(50,1)+4;];
%% recogintion
    c=normal(x,mu1,var1)>normal(x,mu2,var2);
%% maximization
    mu1=sum(x(c>0.5))/sum(c);
    var1=sum((x(c>0.5)-mu1).^2)/sum(c);
    mu2=sum(x(c<0.5))/(100-sum(c));
    var2=sum((x(c<0.5)-mu2).^2)/(100-sum(c));
 end