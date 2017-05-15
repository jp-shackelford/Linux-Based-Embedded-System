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
Motor* motorinit(Motor *m, int In1pin, int In2pin, int PWMpin, int STBYpin, int offset)
{
  m = malloc(sizeof(Motor));
  m->IN1PIN = initGPIO(In1pin);
  m->IN2PIN = initGPIO(In2pin);
  m->PWM  = initPWM(PWMpin);
  m->STDBY  = initGPIO(STBYpin);
  m->OFFSET = offset;
  // TODO-> initialize these GPIO's 
}

// for testing...
int main() {
    FILE* pwmOne = initPWM(1); 
    FILE* pwmTwo = initPWM(0); 
}

/*
void Motor::drive(int speed)
{
  digitalWrite(Standby, HIGH);
  speed = speed * Offset;
  if (speed>=0) fwd(speed);
  else rev(-speed);
}
void Motor::drive(int speed, int duration)
{
  drive(speed);
  delay(duration);
}

void Motor::fwd(int speed)
{
   digitalWrite(In1, HIGH);
   digitalWrite(In2, LOW);
   analogWrite(PWM, speed);

}

void Motor::rev(int speed)
{
   digitalWrite(In1, LOW);
   digitalWrite(In2, HIGH);
   analogWrite(PWM, speed);
}

void Motor::brake()
{
   digitalWrite(In1, HIGH);
   digitalWrite(In2, HIGH);
   analogWrite(PWM,0);
}

void Motor::standby()
{
   digitalWrite(Standby, LOW);
}

void forward(Motor motor1, Motor motor2, int speed)
{
	motor1.drive(speed);
	motor2.drive(speed);
}
void forward(Motor motor1, Motor motor2)
{
	motor1.drive(DEFAULTSPEED);
	motor2.drive(DEFAULTSPEED);
}


void back(Motor motor1, Motor motor2, int speed)
{
	int temp = abs(speed);
	motor1.drive(-temp);
	motor2.drive(-temp);
}
void back(Motor motor1, Motor motor2)
{
	motor1.drive(-DEFAULTSPEED);
	motor2.drive(-DEFAULTSPEED);
}
void left(Motor left, Motor right, int speed)
{
	int temp = abs(speed)/2;
	left.drive(-temp);
	right.drive(temp);
	
}


void right(Motor left, Motor right, int speed)
{
	int temp = abs(speed)/2;
	left.drive(temp);
	right.drive(-temp);
	
}
void brake(Motor motor1, Motor motor2)
{
	motor1.brake();
	motor2.brake();
} */ 