cd d:\ku\Docs\Docs\ML\A4\practical-cnn-2015a\practical-cnn-2015a
clear; close; setup;

% hyperparameters.
cEPOCH = 5;
learningRATE = 0.001;
ch2h = ( ( ((28 - 5 + 1) / 2 - 3 + 1) / 2 ) .^ 2 ) * 100;

% prepare before epochs.
load('data\mnist-baseline\imdb.mat');
images.data = images.data ./ 255;
wh1 = randn(5, 5, 1, 100, 'single') ./ sqrt(5.*5.*100);
wh2 = randn(3, 3, 100, 100, 'single') ./ sqrt(3.*3.*100);
wo = randn(10, ch2h, 'single') ./ sqrt(ch2h);
misRATE = zeros(cEPOCH, 1);

% epochs.
for ie = 1:cEPOCH
    fprintf('epoch #%d: ', ie);
    tic;
    % training.
    for it = 1:100:60000
        y = zeros(100, 10);
        for iti = 1:100
            y(iti, images.labels(1, it + iti - 1)) = 1;
        end
        x = images.data(:, :, :, it:(it+100-1));
        
        h1c = vl_nnconv(x, wh1, []);
        h1p = vl_nnpool(h1c, 2, 'stride', 2);
        h1 = vl_nnrelu(h1p);
        h2c = vl_nnconv(h1, wh2, []);
        h2p = vl_nnpool(h2c, 2, 'stride', 2);
        h2 = vl_nnrelu(h2p);
        h2h = reshape(h2, [ch2h 100])';
        o = 1 ./ (1 + exp(- h2h * wo'));
        
        do = o .* (1-o) .* (y-o);
        dwo = do' * h2h;
        dh2h = do * wo;
        dh2 = reshape(dh2h', size(h2));
        dh2p = vl_nnrelu(h2p, dh2);
        dh2c = vl_nnpool(h2c, 2, dh2p, 'stride', 2);
        [dh1, dwh2] = vl_nnconv(h1, wh2, [], dh2c);
        dh1p = vl_nnrelu(h1p, dh1);
        dh1c = vl_nnpool(h1c, 2, dh1p, 'stride', 2);
        [dx, dwh1] = vl_nnconv(x, wh1, [], dh1c);
        
        wh1 = wh1+ learningRATE .* dwh1;
        wh2 = wh2 + learningRATE .* dwh2;
        wo = wo + learningRATE .* dwo;
    end
    
    % validation.
    for iv = 60001:100:70000
        y = zeros(100, 10);
        for ivi = 1:100
            y(ivi, images.labels(1, iv + ivi - 1)) = 1;
        end
        x = images.data(:, :, :, iv:(iv+100-1));
        
        h1c = vl_nnconv(x, wh1, []);
        h1p = vl_nnpool(h1c, 2, 'stride', 2);
        h1 = vl_nnrelu(h1p);
        h2c = vl_nnconv(h1, wh2, []);
        h2p = vl_nnpool(h2c, 2, 'stride', 2);
        h2 = vl_nnrelu(h2p);
        h2h = reshape(h2, [ch2h 100])';
        o = 1 ./ (1 + exp(- h2h * wo'));
        
        [M, I] = max((o'));
        misRATE(ie) = misRATE(ie) + sum(I ~= images.labels(1, iv:(iv+100-1)));
    end
    
    misRATE(ie) = misRATE(ie) ./ 10000;
    fprintf(' misclassification rate == %f; ', misRATE(ie));
    toc;
end

figure(1);
plot((1:cEPOCH)', misRATE);
title('Learning Curve');
xlabel('Training Epoch'); ylabel('Misclassification Rate');