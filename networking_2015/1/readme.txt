Packet transmission - MATLAB code.
Parameters: total message length, per packet size, multiple hops, bandwidth of links, delay.

	Programing – Simple Simulation
1)	The code is accomplished in MATLAB;
2)	p2.m: function [time3D, cPkt3D] = p2( B, M, P, needPrint )
o	It simulates the procedure of packet arrivals;
o	Parameter B: horizontal vector containing Bandwidths in Mbps;
o	Parameter M: horizontal vector containing Message sizes in MB;
o	Parameter P: horizontal vector containing Packet sizes in MB;
o	Parameter needPrint: assign 1 to display the procedure, otherwise assign 0;
o	Returning time3D: total delay times of all combinations of B, M, P elements;
o	Returning cPkt3D: packet counts of all combinations of B, M, P elements;
o	If the size of last packet in a message is less than the corresponding element in P, it is extended to be.

function [time3D, cPkt3D] = p2( B, M, P, needPrint )
% p2, simulate packet arrivals, and display the procedure if needed.
%
% B, M, P should be 1*n vectors; B -> bandwidths(Mbps), M -> message
% sizes(MB), P -> packet sizes(MB), needPrint -> 1 for displaying procedure;
%
% if the size of last packet in a message is less than the corresponding 
% element of P, it is extended to be.
if needPrint;
    disp('Simulating packet arrivals at routers and D:');
    disp('Packets are indexed from 0, minus numbers indicate no arrival;');
    disp('R1, R2 and R3 fields in the last line are invalid;');
end
time3D = zeros(size(M, 2), size(P, 2), size(B, 2));
cPkt3D = time3D;

for ib = 1: size(B, 2);
    timePlane = (1: size(M, 2))' * (1: size(P, 2));
    pktPlane = timePlane;
    
    for im = 1: size(M, 2);
        for ip = 1: size(P, 2);
            if needPrint;
                disp([char(10), 'Bandwidth = ', num2str(B(1, ib)), 'Mbps; Message Size = ' ...
                    num2str(M(1, im)), 'MB; Packet Size = ', num2str(P(1, ip)), 'MB']);
                fprintf('%13s%5s%5s%5s%5s\n', 'PassedTime(s)', 'R1', 'R2', 'R3', 'D');
            end
            
            cPackets = ceil( M(im) ./ P(ip) );
            period = P(ip) .* 8 ./ B(ib);
            
            time = -period; R1 = -2; R2 = -3; R3 = -4;
            for D = -4: (cPackets - 1);
                R1 = R1 + 1; R2 = R2 + 1; R3 = R3 + 1;
                time = time + period;
                if needPrint; fprintf('%13.3f%5i%5i%5i%5i\n', time, R1, R2, R3, D); end
            end
            
            timePlane(im, ip) = time;
            pktPlane(im, ip) = cPackets;
        end
    end
    
    time3D(:, :, ib) = timePlane;
    cPkt3D(:, :, ib) = pktPlane;
end

3)	simulation.m: 

clear; close;
p2([20 100], [4 50], [0.1, 2, 3], 1);

A part of output:
Bandwidth = 20Mbps; Message Size = 4MB; Packet Size = 2MB
PassedTime(s)	R1   R2   R3    D
        0.000   -1   -2   -3   -4
        0.800    0   -1   -2   -3
        1.600    1    0   -1   -2
        2.400    2    1    0   -1
        3.200    3    2    1    0
        4.000    4    3    2    1

o	Elements of columns R1, R2, R3, D indicate in indices (starting from 0) of packets arriving at routers or host;
o	For programing simplicity, the first line of output are minus numbers indicating no packet arrival, and in the last line only the number under D is valid.
4)	graph_drawing.m: 

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

o	When drawing the graph of Total Delay Time vs. Message Size, packet length is fixed to 2MB;
o	When drawing the graph of Total Delay Time vs. Number of Packets, message length is fixed to 30MB.

