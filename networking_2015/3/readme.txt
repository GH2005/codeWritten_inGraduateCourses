•	Coding platform: Visual Studio 2015 community, Windows X64; function _itoa may be incompatible with other compilers;
•	Code structure: in this archive file, p3\ is the directory of the “Visual Studio solution” to this problem; there are 4 “project”s in this solution, which are “sender”, “receiver”, “CRC32_original_MSG_generator”, and “CRC32_RDM_error_generator”, and each of them generate a binary executable in p3\Debug\; input and output files are also required to be placed in p3\Debug\;
•	Before running any of the “project”s, it is necessary to modify the input-file name macro definition to make sure the program can actually read the input file;
•	Here is a demonstration required by question 3.3: 
o	p3\Debug\demo.txt is an input for sender.exe; in demo.txt, the first line is an integer N indicating the number of cases, followed by N lines; each line specifies an original message and the generator polynomial it uses; the content of demo.txt is
1
1010010010101100101011100       110011
o	the sender.exe will output demo_transmitted.txt as an input file of receiver.exe; in demo_transmitted.txt, the first line is an integer N indicating the number of cases, followed by N lines; each line specifies a sent bit string that is appended with error check bits, and the generator polynomial it uses; the content of demo_transmitted.txt is
1
101001001010110010101110000011       110011
o	the receiver.exe reads demo_transmitted.txt and performs error check; it output demo_checked.txt to show the remainder it gets in every case and the count of erroneous cases; the content of demo_checked.txt is
00000
0
Therefore, no error is found;
o	if an error is added manually into demo_transmitted.txt and a new file demo_err_transmitted.txt is created; the content is
1
101001000111101011001110000011       110011
Then the receiver.exe can find this error by output demo_err_checked.txt:
00111
1
•	CRC32_original_MSG_generator.exe and CRC32_RDM_error_generator.exe is used to stochastically generate 1520-byte-long original messages and < 32 bit, == 32 bit, > 32 bit burst errors; the latter one is intended to accept an output of sender.exe as an input.
Burst error length	Number of frames	Number of frames in which error
was detected
< 32 bits	320	312
= 32 bits	330	330
>32 bits	350	350

An explanation about why in my case 8 frames with an error < 32 bits are undetected: I designed the CRC32_RDM_error_generator to generate an error of random length from 1 bit to 31 bits. Thus, an error of small length runs the risk of being all zeros that is not an error actually.

