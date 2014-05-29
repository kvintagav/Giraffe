/*
*
@file		socket.c
@brief	setting chip register for socket
		last update : 2008. Jan
*
*/

#include "stm32f4xx.h"
//#include "config.h"
#include "stdio.h"
#include "w5200.h"
#include "socket.h"
#include "util.h"
#include <stdbool.h>
static uint16 local_port;
extern uint16 sent_ptr;

/**
@brief	This Socket function initialize the channel in perticular mode, and set the port and wait for W5200 done it.
@return 	1 for sucess else 0.
*/  
uint8 socket(SOCKET s, uint8 protocol, uint16 port, uint8 flag)
{
	uint8 ret;
#ifdef __DEF_IINCHIP_DBG__
//	printf("socket()\r\n");
#endif
	if ((protocol == Sn_MR_TCP) || (protocol == Sn_MR_UDP) || (protocol == Sn_MR_IPRAW) || (protocol == Sn_MR_MACRAW) || (protocol == Sn_MR_PPPOE))
	{
		close(s);
		IINCHIP_WRITE(Sn_MR(s),protocol | flag);
		if (port != 0) {
			IINCHIP_WRITE(Sn_PORT0(s),(uint8)((port & 0xff00) >> 8));
			IINCHIP_WRITE((Sn_PORT0(s) + 1),(uint8)(port & 0x00ff));
		} else {
			local_port++; // if don't set the source port, set local_port number.
			IINCHIP_WRITE(Sn_PORT0(s),(uint8)((local_port & 0xff00) >> 8));
			IINCHIP_WRITE((Sn_PORT0(s) + 1),(uint8)(local_port & 0x00ff));
		}
		IINCHIP_WRITE(Sn_CR(s),Sn_CR_OPEN); // run sockinit Sn_CR

		/* wait to process the command... */
		while( IINCHIP_READ(Sn_CR(s)) ) 
			;
		/* ------- */
		ret = 1;
	}
	else
	{
		ret = 0;
	}
#ifdef __DEF_IINCHIP_DBG__
	//printf("Sn_SR = %.2x , Protocol = %.2x\r\n", IINCHIP_READ(Sn_SR(s)), IINCHIP_READ(Sn_MR(s)));
#endif
	return ret;
}


/**
@brief	This function close the socket and parameter is "s" which represent the socket number
*/ 
void close(SOCKET s)
{
#ifdef __DEF_IINCHIP_DBG__
	//printf("close()\r\n");
#endif
	
	IINCHIP_WRITE(Sn_CR(s),Sn_CR_CLOSE);

	/* wait to process the command... */
	while( IINCHIP_READ(Sn_CR(s)) ) 
		;
	/* ------- */
                /* all clear */
		IINCHIP_WRITE(Sn_IR(s), 0xFF);
}


/**
@brief	This function established  the connection for the channel in passive (server) mode. This function waits for the request from the peer.
@return	1 for success else 0.
*/ 
uint8 listen(SOCKET s)
{
	uint8 ret;
#ifdef __DEF_IINCHIP_DBG__
	//printf("listen()\r\n");
#endif
	if (IINCHIP_READ(Sn_SR(s)) == SOCK_INIT)
	{
		IINCHIP_WRITE(Sn_CR(s),Sn_CR_LISTEN);
		/* wait to process the command... */
		while( IINCHIP_READ(Sn_CR(s)) ) 
			;
		/* ------- */
		ret = 1;
	}
	else
	{
		ret = 0;
#ifdef __DEF_IINCHIP_DBG__
	//printf("Fail[invalid ip,port]\r\n");
#endif
	}
	return ret;
}


/**
@brief	This function established  the connection for the channel in Active (client) mode. 
		This function waits for the untill the connection is established.
		
@return	1 for success else 0.
*/ 
uint8 connect(SOCKET s, uint8 * addr, uint16 port)
{
	uint8 ret;
#ifdef __DEF_IINCHIP_DBG__
	//printf("connect()\r\n");
#endif
	if 
		(
			((addr[0] == 0xFF) && (addr[1] == 0xFF) && (addr[2] == 0xFF) && (addr[3] == 0xFF)) ||
		 	((addr[0] == 0x00) && (addr[1] == 0x00) && (addr[2] == 0x00) && (addr[3] == 0x00)) ||
		 	(port == 0x00) 
		) 
 	{
 		ret = 0;
#ifdef __DEF_IINCHIP_DBG__
	//printf("Fail[invalid ip,port]\r\n");
#endif
	}
	else
	{
		ret = 1;
		// set destination IP
		IINCHIP_WRITE(Sn_DIPR0(s),addr[0]);
		IINCHIP_WRITE((Sn_DIPR0(s) + 1),addr[1]);
		IINCHIP_WRITE((Sn_DIPR0(s) + 2),addr[2]);
		IINCHIP_WRITE((Sn_DIPR0(s) + 3),addr[3]);
		IINCHIP_WRITE(Sn_DPORT0(s),(uint8)((port & 0xff00) >> 8));
		IINCHIP_WRITE((Sn_DPORT0(s) + 1),(uint8)(port & 0x00ff));
		IINCHIP_WRITE(Sn_CR(s),Sn_CR_CONNECT);
                /* wait for completion */
		while ( IINCHIP_READ(Sn_CR(s)) ) ;

	}
	
	return ret;
}



/**
@brief	This function used for disconnect the socket and parameter is "s" which represent the socket number
@return	1 for success else 0.
*/ 
void disconnect(SOCKET s)
{
#ifdef __DEF_IINCHIP_DBG__
	//printf("disconnect()\r\n");
#endif
	IINCHIP_WRITE(Sn_CR(s),Sn_CR_DISCON);

	/* wait to process the command... */
	while( IINCHIP_READ(Sn_CR(s)) ) 
		;
	/* ------- */
}


/**
@brief	This function used to send the data in TCP mode
@return	1 for success else 0.
*/ 
uint16 send(SOCKET s, const uint8 * buf, uint16 len, bool retry, bool master)
{
	uint8 status=0;
	uint16 ret=0;
	uint16 freesize=0;
	uint16 txrd, txrd_before_send;

#ifdef __DEF_IINCHIP_DBG__
	//printf("send()\r\n");
#endif        
        if(retry) ;
        else {     
           if (len > getIINCHIP_TxMAX(s)) ret = getIINCHIP_TxMAX(s); // check size not to exceed MAX size.
           else ret = len;
        
                 // if freebuf is available, start.
                do 
                {
                        freesize = getSn_TX_FSR(s);
                        status = IINCHIP_READ(Sn_SR(s));
                        if ((status != SOCK_ESTABLISHED) && (status != SOCK_CLOSE_WAIT))
                        {
                                ret = 0; 
                                break;
                        }
#ifdef __DEF_IINCHIP_DBG__
	//	printf("socket %d freesize(%d) empty or error\r\n", s, freesize);
#endif
                } while (freesize < ret);

                // copy data
                send_data_processing(s, (uint8 *)buf, ret, master);
        }        
        
        txrd_before_send = IINCHIP_READ(Sn_TX_RD0(s));
        txrd_before_send = (txrd_before_send << 8) + IINCHIP_READ(Sn_TX_RD0(s) + 1);
  
	IINCHIP_WRITE(Sn_CR(s),Sn_CR_SEND);
	
	/* wait to process the command... */
	while( IINCHIP_READ(Sn_CR(s)) ){};
	
		while ( (IINCHIP_READ(Sn_IR(s)) & Sn_IR_SEND_OK) != Sn_IR_SEND_OK )
        {
                  if(IINCHIP_READ(Sn_IR(s)) == SOCK_CLOSED)
                  {
#ifdef __DEF_IINCHIP_DBG__
	//	    printf("SOCK_CLOSED.\r\n");                    
#endif
		    close(s);
		    return 0;
                  }
		}
                IINCHIP_WRITE(Sn_IR(s), Sn_IR_SEND_OK);
                
                txrd = IINCHIP_READ(Sn_TX_RD0(s));
                txrd = (txrd << 8) + IINCHIP_READ(Sn_TX_RD0(s) + 1);
                
                if(txrd > txrd_before_send) {
                  ret = txrd - txrd_before_send;
                } else {
                  ret = (0xffff - txrd_before_send) + txrd + 1;
                }
                
                return ret;

}



/**
@brief	This function is an application I/F function which is used to receive the data in TCP mode.
		It continues to wait for data as much as the application wants to receive.
		
@return	received data size for success else -1.
*/ 
uint16 recv(SOCKET s, uint8 * buf, uint16 len)
{
	uint16 ret=0;
#ifdef __DEF_IINCHIP_DBG__
//	printf("recv()\r\n");
#endif

	if ( len > 0 )
	{
		recv_data_processing(s, buf, len);
		IINCHIP_WRITE(Sn_CR(s),Sn_CR_RECV);

		/* wait to process the command... */
		while( IINCHIP_READ(Sn_CR(s)));
		/* ------- */
	
		ret = len;
	}
	return ret;
}


/**
@brief	This function is an application I/F function which is used to send the data for other then TCP mode. 
		Unlike TCP transmission, The peer's destination address and the port is needed.
		
@return	This function return send data size for success else -1.
*/ 
uint16 sendto(SOCKET s, const uint8 * buf, uint16 len, uint8 * addr, uint16 port, bool master)
{
	uint16 ret=0;
	
#ifdef __DEF_IINCHIP_DBG__
//	printf("sendto()\r\n");
#endif
   if (len > getIINCHIP_TxMAX(s)) ret = getIINCHIP_TxMAX(s); // check size not to exceed MAX size.
   else ret = len;

	if
		(
		 	((addr[0] == 0x00) && (addr[1] == 0x00) && (addr[2] == 0x00) && (addr[3] == 0x00)) ||
		 	((port == 0x00)) ||(ret == 0)
		) 
 	{
 	   /* added return value */
 	   ret = 0;
#ifdef __DEF_IINCHIP_DBG__
//	printf("%d Fail[%.2x.%.2x.%.2x.%.2x, %.d, %d]\r\n",s, addr[0], addr[1], addr[2], addr[3] , port, len);
//	printf("Fail[invalid ip,port]\r\n");
#endif
	}
	else
	{
		IINCHIP_WRITE(Sn_DIPR0(s),addr[0]);
		IINCHIP_WRITE((Sn_DIPR0(s) + 1),addr[1]);
		IINCHIP_WRITE((Sn_DIPR0(s) + 2),addr[2]);
		IINCHIP_WRITE((Sn_DIPR0(s) + 3),addr[3]);
		IINCHIP_WRITE(Sn_DPORT0(s),(uint8)((port & 0xff00) >> 8));
		IINCHIP_WRITE((Sn_DPORT0(s) + 1),(uint8)(port & 0x00ff));     			
  		// copy data
 		send_data_processing(s, (uint8 *)buf, ret, master);
		IINCHIP_WRITE(Sn_CR(s),Sn_CR_SEND);
		/* wait to process the command... */
		while( IINCHIP_READ(Sn_CR(s)) ) 
			;
		/* ------- */

	   while ( (IINCHIP_READ(Sn_IR(s)) & Sn_IR_SEND_OK) != Sn_IR_SEND_OK ) 
	   {
    	        if (IINCHIP_READ(Sn_IR(s)) & Sn_IR_TIMEOUT)
		{
#ifdef __DEF_IINCHIP_DBG__
//				printf("send fail.\r\n");
#endif
                /* clear interrupt */
           	IINCHIP_WRITE(Sn_IR(s), (Sn_IR_SEND_OK | Sn_IR_TIMEOUT)); /* clear SEND_OK & TIMEOUT */
		return 0;
		}
	   }

	   IINCHIP_WRITE(Sn_IR(s), Sn_IR_SEND_OK);
	}
	return ret;
}


/**
@brief	This function is an application I/F function which is used to receive the data in other then
	TCP mode. This function is used to receive UDP, IP_RAW and MAC_RAW mode, and handle the header as well. 
	
@return	This function return received data size for success else -1.
*/ 

uint16 recvfrom(SOCKET s, uint8 * buf, uint16 len, uint8 * addr, uint16 *port)
{
	uint8 head[8];
	uint16 data_len=0;
	uint16 ptr=0;
#ifdef __DEF_IINCHIP_DBG__
//	printf("recvfrom()\r\n");
#endif

	if ( len > 0 )
	{
   	ptr = IINCHIP_READ(Sn_RX_RD0(s));
   	ptr = ((ptr & 0x00ff) << 8) + IINCHIP_READ(Sn_RX_RD0(s) + 1);
#ifdef __DEF_IINCHIP_DBG__
 //  	printf("ISR_RX: rd_ptr : %.4x\r\n", ptr);
#endif
   	switch (IINCHIP_READ(Sn_MR(s)) & 0x07)
   	{
   	case Sn_MR_UDP :
   			read_data(s, (uint8 *)ptr, head, 0x08);
   			ptr += 8;
   			// read peer's IP address, port number.
    		addr[0] = head[0];
   			addr[1] = head[1];
   			addr[2] = head[2];
   			addr[3] = head[3];
   			*port = head[4];
   			*port = (*port << 8) + head[5];
   			data_len = head[6];
   			data_len = (data_len << 8) + head[7];
   			
#ifdef __DEF_IINCHIP_DBG__
 //  			printf("UDP msg arrived\r\n");
 //  			printf("source Port : %d\r\n", *port);
 //  			printf("source IP : %d.%d.%d.%d\r\n", addr[0], addr[1], addr[2], addr[3]);
#endif

			read_data(s, (uint8 *)ptr, buf, data_len); // data copy.
			ptr += data_len;

			IINCHIP_WRITE(Sn_RX_RD0(s),(uint8)((ptr & 0xff00) >> 8));
			IINCHIP_WRITE((Sn_RX_RD0(s) + 1),(uint8)(ptr & 0x00ff));
   			break;
   
   	case Sn_MR_IPRAW :
   			read_data(s, (uint8 *)ptr, head, 0x06);
   			ptr += 6;
   
   			addr[0] = head[0];
   			addr[1] = head[1];
   			addr[2] = head[2];
   			addr[3] = head[3];
   			data_len = head[4];
   			data_len = (data_len << 8) + head[5];
   	
#ifdef __DEF_IINCHIP_DBG__
 //  			printf("IP RAW msg arrived\r\n");
 //  			printf("source IP : %d.%d.%d.%d\r\n", addr[0], addr[1], addr[2], addr[3]);
#endif
			read_data(s, (uint8 *)ptr, buf, data_len); // data copy.
			ptr += data_len;

			IINCHIP_WRITE(Sn_RX_RD0(s),(uint8)((ptr & 0xff00) >> 8));
			IINCHIP_WRITE((Sn_RX_RD0(s) + 1),(uint8)(ptr & 0x00ff));
   			break;
   	case Sn_MR_MACRAW :
   			read_data(s,(uint8*)ptr,head,2);
   			ptr+=2;
   			data_len = head[0];
   			data_len = (data_len<<8) + head[1] - 2;
   			if(data_len > 1514) 
   			{
   	//			printf("data_len over 1514\r\n");
   				while(1);
   			}

   			read_data(s,(uint8*) ptr,buf,data_len);
   			ptr += data_len;
   			IINCHIP_WRITE(Sn_RX_RD0(s),(uint8)((ptr & 0xff00) >> 8));
   			IINCHIP_WRITE((Sn_RX_RD0(s) + 1),(uint8)(ptr & 0x00ff));
   			
#ifdef __DEF_IINCHIP_DGB__
//			printf("MAC RAW msg arrived\r\n");
//			printf("dest mac=%.2X.%.2X.%.2X.%.2X.%.2X.%.2X\r\n",buf[0],buf[1],buf[2],buf[3],buf[4],buf[5]);
//			printf("src  mac=%.2X.%.2X.%.2X.%.2X.%.2X.%.2X\r\n",buf[6],buf[7],buf[8],buf[9],buf[10],buf[11]);
//			printf("type    =%.2X%.2X\r\n",buf[12],buf[13]); 
#endif			
			break;

   	default :
   			break;
   	}
		IINCHIP_WRITE(Sn_CR(s),Sn_CR_RECV);


		while( IINCHIP_READ(Sn_CR(s)) ) ;

	}
#ifdef __DEF_IINCHIP_DBG__
//	printf("recvfrom() end ..\r\n");
#endif
 	return data_len;
}


