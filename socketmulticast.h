

#ifndef SOCKETMULTICAST_H
#define SOCKETMULTICAST_H
#include "PaqueteDatagrama.h"
#include <arpa/inet.h>
#include <strings.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>


class SocketMulticast{
  public:
    SocketMulticast(int);
    ~SocketMulticast();
    int recibe(PaqueteDatagrama);
    int envia(PaqueteDatagrama, unsigned char); //Se une a un grupo multicast, recibe la IP multicast
    void unirseGrupo(char *);
    //Se sale de un grupo multicast, recibe la IP multicast
    void salirseGrupo(char *);
  private:
     int s; //ID socket
 };
#endif
