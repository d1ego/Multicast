#ifndef SOCKETMULTICAST_H
#define SOCKETMULTICAST_H

#include <arpa/inet.h>
#include <strings.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

#include "PaqueteDatagrama.h"

class SocketMulticast{
public:
SocketMulticast(int);
~SocketMulticast();
int recibe(PaqueteDatagrama & p);
int envia(PaqueteDatagrama & p, unsigned char ttl); //Se une a un grupo multicast, recibe la IP multicast
void unirseGrupo(const char *);
//Se sale de un grupo multicast, recibe la IP multicast
void salirseGrupo(const char *);
private:
   int s; //ID socket
};

#endif
