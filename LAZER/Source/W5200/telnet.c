#include "stm32f4xx.h"
#include "types.h"
#include "W5200\w5200.h"
#include "W5200\socket.h"
#include "config.h"
#include "W5200\telnet.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define FALSE 0
enum tel_cmd {
  HELP_CMD,
  GET_LED_CMD,
  LED3_ON_CMD,
  LED4_ON_CMD,
  LED3_OFF_CMD,
  LED4_OFF_CMD,
  EXIT_CMD  
};

// Command table
char *commands[] = {
  "help",
  "get led",
  "led3 on",
  "led4 on",
  "led3 off",
  "led4 off",
  "exit",
  NULL
};

// Telnet options
char *tel_options[] = {
        "Transmit Binary",
        "Echo",
	"",
	"Suppress Go Ahead",
	"",
	"Status",
	"Timing Mark"
};

//buf in send() : const uint8 * buf
char buf[LINELEN] = {0,};
uint8 remote[NOPTIONS] = {0,};

uint8 telnet_ID[] = {"wiznet"};
uint8 telnet_PW[] = {"0000"};

uint8 user_name[DATA_BUF_SIZE];
uint8 user_password[DATA_BUF_SIZE];
char data_buf[DATA_BUF_SIZE];

uint16 buf_index;

/** 0:close, 1:ready, 2:connected */
extern uint8 ch_status[MAX_SOCK_NUM];

uint8 user_state;

void TELNETS(SOCKET s, uint16 port)
{  
  switch (getSn_SR(s))	{  /* Get the state of socket s */
    case SOCK_ESTABLISHED :   /* If the socket is established */	    
      if(ch_status[s] == ready_state) {  
        printf("\r\nTdkvdELNET server established via SOCKET %d\r\n", s);
        init_telopt(s);     /* Initialize and negotiate the options */
        ch_status[s] = connected_state;  
      }
            
      if(getSn_RX_RSR(s) > 0) {             
        tel_input(s);     /* If there is any received data, process it */          
      }
      break;    
  
    case SOCK_CLOSE_WAIT :      
      printf("\r\nSOCKET %d : CLOSE_WAIT", s);
      disconnect(s);     /* Disconnect the socket s */          
      break;      
  
    case SOCK_CLOSED :
      if(!ch_status[s]) {      
        printf("\r\n----------------------------------- \r\n");
        printf("\r\nW5200 TELNET server start!");
        ch_status[s] = ready_state;           
      } 
      if(socket(s,Sn_MR_TCP,port,0x00) == 0) {     /* reinitialize the socket */     
        printf("\r\n%d : Fail to create socket.", s);
        ch_status[s] = close_state;
      } else {
        listen(s);     /* Listen sockets */
        printf("\r\nSOCKET %d : LISTEN", s);
        user_state = USERNAME;
      }
      break;       
  }     /* End switch */
}     /* End TELNETS function */

void init_telopt(SOCKET s)
{
  sendIAC(s, DO, TN_ENVIRONMENT);
  sendIAC(s, WILL, TN_ECHO);      /* Negotiate ECHO option */
}

void sendIAC(SOCKET s, uint8 r1, uint8 r2) 
{
  switch(r1) {
    case WILL :
      printf("sent : will");      /* WILL command */
      break;
      
    case WONT :
      printf("sent : wont");      /* WONT command */
      break;
      
    case DO :
      printf("sent : do");      /*DO command */
      break;
      
    case DONT :
      printf("sent : dont");      /* DONT command */
      break;  
      
    case IAC :
      break;
  }
  
  if(r2 <= NOPTIONS) {
    printf("%s\r\n", tel_options[r2]);
  } else {
    printf("%u\r\n", r2);
  }
  
  sprintf(buf, "%c%c%c", IAC, r1, r2);  
  send(s, (uint8 const *)buf, strlen(buf), FALSE);      /* Send IAC, command and option to the client */    
}     /* End init_telopt function */

void tel_input(SOCKET s)
{
  uint8 c;
  uint8 input_command[] ="W5200>";
  while(1)
  {
    if(getSn_RX_RSR(s) == 0 ) break;      /* If there is no received data, break */
    if(recv(s, &c, 1) == 0) break;      /* If there the received data is 0, break */
    if(user_state == LOGOUT) break;     /* If the user's state is LOGOUT, break */     
    
    if(c != IAC) {      /* If the received data is not a control character */
      data_buf[buf_index++] = c;      /* Save the received data to data_buf */
      putchar(c);      
      
      if(user_state != PASSWORD) {  
        sprintf(buf, "%c", c);
        send(s, (uint8 const *)buf, strlen(buf), FALSE);
      }
      
      if(c == '\n') {     /* If receive an '\n' ASCII code */
        if(buf_index > 1) {
                    
          if(data_buf[buf_index-2] == '\r') {
            data_buf[buf_index-2] = '\0';
          } else {
            data_buf[buf_index-1] = '\0';
          }
          
          if(user_state != LOGIN) login(s); /* Call the login() to process login */
          else proc_command(s);     /* Process the received data */
          
          if(user_state == LOGIN) {           
            send(s, input_command, 6, FALSE);
          }
        } else {         
          send(s, input_command, 6, FALSE); 
        }
        buf_index = 0;
      }
      continue; 
    }    
      
    if(recv(s, &c, 1) == 0) break;     
    switch(c) {      /* If received an IAC character */
      case WILL :
        if(recv(s, &c, 1) == 0) break;
        willopt(s, c);      /* Call the willopt() to process WILL command */
        break;
      case WONT :
        if(recv(s, &c, 1) == 0) break;
        wontopt(s, c);      /* Call the wontopt() to process WONT command */
        break;
      case DO :
        if(recv(s, &c, 1) == 0) break;
        doopt(s, c);      /* Call the doopt() to process DO command */
        break;
      case DONT :
        if(recv(s, &c, 1) == 0) break;
        dontopt(c);     /* Call the dontopt() to process DONT command */
        break;
      case IAC :
        break;      
    }
    break;
  }
}     /* End tel_input function */

void willopt(SOCKET s, uint16 opt)
{
  int ack;
  printf("recv: will");
  if(opt <= NOPTIONS) {
    printf("%s\r\n", tel_options[opt]);
  } else {
    printf("%u\r\n", opt);
  }
  
  switch(opt) {
  case TN_TRANSMIT_BINARY :
  case TN_ECHO :
  case TN_SUPPRESS_GA :
    ack = DO;     /* If receive 'WILL' and it has TN_SUPPRESS_GA option, transmit 'DO' */
    break;
  default :
    ack = DONT;     /* Refuse other commands which not defined */	
  }
  sendIAC(s, ack, opt);
}     /* End willopt function */

void wontopt(SOCKET s, uint16 opt)
{
  printf("recv: wont");
  if(opt <= NOPTIONS) {
    printf("%s\r\n", tel_options[opt]);
  } else {
    printf("%u\r\n", opt);
  }
  
  switch(opt) {
  case TN_TRANSMIT_BINARY :
  case TN_ECHO :
  case TN_SUPPRESS_GA :     /* If receive WONT command with TN_SUPPRESS_GA option */
    if(remote[opt] == 0) {
      remote[opt] = 1;      /* Set the TN_SUPPRESS_GA option */
      sendIAC(s, DONT, opt);      /* Send DONT command with TN_SUPPRESS_GA */
    }
    break;
  }
}     /* End wontopt function */

void doopt(SOCKET s, uint16 opt)
{
  printf("recv: do ");
  if(opt <= NOPTIONS) {
    printf("%s\r\n", tel_options[opt]);
  } else {
    printf("%u\r\n", opt);
  }
  
  switch(opt) {
  case TN_SUPPRESS_GA :     /* If receive DO command with TN_SUPPRESS_GA option */
    sendIAC(s, WILL, opt);      /* Send WILL command with TN_SUPPRESS_GA */
    break;
  case TN_ECHO :      /* If receive DO command with TN_ECHO option */
    sprintf(buf, "WELCOME!\r\nID : ");
    send(s, (uint8 const *)buf, strlen(buf), FALSE);
    break;
  default :
    sendIAC(s, WONT, opt);
  }  
}     /* End doopt function */

void dontopt(uint16 opt)
{
  printf("recv: dont ");
  if(opt <= NOPTIONS) {
    printf("%s\r\n", tel_options[opt]);
  } else {
    printf("%u\r\n", opt);
  }
  
  switch(opt) {
  case TN_TRANSMIT_BINARY :
  case TN_ECHO :
  case TN_SUPPRESS_GA :     /* If receive DONT command with TN_SUPPRESS_GA option */
    if(remote[opt] == 0) {      /* Set the TN_SUPPRESS_GA option */
      remote[opt] = 1;     
    }
    break;
  }  
}     /* End dontopt function */
  
void proc_command(SOCKET s)
{  
  char **cmdp;
  char *cp;
  char *help = {"HELP: Show all available commands\
    \r\nGET LED: Show all LED status\
    \r\nLED3 ON/OFF: Turn ON/OFF the LED3\
    \r\nLED4 ON/OFF: Turn ON/OFF the LED4\
      \r\nEXIT: Exit from W5200 TELNET server\r\n"}; /* command HELP : Message */   
  
  for(cp = data_buf; *cp != '\0';  cp++){
    *cp = tolower(*cp);     /* Translate big letter to small letter */       
  }  
    
  if(*data_buf != '\0') {
    /* Find the input command in table; if it isn't there, return Syntax Error */
    for(cmdp = commands; *cmdp != NULL; cmdp++) {      
      if(strncmp(*cmdp, data_buf, strlen(*cmdp)) == 0) break;      
    }
    
    if(*cmdp == NULL) {
      printf("NULL command\r\n");
      sprintf(buf, "%s : BAD command\r\n", data_buf);
      send(s, (uint8 const *)buf, strlen(buf), FALSE);
      return;
    }
    
    switch(cmdp - commands) {
         
      case HELP_CMD :     /* Process HELP command */
        printf("HELP_CMD\r\n");
        sprintf(buf, help);
        send(s, (uint8 const *)buf, strlen(buf), FALSE);
        break;
      
      case GET_LED_CMD :      /* Process GET LED command */
        printf("GET_LED_CMD\r\n");      
        sprintf(buf, "LED%d is %s\r\n", 3, GPIO_ReadOutputDataBit(GPIOA, LED3) ? "OFF" : "ON");
        send(s, (uint8 const *)buf, strlen(buf), FALSE);
        sprintf(buf, "LED%d is %s\r\n", 4, GPIO_ReadOutputDataBit(GPIOA, LED4) ? "OFF" : "ON");
        send(s, (uint8 const *)buf, strlen(buf), FALSE);
        break;
        
      case LED3_ON_CMD :      /* Process LED3 ON command */      
        printf("LED3_ON_CMD\r\n");
        sprintf(buf, "Turn ON the LED3\r\n");
        send(s, (uint8 const *)buf, strlen(buf), FALSE);
        GPIO_ResetBits(GPIOA, LED3); // led3 on
        break;
        
      case LED4_ON_CMD :      /* Process LED4 ON command */
        printf("LED4_ON_CMD\r\n");
        sprintf(buf, "Turn ON the LED4\r\n");
        send(s, (uint8 const *)buf, strlen(buf), FALSE);
        GPIO_ResetBits(GPIOA, LED4); // led4 on
        break;
        
      case LED3_OFF_CMD :     /* Process LED3 OFF command */
        printf("LED3_OFF_CMD\r\n");
        sprintf(buf, "Turn OFF the LED3\r\n");
        send(s, (uint8 const *)buf, strlen(buf), FALSE);
        GPIO_SetBits(GPIOA, LED3); // led3 off      
        break;
      
      case LED4_OFF_CMD :     /* Process LED4 OFF command */
        printf("LED4_OFF_CMD\r\n");
        sprintf(buf, "Turn OFF the LED4\r\n");
        send(s, (uint8 const *)buf, strlen(buf), FALSE);
        GPIO_SetBits(GPIOA, LED4); // led4 off      
        break;
        
      case EXIT_CMD :     /* Process EXIT command */
        printf("EXIT command\r\n");
        sprintf(buf, "EXIT command\r\nGood bye!\r\nLogout from W5200 TELNET\r\n");
        send(s, (uint8 const *)buf, strlen(buf), FALSE);
        close(s); 
        user_state = LOGOUT;
        break;
      
      default :
        break;
    }
  }
}     /* End proc_command function */

void login(SOCKET s)
{  
  if(user_state == USERNAME) {       /* input the client ID and Password */
    strcpy((char *)user_name, data_buf);    
    sprintf(buf, "Password : ");
    send(s, (uint8 const *)buf, strlen(buf), FALSE);
    user_state = PASSWORD;
    return;
  } else if(user_state == PASSWORD) {
    strcpy((char *)user_password, data_buf);   
    
    /*Check the client ID and Password*/    
    if(!(strcmp((char const *)user_name, (char const *)telnet_ID)) && !(strcmp((char const *)user_password, (char const *)telnet_PW))) {
      sprintf(buf, "\r\n=======================");
      send(s, (uint8 const *)buf, strlen(buf), FALSE);
      sprintf(buf, "\r\nSuccessfully connected!\
        \r\nImplemented Commands : HELP, GET LED, LED3 ON/OFF, LED4 ON/OFF, EXIT\r\n");                      
      send(s, (uint8 const *)buf, strlen(buf), FALSE);
      sprintf(buf, "=======================\r\n");
      send(s, (uint8 const *)buf, strlen(buf), FALSE);
      user_state = LOGIN;
      return;
    } else {
      sprintf(buf, "\r\nID or Password incorrect!\r\n");  /* If the ID or Password incorrect, print error msg */
      send(s, (uint8 const *)buf, strlen(buf), FALSE);
      sprintf(buf, "ID : ");
      send(s, (uint8 const *)buf, strlen(buf), FALSE);
      user_state = USERNAME;
      return;      
    }
  }
}     /* End login function */
