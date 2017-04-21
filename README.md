Lab 2 - EE 474
This lab utilizes the GPIO pins of the BBB in order to drive an <br /> 
lcd display as well as test for jitter of the GPIO pins.<br /> 
<br /> 
There are three main stages to this lab<br /> 
1) Testing jitter using osc.c<br /> 
    Compiled normally and ran as such:<br /> 
    $gdb -o o osc.c<br /> 
    //to run<br /> 
    $./o<br /> 
2) Guessing Game ggb.c<br /> 
    This program is simple and doesn't use a pipe but<br /> 
    has multiple states for the LCD display<br /> 
    compiled normally and ran as uch:<br /> 
    $gdb -o g ggb.c<br /> 
    $./g<br /> 
3) The third are two programs that run concurrently <br /> 
   and talk to each other using a named pipe.<br /> 
   front.c and back.c<br /> 
   $gdb -o f front.c<br /> 
   $gdb -o g back.c<br /> 
   To run both programs at once, we used two terminal windows<br /> 
   IN TERMINAL 1<br /> 
   $./f<br /> 
   IN TERMINAL 2<br /> 
   $./b<br /> 
   after this, terminal 1 is used to write in strings<br /> 
   that will then be displayed on the LCD. <br /> 
<br /> 

Lab 1 - EE 474
This lab utilizes the beaglebone black device's PWM and GPIO pins
to count upwards to seven in binary repeatedly on three LEDS as
well as use the PWM to generate musical frequencies to a speaker
