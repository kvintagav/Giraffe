#ifndef _LOOPBACK_H
#define _LOOPBACK_H

#include "types.h"

void loopback_tcps(SOCKET s, uint16 port);
void loopback_tcpc(SOCKET s, uint16 port);
void loopback_udp(SOCKET s, uint16 port);
void sender_tcps(SOCKET s, uint16 port);
void sender_tcpc(SOCKET s, uint16 port);

#endif
