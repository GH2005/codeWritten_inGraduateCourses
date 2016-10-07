simulation of processing of access control list at a router interface. 
	Structure of files: my code in C++ to this question was accomplished using Visual Studio 2015 Community, and the Visual Studio Solution is in directory code_a2q2. The actual code file is code_a2q2\code_a2q2\code_nwsec_a2q2.cpp. A build is code_a2q2\Debug\code_nwsec_a2q2.exe. If the program is started from Visual Studio IDE, the sample input files to be used are code_a2q2\code_a2q2\acl_collection.txt and code_a2q2\code_a2q2\packets.txt; or if the build is launched, the two files are already placed with it to support its execution.
	Format of input files
	A file that describes ACLs: (sample file acl_collection.txt)
To describe an ACL, first write a line containing the keyword “ACL” and then the sequence number for it, starting from 0. In each ACL, there can be multiple rules starting with “access-list”. The format for writing each rule can only be:
Standard ACL: access-list [number] (permit|deny) ([srcip mask] | any)
Or
Extended ACL: access-list [number] (permit|deny) (ip|tcp|udp|icmp) ([srcip mask] | any) (eq [src_port])? ([dstip mask] | any) (eq [dst_port])?
where (x|y) means either x or y, and (x)? means there can be zero or one x. Each ACL must be ended with only one set of “interface” and “ip access-group” commands. Finally, after all ACLs have been added, there must be a keyword “END” to mark the end of the file.
	A file that contains groups of test packets for each ACL mentioned in the file above: (sample file packets.txt)
Each ACL must be supplied with a group of packets used to test it. The file must be organized in the way corresponding to the file above. Each group is started by the keyword “PACKET_GROUP” followed by its sequence number, and is ended by “END_GROUP”. Between the two keywords, each packet is described using the following format:
[srcip] [dstip] (tcp|udp|icmp) [src_port] [dst_port]
Note that for icmp packets, src_port and dst_port must be -1 to indicate they are unspecified.
	The algorithm found by me to decide if an IP address matches another given a wildcard mask:
Both the two IP addresses perform a bit-by-bit OR operation with the wildcard mask, and this produces two results. If the two results are exactly equal, they match. Otherwise, they do not.

