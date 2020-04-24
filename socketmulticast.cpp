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
    int len = sizeof(direccionLocal);
    bzero(&direccionLocal, len);
    direccionLocal.sin_family = AF_INET;
    direccionLocal.sin_addr.s_addr = INADDR_ANY;
    direccionLocal.sin_port = htons(puerto);

    bind(s, (sockaddr *)&direccionLocal, len);
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

    // Guarda direccción fuente.
    //uint32_t direccionFuente = ntohl(direccionForanea.sin_addr.s_addr);
    //char direccionFuenteCadena[16];
    //sprintf(direccionFuenteCadena, "%u.%u.%u.%u",direccionFuente >> 24 & 0xff,direccionFuente >> 16 & 0xff,direccionFuente >> 8 & 0xff,direccionFuente & 0xff);
    // p.inicializaIp(direccionFuenteCadena);
    //
    // // Guarda puerto fuente.
    // in_port_t puertoFuente = ntohs(direccionForanea.sin_port);
    // p.inicializaPuerto(puertoFuente);
    p.inicializaIp(inet_ntoa(direccionForanea.sin_addr));
  	p.inicializaPuerto(ntohs(direccionForanea.sin_port));
    return recibidos;
}

int SocketMulticast::envia(PaqueteDatagrama &p, unsigned char ttl)
{
    unsigned char TTL = ttl;
    setsockopt(s, IPPROTO_IP, IP_MULTICAST_TTL, (void *)&TTL, sizeof(TTL));

    sockaddr_in direccionForanea;
    int len = sizeof(direccionForanea);
    bzero(&direccionForanea, len);
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
