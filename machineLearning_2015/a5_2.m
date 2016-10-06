clear; close;

n = 4;
cloudy = 1; sprinkler = 2; rain = 3; wetgrass = 4;
dag = zeros(n, n);
dag(cloudy, [sprinkler rain]) = 1;
dag([sprinkler rain], wetgrass) = 1;

szNode = [2 2 2 2];
bnet = mk_bnet(dag, szNode);
bnet.CPD{cloudy} = tabular_CPD(bnet, cloudy, [0.6 0.4]);
bnet.CPD{sprinkler} = tabular_CPD(bnet, sprinkler, ...
    [0.5 0.9 0.5 0.1]);
bnet.CPD{rain} = tabular_CPD(bnet, rain, [0.8 0.2 0.2 0.8]);
bnet.CPD{wetgrass} = tabular_CPD(bnet, wetgrass, ...
    [1.0 0.1 0.1 0.01 0.0 0.9 0.9 0.99]);
engine = jtree_inf_engine(bnet);

evidence1 = cell(1, n);
evidence1{wetgrass} = 2;
[engine1, loglik1] = enter_evidence(engine, evidence1);
marg1 = marginal_nodes(engine1, sprinkler);
p1 = marg1.T(2)

evidence2 = cell(1, n);
evidence2([rain wetgrass]) = {2};
[engine2, loglik2] = enter_evidence(engine, evidence2);
marg2 = marginal_nodes(engine2, sprinkler);
p2 = marg2.T(2)