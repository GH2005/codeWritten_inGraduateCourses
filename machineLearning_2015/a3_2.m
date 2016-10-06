clear; close;

cRpt = 20;
load('data1.mat');
tenFoldLoss = zeros(cRpt, 5);
for i = 1:cRpt
    tenFoldLoss(i, 1) = crossval('mcr', data(:, 1:4), data(:, 5), 'Predfun', @predfunSVM);
    tenFoldLoss(i, 2) = crossval('mcr', data(:, 1:4), data(:, 5), 'Predfun', @predfunSP);
    tenFoldLoss(i, 3) = crossval('mcr', data(:, 1:4), data(:, 5), 'Predfun', @predfunMLP);
    tenFoldLoss(i, 4) = crossval('mcr', data(:, 1:4), data(:, 5), 'Predfun', @predfunLDA);
    tenFoldLoss(i, 5) = crossval('mcr', data(:, 1:4), data(:, 5), 'Predfun', @predfunEM);
end

plot((1:cRpt)', tenFoldLoss(:, 1), 'x-', (1:cRpt)', tenFoldLoss(:, 2), 'o-', ...
    (1:cRpt)', tenFoldLoss(:, 3), 'p-', (1:cRpt)', tenFoldLoss(:, 4), '.:', ...
    (1:cRpt)', tenFoldLoss(:, 5), ':');
axis([0 cRpt+1 0.04 0.13]);
xlabel('Repetition'); ylabel('10 Fold Misclassification Rate');
legend('SVM', 'Single Perceptron', 'Multilayer Perceptron', 'LDA', ...
    'EM');