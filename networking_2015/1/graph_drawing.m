clear; close;
M = 11: 1: 50;
[t1, c1] = p2(100, M, 2, 0);
P = 0.5: 0.5: 20;
[t2, c2] = p2(100, 30, P, 0);

figure(1);
plot(M, t1', '.-');
title('Total Delay Time vs. Message Size');
xlabel('Message Size(MB)'); ylabel('Total Delay Time(s)');

figure(2);
plot(c2', t2', '.-');
title('Total Delay Time vs. Number of Packets');
xlabel('Number of Packets(MB)'); ylabel('Total Delay Time(s)');