#include "TankHeader.h"

static int shots = 5;

// reads and interprets the keystroke.
// Returns 0 if no action needs to be taken, -1 for a quit
// Otherwise returns the positive integer action to take
int readButton(int press) {
	// Arrow keys take 3 getch() calls
	// hit ESC Q/q to quit
	if (press == 27) { 
		if (getch() == 91) {
			int button = getch();
			// Interpret which arrow key (if any) was pressed
			switch(button) {
				case LEFT :
					printf("%s\n", "LEFT");
					return TURN_LEFT;
				case RIGHT :
					printf("%s\n", "RIGHT");
					return TURN_RIGHT;
				case DOWN :
					printf("%s\n", "DOWN");
					return BACKWARD;
				case UP :
					printf("%s\n", "UP");
					return FORWARD;
				default : 
					return NO_ACTION;
		    }

		} else { // If got ESC followed by a non-arrow key
			int p = 0;
			printf("%s\n", "paused");
			while (p != 27) { // Loop until ESC pressed again
				p = getch();
				if (p == 113 || p == 81) { // q or Q
					printf("%s\n", "quitting");
					exit(EXIT_SUCCESS);
					return QUIT;
				}
			}
			printf("%s\n", "back to game");
			return NO_ACTION;
		}
	} else if (press == 119 || press == 87) {
		printf("%s\n", "UP");
		return FORWARD;
	} else if (press == 97 || press == 65) {
		printf("%s\n", "LEFT");
		return TURN_LEFT;
	} else if (press == 115 || press == 83) {
		printf("%s\n", "DOWN");
		return BACKWARD;
	} else if (press == 100 || press == 68) {
		printf("%s\n", "RIGHT");
		return TURN_RIGHT;
	} else if (press == 32 || press == 102 || press == 70) {
		if (shots > 0) {
			shots--;
			printf("%s%d\n", "FIRE!            ", shots);
			return FIRE;
		} else {
			printf("%s\n", "out of ammo! Must reload");
			return NO_ACTION;
		}
	} else if (press == 114 || press == 82) {
		if (shots < CAP) {
			printf("%s\n", "Reloading");
			sleep(5);
			shots = 5;
			printf("%s\n", "Done reloading");
		}
		return NO_ACTION;
	} else {
		return NO_ACTION;
	}
}

int getch(void) {
      int c = 0;

      struct termios org_opts, new_opts;
      int res = 0;
          //-----  store old settings -----------
      res = tcgetattr(STDIN_FILENO, &org_opts);
      assert(res == 0);
          //---- set new terminal parms --------
      memcpy(&new_opts, &org_opts, sizeof(new_opts));
      new_opts.c_lflag &= ~(ICANON | ECHO | ECHOE | ECHOK | ECHONL | ECHOPRT | ECHOKE | ICRNL);
      tcsetattr(STDIN_FILENO, TCSANOW, &new_opts);
      c = getchar();
          //------  restore old settings ---------
      res = tcsetattr(STDIN_FILENO, TCSANOW, &org_opts);
      assert(res == 0);
      return(c);
}

int main() {

	int prevAction = 0;
	while (1) {
		int action = readButton(getch());
		if (action > 0) {
			if (action == FORWARD && prevAction == BACKWARD) {
				printf("%s\n", "Will send BRAKE");
			} else if (action == BACKWARD && prevAction == FORWARD) {
				printf("%s\n", "Will send BRAKE");
			} else {
				printf("%s%d\n", "Will send ", action);
			}
		}
		prevAction = action; // Store action
	}
 
	return 0;
}