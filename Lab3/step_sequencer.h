	#define C 3822629
	#define D 3405299
	#define E 2873442
	#define F 2702775
	#define G 2551020
	#define A 2145186
	#define B 1911132

	static FILE* musPeriod;
   static FILE* musDuty;
   static FILE* musEnable;

	void init_pwm();
	void getInput(char *prompt, char *inputBuffer, int buffersize); 
	void play(int period, char time);
