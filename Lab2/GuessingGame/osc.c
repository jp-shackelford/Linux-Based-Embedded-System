#import "BB_Library.c"
#define G  27 
#define GV "/sys/class/gpio/gpio27/value" // gpio value file
#define GD "/sys/class/gpio/gpio27/direction" // gpio direction file

void main() {
	FILE* sys = getStream("/sys/class/gpio/export", "w");
	writeToStream(sys, "%d", G);
	FILE* gv = getStream(GV, "w");
	FILE* gd = getStream(GD, "w");
	writeToStream(gd, "%s", "out");
	while(1) {
		writeToStream(gv, "%d", 1);
		usleep(1);
		writeToStream(gv, "%d", 0); 
		usleep(1); 
	}
}