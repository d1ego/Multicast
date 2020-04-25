#include "SocketMulticast.h"

SocketMulticast::SocketMulticast(int puerto)
{
    s = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    int reuse = 1;
    if (setsockopt(s, SOL_SOCKET, SO_REUSEPORT, &reuse, sizeof(reuse)) == -1)
    {
        printf("Error al llamar a la función setsockopt\n");
        exit(0);
    }
    sockaddr_in direccionLocal;
    int clilen = sizeof(direccionLocal);
    bzero(&direccionLocal, clilen);
    direccionLocal.sin_family = AF_INET;
    direccionLocal.sin_addr.s_addr = INADDR_ANY;
    direccionLocal.sin_port = htons(puerto);
    bind(s, (sockaddr *)&direccionLocal, clilen);
}
SocketMulticast::~SocketMulticast()
{
    close(s);
}
int SocketMulticast::recibe(PaqueteDatagrama &p)
{
    // Recibe datos.
    sockaddr_in direccionForanea;
    int clilen = sizeof(direccionForanea);
    int recibidos = recvfrom(s,p.obtieneDatos(),p.obtieneLongitud(),0,(struct sockaddr *)&direccionForanea,(socklen_t *)&clilen);
    p.inicializaIp(inet_ntoa(direccionForanea.sin_addr));
    p.inicializaPuerto(ntohs(direccionForanea.sin_port));
    return recibidos;
}
int SocketMulticast::envia(PaqueteDatagrama &p, unsigned char ttl)
{
    unsigned char TTL = ttl;
    setsockopt(s, IPPROTO_IP, IP_MULTICAST_TTL, (void *)&TTL, sizeof(TTL));
    sockaddr_in direccionForanea;
    int clilen = sizeof(direccionForanea);
    bzero(&direccionForanea, clilen);
    direccionForanea.sin_family = AF_INET;
    direccionForanea.sin_addr.s_addr = inet_addr(p.obtieneDireccion());
    direccionForanea.sin_port = htons(p.obtienePuerto());
    return sendto(s, p.obtieneDatos(), p.obtieneLongitud(), 0, (sockaddr *)&direccionForanea, sizeof(direccionForanea));
}
void SocketMulticast::unirseGrupo(const char *multicastIP)
{
    ip_mreq multicast;
    multicast.imr_multiaddr.s_addr = inet_addr(multicastIP);
    multicast.imr_interface.s_addr = INADDR_ANY;
    setsockopt(s, IPPROTO_IP, IP_ADD_MEMBERSHIP, (void *)&multicast, sizeof(multicast));
}
void SocketMulticast::salirseGrupo(const char *multicastIP)
{
    ip_mreq multicast;
    multicast.imr_multiaddr.s_addr = inet_addr(multicastIP);
    multicast.imr_interface.s_addr = INADDR_ANY;
    setsockopt(s, IPPROTO_IP, IP_DROP_MEMBERSHIP, (void *)&multicast, sizeof(multicast));
}
