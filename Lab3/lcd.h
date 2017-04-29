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

static int __init lcd_init(void);
static void __exit lcd_exit(void);
static ssize_t lcd_write();
static ssize_t lcd_clear();


static struct file_operations fops = {
   .owner = THIS_MODULE,
   .write = lcd_write,
   .clear = lcd_clear
};
#endif