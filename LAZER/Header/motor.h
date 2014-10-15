#ifndef __MOTOR_H
#define __MOTOR_H



#include "main.h"

#define INPORT 		0
#define OUTPORT		2
#define POLARPORT	4
#define CONFPORT	6


//#define DISABLE 0
//#define ENABLE 	1

#define EE_ERROR 0
#define NO_ERROR 1

//#define MASK_ENABLE_0  0x00C0
//#define MASK_ENABLE_1  0x0300

#define FAZA0 0x0C
#define FAZA1 0x18
#define FAZA2 0x30
#define FAZA3 0x24

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
	
	bool senser_open;
	bool senser_close;
	uint8 address_i2c;
	uint8 mask_enable;
	uint8 mask_senser;
	bool port;
	uint8 current_faza;
}
MOTOR_STATE;

bool motorSendI2C(uint8 address, uint8 cmd ,uint8 data );
bool motorRecvI2C(uint8 address, uint8 cmd ,uint8 *data );
int motorTurn(int number,int direction, int tick );
void init_motor(void);
void motorTest(void);
void init_gpio_motor(void);
void motor_senser(void);

#endif
