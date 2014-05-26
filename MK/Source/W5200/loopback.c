
#include "stm32f4xx.h"
#include "config.h"
#include "w5200.h"
#include "socket.h"
#include "util.h"
#include "loopback.h"
#include "main.h"
//#include "control_fpga.h"
#include <stdio.h>

#define tick_second 1

extern uint8 ch_status[MAX_SOCK_NUM];
extern CHCONFIG_TYPE_DEF Chconfig_Type_Def; 
extern uint32_t presentTime;
uint16 any_port = 1000;

uint8 DATA_BUFF_A[SIZE_BUF]; 



void sender_tcps(SOCKET s, uint16 port)
{	        
	uint16 RSR_len;
//	uint16 received_len;
	uint8 DATA_BUFF_IN[INPUT_SIZE_BUF]; 

//	uint16 sent_data_len = 0;
//	uint8 tmp_retry_cnt = 0;        

	
	switch (getSn_SR(s))
	{
		case SOCK_ESTABLISHED:					
			if(ch_status[s]==1)
			{
				ch_status[s] = 2;
			}
		
					send(s, DATA_BUFF_IN, MAX_SIZE_BUF, (bool)WINDOWFULL_FLAG_OFF, FPGA);  						
					
					
		/*		
			if ((RSR_len = getSn_RX_RSR(s)) > 0) 		
			{
				if (RSR_len > TX_RX_MAX_BUF_SIZE) RSR_len = TX_RX_MAX_BUF_SIZE;	
					received_len = recv(s, DATA_BUFF_IN, RSR_len); 
					LED3_onoff(ON);
					send(s, DATA_BUFF_IN, MAX_SIZE_BUF, (bool)WINDOWFULL_FLAG_OFF, FPGA);  						
					LED3_onoff(OFF);
			}*/                             
		break;
		case SOCK_CLOSE_WAIT:                           

					if ((RSR_len = getSn_RX_RSR(s)) > 0) 	
			{
				if (RSR_len > TX_RX_MAX_BUF_SIZE) RSR_len = TX_RX_MAX_BUF_SIZE;	
																		
//				received_len = recv(s, DATA_BUFF_IN, RSR_len);		
			}
			disconnect(s);
			ch_status[s] = 0;
		break;
		case SOCK_CLOSED:                                    
			if(!ch_status[s]) 
			{

				ch_status[s] = 1;
			}                
			if(socket(s,Sn_MR_TCP,port,0x00) == 0)   
			{

				ch_status[s] = 0;
			}                                			                
		break;
		case SOCK_INIT:   
			listen(s); 
		break;
		default:
		break;                
	}
}


void sender_tcpc(SOCKET s, uint16 port)
{							
    uint16 RSR_len;
//    uint16 received_len;
		uint8 DATA_BUFF_IN[INPUT_SIZE_BUF]; 

//    uint16 sent_data_len = 0;
//    uint8 tmp_retry_cnt = 0;                
	switch (getSn_SR(s))
	{
	case SOCK_ESTABLISHED:						/* if connection is established */                
		if(ch_status[s]==1)
		{
			ch_status[s] = 2;
		}

			send(s, DATA_BUFF_IN, MAX_SIZE_BUF, (bool)WINDOWFULL_FLAG_OFF, FPGA);  						
		
	break;
	case SOCK_CLOSE_WAIT:                         		/* If the client request to close */
		if ((RSR_len = getSn_RX_RSR(s)) > 0) 			/* check Rx data */
		{
			/* if Rx data size is lager than TX_RX_MAX_BUF_SIZE */
			if (RSR_len > TX_RX_MAX_BUF_SIZE) RSR_len = TX_RX_MAX_BUF_SIZE;																													/* the data size to read is MAX_BUF_SIZE. */
		//	received_len = recv(s, DATA_BUFF_A, RSR_len);		/* read the received data */
		}
		disconnect(s);
		ch_status[s] = 0;
	break;
	case SOCK_CLOSED:                                   /* if a socket is closed */               
		if(!ch_status[s])
		{
			ch_status[s] = 1;
		}		
		if(socket(s, Sn_MR_TCP, port, 0x00) == 0)    /* reinitialize the socket */
		{
			ch_status[s] = 0;
		}
	break;
    case SOCK_INIT:     /* if a socket is initiated */
        if(time_return() - presentTime >= (tick_second * 3))  /* For TCP client's connection request delay : 3 sec */
		{
		/* Try to connect to TCP server(Socket, DestIP, DestPort) */  
        	connect(s, Chconfig_Type_Def.destip, Chconfig_Type_Def.port); 
            presentTime = time_return();                       
        }                
    break;  
    default:
    break;        
    }
}

void loopback_tcps(SOCKET s, uint16 port)
{	        
    uint16 RSR_len;
    uint16 received_len;

    uint16 sent_data_len = 0;
    uint8 tmp_retry_cnt = 0;        
	
	switch (getSn_SR(s))
	{
	case SOCK_ESTABLISHED:					/* if connection is established */
		if(ch_status[s]==1)
		{
//	printf("\r\n%d : Connected",s);
//  printf("\r\n Peer IP : %d.%d.%d.%d", IINCHIP_READ(Sn_DIPR0(s)+0),  IINCHIP_READ(Sn_DIPR0(s)+1), IINCHIP_READ(Sn_DIPR0(s)+2), IINCHIP_READ(Sn_DIPR0(s)+3));
//  printf("\r\n Peer Port : %d", ( (uint16)(IINCHIP_READ(Sn_DPORT0(s)+0)<<8) +(uint16)IINCHIP_READ(Sn_DPORT0(s)+1)));
			ch_status[s] = 2;
		}
        if ((RSR_len = getSn_RX_RSR(s)) > 0) 			/* check Rx data */
		{
			if (RSR_len > TX_RX_MAX_BUF_SIZE) RSR_len = TX_RX_MAX_BUF_SIZE;	/* if Rx data size is lager than TX_RX_MAX_BUF_SIZE */
			/* the data size to read is MAX_BUF_SIZE. */
				received_len = recv(s, DATA_BUFF_A, RSR_len);   									/* read the received data */
	            sent_data_len = send(s, DATA_BUFF_A, received_len, (bool)WINDOWFULL_FLAG_OFF,MCU);    	/* sent the received data */
	        	if(sent_data_len != received_len) /* only assert when windowfull */
	            {
	            	init_windowfull_retry_cnt(s);                              
	                while(sent_data_len !=  received_len) 
	                {
//  					printf("ret :%d\r\n", sent_data_len);
	                	tmp_retry_cnt = incr_windowfull_retry_cnt(s);
	                    if(tmp_retry_cnt <= WINDOWFULL_MAX_RETRY_NUM) 
	                    {
	                    	sent_data_len += send(s, DATA_BUFF_A, received_len, (bool)WINDOWFULL_FLAG_ON,MCU); 
	                        Delay_ms(WINDOWFULL_WAIT_TIME);
	                    } 
	                    else 
	                    {
//                        printf("WindowFull !!!\r\n");
	                        close(s);
//                        printf("Socket Close !!!\r\n");
	                    	while(1);  // if the 'Windowfull' occers, socket close and sending process stop.
	                        // user can be customize(WINDOWFULL_MAX_RETRY_NUM, Windowfull handling ...)
	                    }
	                }                               
	            }
            	
		}                             
	break;
	case SOCK_CLOSE_WAIT:                           	/* If the client request to close */
//		printf("\r\n%d : CLOSE_WAIT", s);
        if ((RSR_len = getSn_RX_RSR(s)) > 0) 			/* check Rx data */
		{
			if (RSR_len > TX_RX_MAX_BUF_SIZE) RSR_len = TX_RX_MAX_BUF_SIZE;	/* if Rx data size is lager than TX_RX_MAX_BUF_SIZE */
																			/* the data size to read is MAX_BUF_SIZE. */
			received_len = recv(s, DATA_BUFF_A, RSR_len);		/* read the received data */
		}
		disconnect(s);
		ch_status[s] = 0;
	break;
	case SOCK_CLOSED:                                       /* if a socket is closed */
		if(!ch_status[s]) 
		{
//			printf("\r\n%d : Loop-Back TCP Server Started. port : %d", s, port);
			ch_status[s] = 1;
		}                
		if(socket(s,Sn_MR_TCP,port,0x00) == 0)    /* reinitialize the socket */
		{
//			printf("\r\n%d : Fail to create socket.",s);
			ch_status[s] = 0;
		}                                			                
	break;
    case SOCK_INIT:   /* if a socket is initiated */
    	listen(s); 
    break;
    default:
    break;                
	}
}

void loopback_tcpc(SOCKET s, uint16 port)
{		
	
    uint16 RSR_len;
    uint16 received_len;

    uint16 sent_data_len = 0;
    uint8 tmp_retry_cnt = 0;                
	switch (getSn_SR(s))
	{
	case SOCK_ESTABLISHED:						/* if connection is established */                
		if(ch_status[s]==1)
		{
//	printf("\r\n%d : Connected",s);			
			ch_status[s] = 2;
//	LED3_onoff(ON);
		}
		if ((RSR_len = getSn_RX_RSR(s)) > 0) 			/* check Rx data */
		{
			/* if Rx data size is lager than TX_RX_MAX_BUF_SIZE */
			if (RSR_len > TX_RX_MAX_BUF_SIZE) RSR_len = TX_RX_MAX_BUF_SIZE;	
				/* the data size to read is MAX_BUF_SIZE. */
			received_len = recv(s, DATA_BUFF_A, RSR_len);	/* read the received data */
				/* sent the received data */
			sent_data_len = send(s, DATA_BUFF_A, received_len, (bool)WINDOWFULL_FLAG_OFF,MCU);        
            if(sent_data_len != received_len) /* ohly assert when windowfull */
            {
               	init_windowfull_retry_cnt(s);                              
                while(sent_data_len !=  received_len) 
                {
//      printf("ret :%d\r\n", sent_data_len);
                   	tmp_retry_cnt = incr_windowfull_retry_cnt(s);                  
                    if(tmp_retry_cnt <= WINDOWFULL_MAX_RETRY_NUM) 
                    {
                       	sent_data_len += send(s, DATA_BUFF_A, received_len, (bool)WINDOWFULL_FLAG_ON,MCU); 
                        Delay_ms(WINDOWFULL_WAIT_TIME);
                    } 
                    else 
                    {
//     printf("WindowFull !!!\r\n");
                       	close(s);
//     printf("Socket Close !!!\r\n");
                        while(1);
                    }
                }                               
            }                        
		}
	break;
	case SOCK_CLOSE_WAIT:                         		/* If the client request to close */
//	printf("\r\n%d : CLOSE_WAIT", s);
		if ((RSR_len = getSn_RX_RSR(s)) > 0) 			/* check Rx data */
		{
			/* if Rx data size is lager than TX_RX_MAX_BUF_SIZE */
			if (RSR_len > TX_RX_MAX_BUF_SIZE) RSR_len = TX_RX_MAX_BUF_SIZE;																													/* the data size to read is MAX_BUF_SIZE. */
			received_len = recv(s, DATA_BUFF_A, RSR_len);		/* read the received data */
		}
		disconnect(s);
		ch_status[s] = 0;
	break;
	case SOCK_CLOSED:                                   /* if a socket is closed */               
		if(!ch_status[s])
		{
//	printf("\r\n%d : Loop-Back TCP Client Started. port: %d", s, port);
			ch_status[s] = 1;
		}		
		if(socket(s, Sn_MR_TCP, port, 0x00) == 0)    /* reinitialize the socket */
		{
//	printf("\a%d : Fail to create socket.",s);
			ch_status[s] = 0;
		}
	break;
    case SOCK_INIT:     /* if a socket is initiated */
        if(time_return() - presentTime >= (tick_second * 3))  /* For TCP client's connection request delay : 3 sec */
		{
		/* Try to connect to TCP server(Socket, DestIP, DestPort) */  
        	connect(s, Chconfig_Type_Def.destip, Chconfig_Type_Def.port); 
            presentTime = time_return();                       
        }                
    break;  
    default:
    break;        
    }
}


void loopback_udp(SOCKET s, uint16 port)
{
//	uint16 RSR_len;
        uint16 received_len;
	uint8 * data_buf ;
	uint32 destip = 0;
	uint16 destport;

	switch (getSn_SR(s))
	{
	case SOCK_UDP:
		
	sendto(s, data_buf, received_len,(uint8*)&destip, destport, FPGA);
	
	/*	if ((RSR_len = getSn_RX_RSR(s)) > 0) 			
		{
			if (RSR_len > TX_RX_MAX_BUF_SIZE) RSR_len = TX_RX_MAX_BUF_SIZE;	
																												
			received_len = recvfrom(s, data_buf, RSR_len, (uint8*)&destip, &destport);			
			if(sendto(s, data_buf, received_len,(uint8*)&destip, destport, FPGA) == 0)	
			{
			}
		}
	*/
		break;
	case SOCK_CLOSED:                                               /* if a socket is closed */
		if(socket(s,Sn_MR_UDP,port,0x00)== 0)    /* reinitialize the socket */
		break;
	}
}
