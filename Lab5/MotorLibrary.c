#include "BB_Library.c"
// motor struct, contains GPIO information 
// for a motor's controls 
typedef struct {
    FILE* IN1PIN;
    FILE* IN2PIN;
    FILE* PWM;
    FILE* STDBY;
    int OFFSET;
} Motor; 

// Returns a pointer to a Motor Struct
// Will also initalize In1pin and In2pin to 0
// initialize PWMpin to a frequency of 980hz with a duty cycle of 255
// takes inputs In1, In2, PWM, STDBY, and offset to declare the struct as such
// note that the STDBY pin for this lab MUST BE THE SAME FOR BOTH MOTORS 
void motorinit(Motor *m, int In1pin, int In2pin, int PWMpin, int STBYpin, int offset) {
  m->IN1PIN = initGPIO(In1pin);
  m->IN2PIN = initGPIO(In2pin);
  m->STDBY  = initGPIO(STBYpin); 
  /* initPWM will initalize the PWM's for this lab
    it takes an int as a parameter, 1 will init P8_19 (right side)
    and 0 will init p9_14 (left side), the function returns a 
    pointer to the duty value of the PWM that can be
    set using writeToStream */
  m->PWM = initPWM(PWMpin);
  // this offset is used for some scalar factor
  // in the motor, more on this later... 
  m->OFFSET = offset;
  // TODO-> initialize these GPIO's 
}

void fwd(Motor *m, int speed) {
   writeToStream(m->IN1PIN, "%d", "1");
   writeToStream(m->IN2PIN, "%d", "0");
   setDuty(m->PWM, speed);
}

void rev(Motor *m, int speed) {
   writeToStream(m->IN1PIN, "%d", "0");
   writeToStream(m->IN2PIN, "%d", "1");
   setDuty(m->PWM, speed);
}
void drive(Motor *m, int speed) {
  writeToStream(m->STDBY, "%d", "1");
  speed = speed * m->OFFSET;
  if (speed>=0) fwd(m, speed);
  else rev(m, -speed);
}

void brake(Motor *m) {
   writeToStream(m->IN1PIN, "%d", "1");
   writeToStream(m->IN2PIN, "%d", "1");
   setDuty(m->PWM, 0);
}

void standby(Motor *m) {
   writeToStream(m->STDBY, "%d", "0");
}

void forward_speed(Motor *m0, Motor * m1, int speed) {
  drive(m0, speed);
  drive(m1, speed);
}

void forward(Motor *m0, Motor *m1) {
  drive(m0, -8);
  drive(m1, -8);
}

void back_speed(Motor *m0, Motor *m1, int speed) {
  int temp = abs(speed);
  drive(m0, -temp);
  drive(m1, -temp);
}

void back(Motor * m0, Motor * m1) {
  drive(m0, 8);
  drive(m1, 8);
}

void left(Motor * m0, Motor * m1, int speed) {
  int temp = abs(speed);
  drive(m0, -temp);
  drive(m1, temp);
  
}

void right(Motor * m0, Motor * m1, int speed) {
  int temp = abs(speed);
  drive(m0, temp);
  drive(m1, -temp);
}

void brake_full(Motor * m0, Motor * m1) {
	brake(m0);
	brake(m1);
} 

