#include <stdlib.h>
#include <stdio.h> 

void main() {
	FILE* fp;
	char path[1024];
	fp = popen("/usr/bin/find /sys/devices/ocp.3/ -name \"helper*\" -type d", "r");
	fgets(path, sizeof(path), fp);
	usleep(100000);
	printf("%s", path); 
}
