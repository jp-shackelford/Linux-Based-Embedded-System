/* EE474, ECKER, SPRING17
 * LCD.h, definitions
 * for multiple GPIO pinouts. 
 */ 
#ifndef LCD_H_
#define LCD_H_

#define RS  48
#define RW  49
#define E   27
#define DB0 115
#define DB1 20
#define DB2 60
#define DB3 112
#define DB4 66
#define DB5 69
#define DB6 45
#define DB7 47
// value paths for GPIO(S)
#define RS_VALUE "/sys/class/gpio48/value"
#define RW_VALUE "/sys/class/gpio49/value"
#define E_VALUE "/sys/class/gpio27/value"
#define DB0_VALUE "/sys/class/gpio115/value"
#define DB1_VALUE "/sys/class/gpio20/value"
#define DB2_VALUE "/sys/class/gpio60/value"
#define DB3_VALUE "/sys/class/gpio112/value"
#define DB4_VALUE "/sys/class/gpio66/value"
#define DB5_VALUE "/sys/class/gpio69/value"
#define DB6_VALUE "/sys/class/gpio45/value"
#define DB7_VALUE "/sys/class/gpio47/value"
// direction paths for GPIO(S)
#define RS_DIRECTION "/sys/class/gpio48/direction"
#define RW_DIRECTION "/sys/class/gpio49/direction"
#define E_DIRECTION "/sys/class/gpio27/direction"
#define DB0_DIRECTION "/sys/class/gpio115/direction"
#define DB1_DIRECTION "/sys/class/gpio20/direction"
#define DB2_DIRECTION "/sys/class/gpio60/direction"
#define DB3_DIRECTION "/sys/class/gpio112/direction"
#define DB4_DIRECTION "/sys/class/gpio66/direction"
#define DB5_DIRECTION "/sys/class/gpio69/direction"
#define DB6_DIRECTION "/sys/class/gpio45/direction"
#define DB7_DIRECTION "/sys/class/gpio47/direction"
#endif // LCD_H_
