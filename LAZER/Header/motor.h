#ifndef __MOTOR_H
#define __MOTOR_H



#include "main.h"


#define DELAY_MOTOR 1000000
#define MAX_NUMBERS_TICKS 800

#define INPORT 		0
#define OUTPORT		2
#define POLARPORT	4
#define CONFPORT	6


#define OPEN false
#define CLOSE true
	
#define NUMBERS_MOTOR 4

#define EE_ERROR 0
#define NO_ERROR 1

/*
// for 4-faze turn
#define FAZA0 0x0C
#define FAZA1 0x18
#define FAZA2 0x30
#define FAZA3 0x24
*/


// for 8-faze turn
#define FAZA0 0x0C
#define FAZA1 0x08
#define FAZA2 0x18
#define FAZA3 0x10
#define FAZA4 0x30
#define FAZA5 0x20
#define FAZA6 0x24
#define FAZA7 0x04


/*structure PCA9539PW /1
	MOTOR1 
0.0 sens 	1
0.1 sens 	2
0.2 obm 	1
0.3 obm 	2
0.4 obm 	3
0.5 obm 	4
0.6 en		1
0.7 en		2

	MOTOR2 
1.0 en 		1
1.1 en	 	2
1.2 obm 	1
1.3 obm 	2
1.4 obm 	3
1.5 obm 	4
1.6 sens	1
1.7 sens	2

structure PCA9539PW /2
	MOTOR3 
0.0 en 		1
0.1 en	 	2
0.2 obm 	1
0.3 obm 	2
0.4 obm 	3
0.5 obm 	4
0.6 sens	1
0.7 sens	2

	MOTOR4 
1.0 sens 	1
1.1 sens 	2
1.2 obm 	1
1.3 obm 	2
1.4 obm 	3
1.5 obm 	4
1.6 en		1
1.7 en		2



*/


typedef struct _MOTOR_STATE
{
	
	uint8 address_i2c;
	uint8 mask_enable;
	uint8 mask_senser;
	uint8 mask_senser_open;
	uint8 mask_senser_close;
	bool port;
	uint8 current_faza;
	int current_tick;
	int current_percent;
	int max_count_tick;
	bool work;
	
}
MOTOR_STATE;

bool motorSendI2C(uint8 address, uint8 cmd ,uint8 data );
bool motorRecvI2C(uint8 address, uint8 cmd ,uint8 *data );
int motorTurn(int number, bool direction,int tick ,bool turn_to_senser);
int motorTurnOnPercent(int number , int percent);
void motorSettings(void);
void motorInit(void);
void motorTest(void);
void motorInitGpio(void);
void motorSenser(void);
void MOTOR_HANDLER(void);
void motorInitTimer(void);
#endif
