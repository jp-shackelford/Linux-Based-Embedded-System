#ifndef LCD_H_
#define LCD_H_

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/gpio.h> // For all GPIO functionality
#include <linux/delay.h> // For msleep
#include <linux/cdev.h>
#include <linux/semaphore.h>
#include <asm/uaccess.h>

#define DEVICE_NAME "lcd"

#define GPIO_EN 112
#define GPIO_RW 20
#define GPIO_RS 115
#define GPIO_DATA 60
#define GPIO_RCLK 49
#define GPIO_SRCLK 48


struct fake_device {
	char data[100];
	struct semaphore sem;
} virtual_device;


static struct cdev* mcdev;
static dev_t dev_num;
static int busy;

static int __init lcd_init(void);
static void __exit lcd_exit(void);
static int lcd_open(struct inode*, struct file*);
static int lcd_close(struct inode*, struct file*);
static ssize_t lcd_write(struct file*, const char*, size_t, loff_t*);

static void initialize_lcd(void);
static void toggleShiftClock(void);
static unsigned int_to_bin (unsigned k);
static int* toBits(char key);

void write_to_lcd(int rs, int r, int d7, int d6, int d5, int d4, int d3, int d2, int d1, int d0);
void clear(void);


/* operations usable by this file. */
static struct file_operations fops = {
   .owner = THIS_MODULE,
   .write = lcd_write,
   .open = lcd_open,
   .release = lcd_close
};
#endif
