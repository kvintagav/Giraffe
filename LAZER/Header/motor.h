#ifndef __MOTOR_H
#define __MOTOR_H



#include "main.h"

#define DISABLE 0
#define ENABLE 	1

#define EE_ERROR 0
#define NO_ERROR 1

#define MASK_ENABLE_0  0x00C0
#define MASK_ENABLE_1  0x0300

/*structure PCA9539PW
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
	
	uint16 corner_current;
	uint16 corner_new;
	bool trailer_left;
	bool trailer_right;
	uint8 error;
	uint8 address_i2c;
}
MOTOR_STATE;

int motorTurn(int number,int direction, int corner );
void init_motor(void);

#endif
