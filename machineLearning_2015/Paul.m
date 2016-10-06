setup;
x = im2single(imread('peppers.png')); y=1;
wh1 = randn(5,5, 3,10,'single');
wh2 = randn(3,3,10,20,'single');
nh2 = ((size(x,1)-5+1)/2-3+1)/2 * ...
((size(x,2)-5+1)/2-3+1)/2 * size(wh2,4);
wo = randn(1,nh2,'single');
h1in= vl_nnconv(x, wh1, []);
h1p = vl_nnpool(h1in, 2, 'stride',2);
h1 = vl_nnrelu(h1p);
h2in= vl_nnconv(h1, wh2, []);
h2p = vl_nnpool(h2in, 2, 'stride',2);
h2 = vl_nnrelu(h2p);
h2v = reshape(h2, [nh2 1]);
oin = wo * h2v;
o = 1./(1+exp(-oin));
L = y - o;
do = L .* o .* (1-o);
dwo = do * h2v';
dh2 = wo' * do;
dh2 = reshape(dh2, size(h2));
dh2 = vl_nnrelu(h2p,dh2);
dh2 = vl_nnpool(h2in, 2, dh2, 'stride',2);
[dh1,dwh2] = vl_nnconv(h1,wh2,[],dh2);
dh1 = vl_nnrelu(h1p,dh1);
dh1 = vl_nnpool(h1in, 2, dh1, 'stride',2);
[non,dwh1] = vl_nnconv(x, wh1, [], dh1);