2.	Bit Stuffing
2.1.	Coding platform: Visual C++ in Visual Studio 2015 Community, Windows x64;
the “Visual Studio solution” folder is included in this achieve, in which the code file is p2\p2\a2_q2.cpp;
the original text file is p2\p2\Gibberish.txt;
Running this program in debug mode should generate the output recovered text file in this folder;
2.2.	The sign bit of every byte is counted in the stuffed bit string;
2.3.	In the stuffed bit string, every byte carries 8 bits from original text;
2.4.	Lots of bit operations are utilized in this code, which may cause compatibility problems if other compliers or platforms treat these bit operations in different ways.

