Lab 2 - EE 474
This lab utilizes the GPIO pins of the BBB in order to drive an
lcd display as well as test for jitter of the GPIO pins.

There are three main stages to this lab
1) Testing jitter using osc.c
    Compiled normally and ran as such:
    $gdb -o o osc.c
    //to run
    $./o
2) Guessing Game ggb.c
    This program is simple and doesn't use a pipe but
    has multiple states for the LCD display
    compiled normally and ran as uch:
    $gdb -o g ggb.c
    $./g
3) The third are two programs that run concurrently 
   and talk to each other using a named pipe.
   front.c and back.c
   $gdb -o f front.c
   $gdb -o g back.c
   To run both programs at once, we used two terminal windows
   IN TERMINAL 1
   $./f
   IN TERMINAL 2
   $./b
   after this, terminal 1 is used to write in strings
   that will then be displayed on the LCD. 


Lab 1 - EE 474
This lab utilizes the beaglebone black device's PWM and GPIO pins
to count upwards to seven in binary repeatedly on three LEDS as
well as use the PWM to generate musical frequencies to a speaker
