#include "lcd.h"

static int __init lcd_init(void) {
	int ret = alloc_chrdev_region(&dev_num, 0, 1, DEVICE_NAME);
	if (ret < 0) {
		printk(KERN_ALERT "lcd: Failed to allocate a major number\n");
		return ret;
	}
	printk(KERN_INFO "lcd: major number is %d\n", MAJOR(dev_num));
	printk(KERN_INFO "Use mknod /dev/%s c %d 0 for device file\n", DEVICE_NAME, MAJOR(dev_num));


	mcdev = cdev_alloc();
	mcdev->ops = &fops;
	mcdev->owner = THIS_MODULE;

	ret = cdev_add(mcdev, dev_num, 1);
	if (ret < 0) {
		printk(KERN_ALERT "lcd: unable to add cdev to kernerl\n");
		return ret;
	}

	// CODE TO INITIALIZE...

	// Request all GPIOs
	gpio_request(GPIO_DATA, "sysfs"); // Formerly "fopen"?
	gpio_request(GPIO_EN, "sysfs");
	gpio_request(GPIO_RS, "sysfs");
	gpio_request(GPIO_RW, "sysfs");
	gpio_request(GPIO_RCLK, "sysfs");
	gpio_request(GPIO_SRCLK, "sysfs");

	// Set all GPIO as outputs and off
	gpio_set_direction(GPIO_EN, false); // Formerly "fprintf" I think
	gpio_set_direction(GPIO_DATA, false); 
	gpio_set_direction(GPIO_RCLK, false); 
	gpio_set_direction(GPIO_SRCLK, false); 
	gpio_set_direction(GPIO_RS, false); 
	gpio_set_direction(GPIO_RW, false); 

	// Export all GPIOs
	gpio_export(GPIO_EN, false); // Formerly "fflush"?
	gpio_export(GPIO_DATA, false); 
	gpio_export(GPIO_RCLK, false); 
	gpio_export(GPIO_SRCLK, false); 
	gpio_export(GPIO_RS, false); 
	gpio_export(GPIO_RW, false); 

	printk(KERN_INFO "LCD initialized");
}


static void __exit lcd_exit(void) {
	cdev_del(mcdev);
	unregister_chrdev_region(dev_num, 1);


	// Free all GPIOs from the system
	gpio_free(GPIO_EN);
	gpio_free(GPIO_RCLK);
	gpio_free(GPIO_SRCLK);
	gpio_free(GPIO_RW);
	gpio_free(GPIO_RS);
	gpio_free(GPIO_DATA);

	printk(KERN_ALERT "lcd: successfully unloaded\n");
}

int lcd_write(int d7, int d6, int d5, int d4, int d3, int d2, int d1, int d0) {
	gpio_set_value(GPIO_RCLK, 1);
	int[] a = {d7, d6, d5, d4, d3, d2, d1, d0}
	for (i=7; i>=0; i--) {
		gpio_set_value(GPIO_DATA, a[i]);
		toggleClock();
	}
	gpio_set_value(GPIO_RCLK, 0); // Flip enable signal for LCD display 
	return 0;
}

static void toggleClock() {
	gpio_set_value(GPIO_RCLK, 1);
	msleep(1);
	gpio_set_value(GPIO_RCLK, 0);
}


MODULE_LICENSE("GPL");
module_init(lcd_init);
module_exit(lcd_exit);