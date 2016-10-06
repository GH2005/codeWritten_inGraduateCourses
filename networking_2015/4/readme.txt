1.	Bridge Simulation
•	The code is achieved with Visual C++ 2015. The “Visual Studio Solution” folder “a3_q1” is included in this archive.
•	When starting this program from Visual Studio IDE in debug mode, the working directory of it is \a3_q1\a3_q1; if the program is started by running \a3_q1\Debug\a3_q1.exe, then the working directory should be \a3z_q1\Debug. An input file required by the instruction of this problem should be placed in this working directory where a sample file (snapshot.txt) has been put.
•	The format of an input file for this program is defined below:
The first line contains three elements A, B and C. A denotes that port numbers of this bridge are integers ranging from 0 to A; B denotes that a host name is a capital letter ranging from ‘A’ to B, so that if B is ‘G’, possible host names are ‘A’, ‘B’, ‘C’, ‘D’, ‘E’, ‘F’, ‘G’; C denotes that the initial forwarding database contains C entries. 
The following C lines are all entries in the initial forwarding database.
•	Once the program is started, its console UI will guide users.
•	Though not mentioned, a port number of a bridge is necessary to be included when generating a random frame.
•	Because the port number and the host name are both randomly and independently generated, the case “No FDB update” can be rare.

