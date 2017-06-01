#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>
#include <sys/time.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>

#define CAP   5
#define FORWARD 1
#define BACKWARD 2
#define TURN_LEFT 3
#define TURN_RIGHT 4
#define FIRE 5
#define BRAKE 6
#define NO_ACTION 0
#define QUIT -1
