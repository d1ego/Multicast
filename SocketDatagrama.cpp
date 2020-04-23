#include "SocketDatagrama.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>

SocketDatagrama::SocketDatagrama(int _puerto){
	s = socket(AF_INET, SOCK_DGRAM, 0);
	bzero((char *)&direccionLocal, sizeof(direccionLocal));
	bzero((char *)&direccionForanea, sizeof(direccionForanea));

	direccionLocal.sin_family = AF_INET;
	direccionLocal.sin_addr.s_addr = INADDR_ANY;
		direccionLocal.sin_port = htons(_puerto);
	bind(s,(struct sockaddr *) &direccionLocal, sizeof(direccionLocal));
}

SocketDatagrama::~SocketDatagrama() {
    close(s);//cerrar socket
}

int SocketDatagrama::recibe(PaqueteDatagrama &p) {
  unsigned int clilen = sizeof(direccionForanea);
	int a = recvfrom(s, (char *)p.obtieneDatos(), p.obtieneLongitud(), 0, (struct sockaddr *) &direccionForanea, &clilen);
	p.inicializaIp(inet_ntoa(direccionForanea.sin_addr));
	p.inicializaPuerto(ntohs(direccionForanea.sin_port));

	return a;
  }

int SocketDatagrama::envia(PaqueteDatagrama &p) {
  direccionForanea.sin_family = AF_INET;
	direccionForanea.sin_addr.s_addr = inet_addr(p.obtieneDireccion());
	direccionForanea.sin_port = htons(p.obtienePuerto());

	return sendto(s, (char *) p.obtieneDatos(), p.obtieneLongitud(), 0, (struct sockaddr *)&direccionForanea, sizeof(direccionForanea));

}
