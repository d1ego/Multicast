#include "SocketMulticast.h"
#include "SocketDatagrama.h"

int main(int argc, char const *argv[])
{

    if (argc != 3)
    {
        printf("Uso: ./%s [DIRECCIÓN MULTICAST] [PUERTO DE ESCUCHA]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    // Extracción de parámetros.

    char direccionMulticast[16];
    sprintf(direccionMulticast, "%s", argv[1]);

    int puertoRecepcion = atoi(argv[2]);

    // Se crea socket y se une a grupo.
    SocketMulticast socket(puertoRecepcion);
    socket.unirseGrupo(direccionMulticast);

    // Se prepara un paquete para recibir y se imprime origen.
    PaqueteDatagrama p(MAX_LONGITUD_DATOS);
    if (socket.recibe(p) < 0) {
        printf("Error al recibir paquete\n");
        exit(1);
    }

    char dirFuente[16];
    sprintf(dirFuente, "%s", p.obtieneDireccion());

    printf("\nSe recibió un paquete en el grupo.\n");
    printf("\tOrigen: %s:%d\n", dirFuente, p.obtienePuerto());
    printf("\tContenido: %s.\n", p.obtieneDatos());



    socket.salirseGrupo(direccionMulticast);


    // Se abre socket.
    SocketDatagrama socketUnicast(7000);

    // Se genera paquete y se envía.

    PaqueteDatagrama pUnicast("Si recibi dato \n", strlen("Si recibi dato \n"), dirFuente, 6000);


    socketUnicast.envia(pUnicast);
    sleep(1);



    return 0;
}
