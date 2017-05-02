/* EE 475 - ECKER SP17
 * BB_Library.C
 * Contains helpful functions for setting UP LCD, GPIO, ETC.
 *
 */
#include <stdio.h>      // for File IO and printf
#include <time.h>       // for usleep
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#define GPIO_EXPORT_PATH "/sys/class/gpio/export"
#define GPIO_FOLDER      "/sys/class/gpio/gpio115/value"
#define RS                               115
#define RW                               20
#define EN                       112
#define DATA                     60
#define CLOCK                    49
#define LATCH                    48
#define ENOUGH           50
#define LCD_DELAY        100

// Array containing GPIO numbers to be initalized on the BBB
// Append to initialize more GPIO's
static int GPIO[] = {RS, RW, EN, DATA, CLOCK, LATCH};

// Array containing pointers to the value file for each GPIO
// iterate through this to update GPIO Values
static FILE* VALUE_ARRAY[6];


 // Get Stream. Takes a file path, and mode type as parameters
 // will wait for file to be initialized due to BB's timing delays
 // Note: if the path passed in doesn't or will never exist, getStream
 // will loop indefinetly
  FILE* getStream(char *path, char *mode) {
        while(!fopen(path, mode)) {
                usleep(1);
        }
        return fopen(path, mode);
  }

  // combines fprintf and flush into one function
  // call this to clean up the code a bit
  void writeToStream(FILE *stream, char *format, char *value) {
                fprintf(stream, format, value);
                fflush(stream);
  }

   // initializes GPIO's defined in global GPIO array
  // Sets all to Output and Initializes pointers to each
  // GPIO's value
  void initGPIO() {
        // Creates each GPIO's folder in BBB
        int i;
        FILE *exportP = getStream(GPIO_EXPORT_PATH, "w");
        for(i = 0; i < (sizeof(GPIO)/sizeof(GPIO[0])); i++) {
                writeToStream(exportP, "%d", GPIO[i]);
        }
        // Set each GPIO to output (take input from software)
        // also intializes file pointers for each GPIO's Value
        char tempPath[ENOUGH];
        FILE *currentP;
        for(i = 0; i < (sizeof(GPIO)/sizeof(GPIO[0])); i++) {
                // updates String tempPath to BBB GPIO specific folder
                sprintf(tempPath,"%s%d%s","/sys/class/gpio/gpio",GPIO[i],"/direction");
                currentP = getStream(tempPath, "w");
                writeToStream(currentP, "%s", "out");
                sprintf(tempPath,"%s%d%s","/sys/class/gpio/gpio",GPIO[i],"/value");
                VALUE_ARRAY[i] = getStream(tempPath, "w");
        }
  }
  // updates a new 8 bits onto the shift register
  void shiftData(int db7, int db6, int db5,
                                                int db4, int db3, int db2,
                                                int db1, int db0) {
                writeToStream(VALUE_ARRAY[5], "%d", 0); // latch
                writeToStream(VALUE_ARRAY[4], "%d", 0); // clock
                int datas[8] = {db7, db6, db5, db4, db3, db2, db1, db0};
                int i;
                for(i = 0; i < 8; i++) {
                        usleep(LCD_DELAY);
                        writeToStream(VALUE_ARRAY[3], "%d", datas[i]); // data
                        usleep(LCD_DELAY);
                        writeToStream(VALUE_ARRAY[4], "%d", 1); // clock
                        usleep(LCD_DELAY);
                        writeToStream(VALUE_ARRAY[4], "%d", 0); // clock
                }
                usleep(LCD_DELAY);
                writeToStream(VALUE_ARRAY[5], "%d", 1); // latch
                usleep(LCD_DELAY);
                writeToStream(VALUE_ARRAY[5], "%d", 0); // latch
        }
  // issues a new command to the LCD
  void lcd_command(int rs, int rw, int db7,
                                   int db6, int db5, int db4,
                               int db3, int db2, int db1,
                               int db0) {
                // Set Enable Low incase of poor startup
                writeToStream(VALUE_ARRAY[2], "%d", 0); // enable
                // set RS and RW
                writeToStream(VALUE_ARRAY[0], "%d", rs);
                writeToStream(VALUE_ARRAY[1], "%d", rw);
                // Updating data for shift register
                shiftData(db7, db6, db5, db4, db3, db2, db1, db0);
                writeToStream(VALUE_ARRAY[2], "%d", 1);
                usleep(LCD_DELAY);
                writeToStream(VALUE_ARRAY[2], "%d", 0);
  }


  void initLCD() {
        // START BOOT UP COMMANDS
        lcd_command(0,0,0,0,1,1,0,0,0,0);
        usleep(LCD_DELAY); // Wait >4.1ms
        lcd_command(0,0,0,0,1,1,0,0,0,0);
        usleep(LCD_DELAY); // Wait > 100us
        lcd_command(0,0,0,0,1,1,0,0,0,0);
        usleep(LCD_DELAY);
        // END BOOT UP COMMANDS
        lcd_command(0,0,0,0,1,1,1,0,0,0); // set 8-bit/2line
        usleep(LCD_DELAY);
  }

  // Testing For Basic Functionality
  int main() {
        initGPIO(); // WORKING
        initLCD();  // WORKING JESUS CHRIST
  }

