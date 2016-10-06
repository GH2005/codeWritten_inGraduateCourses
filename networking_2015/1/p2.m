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

