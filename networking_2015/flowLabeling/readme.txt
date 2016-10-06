I wrote this program to add application labels to the a Flowtbag flow file and to separate each of these labeled files into two parts. Each part contains TCP or UDP flows exclusively.

	Label source: http://www.iana.org/assignments/service-names-port-numbers/service-names-port-numbers.xhtml. I downloaded the CSV file available on this webpage, and made these operations on it: 1) items with port number higher than 1023 were removed; 2) all commas were replaced by string “ unknown ”that contains two blank characters and “unknown”between them. The second modification may look strange, but by this the final file, namely “0-1023-service-names-port-numbers.txt”, is ready for my code to read it.
	To tell whether a flow is a TCP or a UDP one, my code read the protocol ID (6 or 17);
	For every flow, my code choose the smaller one between source port and destination port and use it to query the port-application table that is a STL map object in my code.
	Application labels are added as one more feature.

