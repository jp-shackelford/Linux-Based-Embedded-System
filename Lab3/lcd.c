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

	int gpios[6] = {GPIO_EN, GPIO_RCLK, GPIO_SRCLK, GPIO_DATA, GPIO_RCLK, GPIO_SRCLK};

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

static void initialize_lcd() {
	busy = 1; // Prevents writing while initializing
	int gpios[6] = {GPIO_EN, GPIO_RCLK, GPIO_SRCLK, GPIO_DATA, GPIO_RCLK, GPIO_SRCLK};

	int i;
	for (i =0; i<6; i++) {
		gpio_request(gpios[i], "sysfs"); // Request each GPIO that will be used
		gpio_set_direction(gpios[i], 0); // Set all GPIOs low and as outputs
		gpio_export(gpios[i], 1); // Export all GPIOs so they show up in system
	}

	udelay(30000);
	for (i = 0; i < 3; i++) {
		lcd_write(0,0,0,0,1,1,0,0,0,0); // 0x30
		udelay(5000);
	}
	lcd_write(0,0,0,0,1,1,1,0,0,0); // set 8-bit/2line
	busy = 0; // Ready for use
}

// sends a command to the LCD display
void write_to_lcd(int d7, int d6, int d5, int d4, int d3, int d2, int d1, int d0) {
	
	busy = 1; // Indicates that we are currently writing to the LCD
	
	int a[8] = {d7, d6, d5, d4, d3, d2, d1, d0}
	
	// Shift data through from 7->0
	for (i=7; i>=0; i--) {
		gpio_set_value(GPIO_DATA, a[i]);
		toggleShiftClock();
	}

	// Toggle clocks to put data onto LCD
	gpio_set_value(GPIO_RCLK, 1); // Flip LCD clock high
	udelay(40);
	gpio_set_value(GPIO_SRCLK, 1); // Store data into shift buffer
	udelay(40);
	gpio_set_value(GPIO_SRCLK, 0);
	udelay(40);
	gpio_set_value(GPIO_RCLK, 0); // Push data into LCD with falling edge
	udelay(40);

	busy = 0; // done writing
}

// Shifts the data one bit in the shift register. Does not store.
static void toggleShiftClock() {
	udelay(40);
	gpio_set_value(GPIO_RCLK, 1);
	udelay(40);
	gpio_set_value(GPIO_RCLK, 0);
	udelay(40);
}

// Clears the LCD display screen
void clear() {
	write_to_lcd(0,0,0,0,0,0,0,0,0,1);
}


static ssize_t lcd_write(struct file* flip, const char* bufSource, size_t bufCount) {

	int gpios[6] = {GPIO_EN, GPIO_RCLK, GPIO_SRCLK, GPIO_DATA, GPIO_RCLK, GPIO_SRCLK};
	unsigned long ret = 0;
	
	printk(KERN_INFO "lcd: writing to device...\n");
	printk(KERN_INFO "%s", bufSource);

	// Check for invalid cases
	if (busy) { // If already writing, discard new data
		printk(KERN_INFO "Busy writing; Throwing away new data");
	} else { // Not busy
		// Put data into virtual device
		ret = copy_from_user(virtual_device.data, bufSource, bufCount);

		clear(); // Clear display on every new write
		int* d = toBits(virtual_device.data); // Converts character into ascii array
		write_to_lcd(d[7], d[6], d[5], d[4], d[3], d[2], d[1], d[0]);
	}

	return ret;
}

// helper function for toBits
static unsigned int_to_bin (unsigned k) {
	return (k == 0 || k == 1 ? k : ((k % 2) + 10 * int_to_bin(k / 2)));
}

// parses a charter to its ascii equivalent, returns
// and array for each bit of charactrs ascii 
static int* toBits(char key) {
        unsigned k;
        int bit;
        char exit_key = 'q';
		static int a[8];	
        k = int_to_bin((unsigned)key); // get key's ascii code
       //Parses the key's ascii code, diplaying one bit at a time
	//while( k != NULL && key != exit_key) {
	int i;
	for(i = 0; i < 8; i++) {
		bit = (k % 2);
        	k = k / 10;
		a[i] = bit;
	}
	return a;
}

static int lcd_open(struct inode *inode, struct file* filp) {
	if (down_interruptible(&virtual_device.sem) != 0) {
		printk(KERN_ALERT "lcd: could not lock device during open\n");
		return -1;
	}
	printk(KERN_INFO "lcd: device opened\n");
	return 0;
}

static int lcd_close(struct inode* inode, struct file *filp) {
	up(&virtual_device.sem);
	printk(KERN_INFO "lcd: closing device\n");
	return 0;
}

MODULE_LICENSE("GPL");
module_init(lcd_init);
module_exit(lcd_exit);