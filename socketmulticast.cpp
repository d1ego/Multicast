#include "socketmulticast.h"

  SocketMulticast(int)
  {
    int reuse = 1;
    bzero((char *)&direccionForanea, sizeof(direccionForanea));
  	bzero((char *)&direccionLocal, sizeof(direccionLocal));
  	if ((s = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
  		perror("Socket creation failed");
  		exit(EXIT_FAILURE);
  	}

  	int reuse = 1;
  	if (setsockopt(s, SOL_SOCKET, SO_REUSEPORT, &reuse, sizeof(reuse)) == -1) {
  		printf("Error al llamar a la funciรณn setsockopt\n");
  		exit(0);
  	}

  	direccionLocal.sin_family = PF_INET;
  	direccionLocal.sin_addr.s_addr = INADDR_ANY;
  	direccionLocal.sin_port = htons(pto);
  	if (bind(s, (struct sockaddr *)&direccionLocal, sizeof(direccionLocal)) < 0)
  	{
  		perror("bind failed");
  		exit(EXIT_FAILURE);
  	}
  }
  ~SocketMulticast()
  {
    close(s);
  }
  int SocketMulticast::recibe(PaqueteDatagrama & p)
  {
    socklen_t clilen = sizeof(direccionForanea);
    int a = recvfrom(s, (char *) p.obtieneDatos(), p.obtieneLongitud(), 0, (struct sockaddr *) &direccionForanea, &clilen);
    p.inicializaIp(inet_ntoa(direccionForanea.sin_addr));
    p.inicializaPuerto(ntohs(direccionForanea.sin_port));
    return a;
  }
  int SocketMulticast::envia(PaqueteDatagrama & p, unsigned char ttl)
  {
    setsockopt(s, IPPROTO_IP, IP_MULTICAST_TTL, (void*)&ttl, sizeof(ttl));
    direccionForanea.sin_family = AF_INET;
  	direccionForanea.sin_addr.s_addr = inet_addr(p.obtieneDireccion());
  	direccionForanea.sin_port = htons(p.obtienePuerto());
    return sendto(s, (char *) p.obtieneDatos(), p.obtieneLongitud(), 0, (struct sockaddr *)&direccionForanea, sizeof(direccionForanea));

  }
  void SocketMulticast::unirseGrupo(char *multicastIP)
  {
    multicast.imr_multiaddr.s_addr = inet_addr(multicastIP);
    multicast.imr_interface.s_addr = htonl(INADDR_ANY);
    setsockopt(s, IPPROTO_IP, IP_ADD_MEMBERSHIP, (void*)&multicast, sizeof(multicast));
  }
  void SocketMulticast::salirseGrupo(char *multicastIP)
  {
    multicast.imr_multiaddr.s_addr = inet_addr(multicastIP);
    multicast.imr_interface.s_addr = htonl(INADDR_ANY);
    setsockopt(s, IPPROTO_IP, IP_DROP_MEMBERSHIP, (void*)&multicast, sizeof(multicast));
  }
