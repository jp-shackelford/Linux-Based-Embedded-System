#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>


#define LEFT  68
#define DOWN  66
#define RIGHT 67
#define UP    65
#define CAP   5
#define FORWARD 1
#define BACKWARD 2
#define TURN_LEFT 3
#define TURN_RIGHT 4
#define FIRE 5
#define BRAKE 6
#define NO_ACTION 0
#define QUIT -1
