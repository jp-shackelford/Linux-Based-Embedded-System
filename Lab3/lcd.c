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

	// Set up semaphore
	sema_init(&virtual_device.sem, 1);
	msleep(100);
	// Run initialization code for LCD display
	initialize_lcd();
	return 0;
}


static void __exit lcd_exit(void) {
	cdev_del(mcdev);
	unregister_chrdev_region(dev_num, 1);

	int gpios[6] = {GPIO_EN, GPIO_RCLK, GPIO_SRCLK, GPIO_DATA, GPIO_RS, GPIO_RW};

	clear(); // Clear LCD

	// Set all pins to 0 and remove gpios
	int i;
	for (i=0; i<6; i++) {
		gpio_set_value(gpios[i], 0);
		gpio_unexport(gpios[i]);
	}

	cdev_del(mcdev);
	unregister_chrdev_region(dev_num, 1);
	printk(KERN_ALERT "lcd: successfully unloaded\n");
}

void initialize_lcd() {
	int gpios[6] = {GPIO_EN, GPIO_RCLK, GPIO_SRCLK, GPIO_DATA, GPIO_RW, GPIO_RS};

	int i;
	for (i=0; i<6; i++) {
		gpio_request(gpios[i], "sysfs"); // Request each GPIO that will be used
		gpio_direction_output(gpios[i], 0); // Set all GPIOs low and as outputs
		gpio_export(gpios[i], 1); // Export all GPIOs so they show up in system
	}

	msleep(300);
	for (i = 0; i < 3; i++) {
		command(0,0,0,0,1,1,0,0,0,0); // 0x30
		msleep(5);
	}
	command(0,0,0,0,1,1,1,0,0,0); // set 8-bit/1line
	command(0,0,0,0,0,0,1,0,0,0); // Display off
	clear();
	command(0,0,0,0,0,0,0,1,1,0); // Increment mode, no shift 
	command(0,0,0,0,0,0,1,1,1,0); // turn on display and cursor
	command(0,0,0,0,0,0,0,1,1,0); // sets cursor and stuf 
}

void shiftData(int db7, int db6, int db5, int db4, int db3, int db2, int db1, int db0) {
    gpio_set_value(GPIO_SRCLK, 0); // latch
    gpio_set_value(GPIO_RCLK, 0); // clock
    int datas[8] = {db7, db6, db5, db4, db3, db2, db1, db0};
    int i;
    for(i = 0; i < 8; i++) {
            usleep_range(10,100);
            gpio_set_value(GPIO_DATA, datas[i]); // data
            usleep_range(10,100);
            gpio_set_value(GPIO_RCLK, 1); // clock
            usleep_range(10,100);
        	gpio_set_value(GPIO_RCLK, 0); // clock
    }
    usleep_range(10,100);
    gpio_set_value(GPIO_SRCLK, 1); 
    usleep_range(10,100);
    gpio_set_value(GPIO_SRCLK, 0);
}

void write_bin(char* nums) {
	command(nums[9], nums[8], nums[7], nums[6], nums[5], nums[4], nums[3], nums[2], nums[1], nums[0]);
}

// sends a command to the LCD display
void command(int rs, int r, int d7, int d6, int d5, int d4, int d3, int d2, int d1, int d0) {
	 // Set Enable Low incase of poor startup
    gpio_set_value(GPIO_EN, 0); // enable
    // set RS and RW
    gpio_set_value(GPIO_RS, rs);
    gpio_set_value(GPIO_RW, r);
    // Updating data for shift register
    shiftData(d7, d6, d5, d4, d3, d2, d1, d0);
    gpio_set_value(GPIO_EN, 1);
    usleep_range(10,100); 
    gpio_set_value(GPIO_EN, 0);
}


// Clears the LCD display screen
void clear() {
	command(0,0,0,0,0,0,0,0,0,1);
}

unsigned int_to_bin(unsigned k) {
	printk("this is my k: %d\n", k);
	return ((k==0 || k==1) ? k : ((k%2) + (10*int_to_bin(k/2))));
}

 int* toBits(char key) {
	unsigned k;
	printk("My key is: %c\n", key);
	static int a[8];
	k = int_to_bin((unsigned)key);
	printk("This is the int_to_bin: %d\n",k);
	int i;
	for (i=0; i<8; i++) {
		a[i] = (k%2);
		k = k/10;
	}
	return a;
}

 int lengthInput(char* input) {
	int length = 0;
	char curr = 0;
	curr = input[length];
	return 1;
}

ssize_t lcd_write(struct file* filp, const char* bufSource, size_t bufCount, loff_t* cursor) {
	unsigned long ret = 0;
	printk(KERN_INFO "lcd: writing to device...\n");
	ret = copy_from_user(virtual_device.buff, bufSource, bufCount);
	char userD[10];
	copy_from_user(userD, bufSource, 10);
	int i;
	for(i = 0; i < 10; i++) {
		if('1' == userD[i]) {
			write_data[i] = 1;		
		} else if('0' == userD[i]) {
			write_data[i] = 0;		
		} else {
			printk("bad input to write\n");
			return ret; 		
		}
	}
	printk("Current data: %s\n", userD);
	command(write_data[0],write_data[1],write_data[2],
		write_data[3],write_data[4],write_data[5],
		write_data[6],write_data[7],write_data[8],
		write_data[9]);
	return ret;
}

// parses a charter to its ascii equivalent, returns
// and array for each bit of charactrs ascii 
int lcd_open(struct inode *inode, struct file* filp) {
	if (down_interruptible(&virtual_device.sem) != 0) {
		printk(KERN_ALERT "lcd: could not lock device during open\n");
		return -1;
	}
	printk(KERN_INFO "lcd: device opened\n");
	return 0;
}

int lcd_close(struct inode* inode, struct file *filp) {
	up(&virtual_device.sem);
	printk(KERN_INFO "lcd: closing device\n");
	return 0;
}

module_init(lcd_init);
module_exit(lcd_exit);
MODULE_LICENSE("GPL");
