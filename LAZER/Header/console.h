

#ifndef __CONSOLE_H
#define __CONSOLE_H

#include "main.h"

#define CMD 	0 
#define DATA 	1


typedef  uint8_t fpga_size_byte;
//=================================================
/*setting W5200 and Server's IP*/

typedef struct _CONFIG_MSG
{
	uint16 ID;
	uint8 version[3];
	uint8 day;
	uint8 month;
	uint16 year;
	

	
	uint8 Mac[6];
	uint8 Lip[4];
	uint8 Sub[4];
	uint8 Gw[4];
	uint8 DNS_Server_IP[4];
	uint16 port_science;
	uint8 DHCP;

	uint16 port;
	uint8 destip[4];
	
	uint8 day_set;
	uint8 month_set;
	uint16 year_set;
	
			
}
CONFIG_MSG;



void CONSOLE_USART_INIT(void);
void CommandProcessing( char *bufer_in, char *bufer_out);
void console_send( char* str);

bool ReadConfig(void);
void PrintVersion( char *bufer_out);
void SettingsDefault(void);
bool CheckAndWriteVersion(void);
bool ReadParameter(char * bufer, int * num_par,uint8 space);
bool ParsingParameter(uint8 * bufer , int * num_par);

#endif

