
#include "main.h"


typedef struct _MOTOR_STATE
{

	uint16 corner_current;
	uint16 corner_new;
	bool trailer_left;
	bool trailer_right;
	
}
MOTOR_STATE;

int motorTurn(int number,int direction, int corner );