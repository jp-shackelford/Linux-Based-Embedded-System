Lab 4 - EE 474 <br />
In this lab we implemented proximity sensors to direct the patterns of our <br />
robo-tank's driving. The motors of the tank were driven by an H-bridge chip wired <br />
to our solderless breadboard. There are four sensors on our tank: a front sensor, <br />
a bottom sensor, and a top sensor. These four optical sensors work in tandem to <br />
alert our tank to any external obstacles. <br />

The program we wrote to control its movement is MotorProgram.c. With this program, <br />
the tank will begin moving forward when it senses an object close above it. <br />
It will drive forward until it senses another object in its path and then halt. <br />
It again waits for an object to appear above it before driving back the opposite <br />
direction until it reaches a wall or some other object in its path. The tank will <br />
also stop trying to drive if it gets picked up. <br />

To compile and run the tank program, follow the commands below:
1. Compile the program with the Makefile by typing <br />
    $ make
2. Run the program with 
    $ ./motor

Alternatively, the BeagleBone Black is programmed to run the tank program on startup. <br />
Simply turn on the board and wait for the program to begin executing. This may take <br />
as long as a minute to begin.
<br />
<br />
<br />
<br />



Lab 3 - EE 474 <br />
This lab introduced the use of Loadable Kernel Modules and the interaction <br />
beteween user-space code and kernel-space code. A shift register was also implemented <br />
in order to reduce the needed number of GPIO pins for our LCD display. <br />
<br/>
The project includes the LCD kernel module that we developed, contianing several libraries <br /> 
and header files, as well as user-space code that interacts with that kernel module. <br />
The kernel module behaves as a custom dirver for the LCD screen, and allows easy access from <br />
the user-space, with simple and clean commands that we implemented. <br />
<br />
The user space code is a note playback program. It displays instructions on the LCD screen as <br />
well as user input and then plays back certain notes based upon the numbers the user inputs. <br />
For detailed instruction on how the program works, see the report found under Group6/Lab3/Lab3_Report.pdf. <br />

To run the step sequencer, follow the commands below: <br />
1. Insert the kernel module into the BeagleBone's Linux kernel with <br />
$ insmod lcd.ko <br />

2. Create the device file for the LCD by running <br />
$ dmesg <br />
   and then copying the line starting with "mknod". This command is usually <br />
$ mknod /dev/lcd c 240 0 <br />

3. Compile the step-sequencer program with <br />
$ make step <br />

4. Run the program using <br />
$ ./step <br />

5. Follow the instructions on the screen! <br />





Lab 2 - EE 474
This lab utilizes the GPIO pins of the BBB in order to drive an <br /> 
lcd display as well as test for jitter of the GPIO pins.<br /> 
<br />
This project contains a program to push text written on the console <br />
to the LCD module connected to the BeagleBone. It does this through use <br />
of a read-only pipe. Users may write up to 16 characters to the display. <br />
<br />
Also included is a simple guessing game. The computer randomly generates a <br />
number and gives the user 3 chances to guess correctly. The LCD shows the state <br />
of the game throughout play. <br />
<br /> 
<br />
To Compile run command <br />
$make lcd
<br />
There are three main stages to this lab<br /> 
1. Running oscilloscope test, run command:<br />
$./o
2. Running guessing game, run command:<br />
$./g
3. Running front and back pipes, run commands in two different terminals:<br />
TERMINAL 1 <br />
$./f<br />
TERMINAL 2 <br />
$./b<br />
For more descriptions of how these programs work, read the lab report
documentation found under Group6/Lab2/Lab2_Report.pdf <br />
Lab 1 - EE 474
This lab utilizes the beaglebone black device's PWM and GPIO pins
to count upwards to seven in binary repeatedly on three LEDS as
well as use the PWM to generate musical frequencies to a speaker
