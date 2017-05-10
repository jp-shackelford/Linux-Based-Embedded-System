#ifndef BRIDGE_H
#define BRIDGE_H

#include <stdio.h>      // for File IO and printf
#include <time.h>       // for usleep
#include <fcntl.h>
#include <stdlib.h>
#include <string.h> 
#include <sys/stat.h>
#include <unistd.h>

#define AIN1 67
#define AIN2 68
#define BIN1 44
#define BIN2 26

#define AIN1_VALUE "sys/class/gpio67/value"
#define AIN2_VALUE "sys/class/gpio68/value"
#define BIN1_VALUE "sys/class/gpio44/value"
#define BIN2_VALUE "sys/class/gpio26/value"



#endif