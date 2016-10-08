A team project in course Ubicomp. The topic was "A recorder that teaches people to play".
proj_recorder.ino is the Arduino code done by me, which can only run on an Arduino board.
Our design consisted of a real recorder, LED lights, a pair of gloves, buttons on a breadboard, ab Arduino Uno board, PuTTY application.

The prototype has three modes of use. Different modes satisfy different demands.
Mode 1: Waiting Mode
In this mode, the program waits for user input. According to a music score written in
the source code, the program will show musical notes in sequence and one by one,
ignoring note lengths. The program goes to the next note only after the user has kept
playing the current note correctly for a certain period of time. And this time period can
be changed in the source code.
Mode 2: Non-Waiting Mode
As long as the prototype starts to run in this mode, it doesn’t wait for the user’s
reaction. It will continuously show each note according to the music score, following
the rhythm specified by note lengths. It’s worth mentioning that there is a short
interval when all LED lights are turned off between two notes, because sometimes two
consecutive notes are the same. The speed at which music is performed is changeable
in the source code. And during the performance, the program keeps taking record of
user actions, so as to give a performance mark at the end. 100% is the full mark.
Mode 3: Free Mode
In this mode, the user may play the recorder freely, and the serial monitor will only
show which note the user is playing in real time.

The following is a Deployment Guide and a User Guide copied directly from our project report.

Deployment Guide
1. Components
1) Recorder x1
2) Glove a pair
3) Arduino Uno x1
4) LED light x8
5) 220-ohm resistor x8
6) Button x8
7) 10k-ohm resistor x8
8) Breadboard x1
9) Windows PC with PuTTY and Arduino IDE installed x1
10) Wire as many as needed
2. Instructions
To follow these instructions, one is required to have basic knowledge about installing
one LED light and one button on an Arduino board. Tutorials can be found in:
https://www.arduino.cc/en/Tutorial/Blink
https://www.arduino.cc/en/Tutorial/Button
The first section of the source code is commented as “Modify these before run (if you
know how)”. In the instructions, sometimes we’ll recommend the user to change some
variables in the source code, and all of them can be found in the first section.
1) Attach 8 LED lights to a pair of gloves. Their positions should follow the standard
fingering chart of the recorder.
2) Connect the LED lights with 220-ohm resistors to the Arduino Uno board. Default
mapping among fingers, recorder holes and digital pin sockets on the Arduino is
in Table 1. If the user wants to rearrange it, they may modify “const int
gc_ledsockets[8]” in the source code.
Table 1. Mapping among fingers, recorder holes and digital pin sockets
Finger Recorder Hole Digital Pin
Left Thumb 1 (the back hole) 2
Left Index Finger 2 3
Left Middle Finger 3 4
Left Ring Finger 4 5
Right Index Finger 5 6
Right Middle Finger 6 7
Right Ring Finger 7 8
Right Little Finger 8 9
3) Install 8 buttons onto the breadboard, and connect them with 10k-ohm resistors
to the Arduino Uno board. Default mapping among recorder holes, buttons and
digital pin sockets (analog pins can be used as digital ones) is in Table 2. The user
may also rearrange this mapping by modifying “const int gc_inputsockets[8]” in
the source code.
Table 2. Mapping among recorder holes, buttons and digital pin sockets
Recorder Hole Button Digital Pin
1 (the back hole) 1 (the leftmost button) 13
2 2 12
3 3 11
4 4 10
5 5 14 (analog 0)
6 6 15 (analog 1)
7 7 16 (analog 2)
8 8 17 (analog 3)
4) In Arduino IDE, compile the source code and upload the program to the Arduino
board.
5) Connect the Arduino Uno board to the Windows PC and starts PuTTY. A Window
titled “PuTTY Configuration” will pop out. Make the following configuration.
a) In “Session” page:
“Connection type” – Serial
“Serial line” – the COM port used to connect the Arduino (e.g., COM5)
“Speed” – 1000000
b) In “Window” page:
“Columns” – 40
“Rows” – 4
c) In “Window” – “Appearance” page:
“Cursor appearance” – Vertical line
“Font settings” – Courier New, 24-point
d) In “Window” – “Behavior” page:
“Window Title” – project recorder
We recommend that the user save the configuration for convenience in the future.
This can be done in the “Session” page.
6) The deployment is done. The prototype will start to work each time the user
launches PuTTY.

User Guide
Same to the Deployment Guide, instructions in this User Guide involve modifications
to the source code. Again they are all located in the section “Modify these before run
(if you know how)”.
1. Supported musical notes
This prototype supports only 8 musical notes. We think that they are enough for
playing some simple tunes. Refer to Figure 2 for details.
2. How to change the music score
The music score is represented by the array “const int gc_seq_attri[][2]” in the source
code. Each note is represented as a pair: (note code, note length). The time duration
in milliseconds of a unit-length note is defined by “#define M2_UNIT_NOTE_LEN”. For
example, if we have “#define M2_UNIT_NOTE_LEN 2000”, then pair (3, 2) means, in
the music score, there is a note that lasts 2 × 2000 = 4000 𝑚𝑠, and its note code is
3. Please always add a note of code 8 at the end of the music score.
3. How to change the duration of intervals
Sometimes two same notes are consecutive in a music score. To help the user
distinguish notes of such kind, we introduced a small interval in the rear part of each
note. During the interval, all LED lights are turned off. The duration of intervals is
defined by “#define INTERVAL_RATIO”.
For example, if we have “#define INTERVAL_RATIO 0.2” and “#define
M2_UNIT_NOTE_LEN 1000”, then the combination of LED lights for the current note
will only glow for (1 − 0.2) × 1000 𝑚𝑠, followed by an interval of 0.2 × 1000 𝑚𝑠,
and then the next note is shown.
4. How to start Mode 1 (Waiting Mode)
Figure 2. Supported musical notes in this prototype
Fingering
Note
Note Code 0 1 2 3 4 5 6 7 8
dummy
1) Modify “#define MODE” in the source code: “#define MODE 1”.
2) Modify “#define M1_FINGER_MAINTAIN” in the source code:
This value defines the time duration (in milliseconds) in which the user is required
to keep playing the current note correctly.
3) Modify “#define INTERVAL_RATIO”: it takes effect in Mode 1 too.
4) Compile it and upload the program to the Arduino board using Arduino IDE.
5) Launch PuTTY.
6) Refer to the high level description above and enjoy it.
5. How to start Mode 2 (Non-Waiting Mode)
1) Modify “#define MODE” in the source code: “#define MODE 2”.
2) Modify “#define M2_UNIT_NOTE_LEN” in the source code: explained above.
3) Modify “#define INTERVAL_RATIO”.
4) Compile it and upload the program to the Arduino board using Arduino IDE.
5) Launch PuTTY.
6) Refer to the high level description above and enjoy it.
6. How to start Mode 3 (Free Mode)
1) Modify “#define MODE” in the source code: “#define MODE 3”.
2) Compile it and upload the program to the Arduino board using Arduino IDE.
3) Launch PuTTY.
4) Refer to the high level description above and enjoy it.
