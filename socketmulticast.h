#ifndef SOCKETMULTICAST_H
#define SOCKETMULTICAST_H

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
