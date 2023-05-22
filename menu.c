#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>



#define PORT 3535
#define SERVER_ADDRESS  "127.0.0.1"
/* En este punto se debe realizar la comunicacion con la busqueda. Sea con tuberias o con memoria compartida */
typedef struct {
    short int sourceId,destinyId,hour,siguiente;
    double meanTravel;
    } travel;

int main()
{   
    int clientfd, serverfd, r;
    struct sockaddr_in client;
    struct hostent *he;
    char buffer[36];
    char str[10];
    travel travelModelShare2;
    double meanTravel;

    // Creacion y comprobacion socket cliente
    clientfd = socket(AF_INET, SOCK_STREAM, 0);
    if(clientfd < 0){
        perror("Error en socket(): \n");
        exit(-1);
    }

    // Configuracion del socket client
    client.sin_family = AF_INET;
    client.sin_port = htons(PORT);

    //  *   Ingreso de direccion servidor por consola
    // inet_aton(argv[1], &client.sin_addr);

    // Ingreso direccion servidor por constante
    inet_aton(SERVER_ADDRESS, &client.sin_addr);


    // Creacion y comporbacion connect
    r = connect(clientfd, (struct sockaddr *)&client, (socklen_t)sizeof(struct sockaddr));
    if(r < 0){
        perror("Error al configurar el socket (bind)");
        exit(-1);
    }    

    // Creacion y comporbacion recive
    r = recv(clientfd, buffer, 36, 0);
    buffer[r] = 0;

    // Impresion del mensaje recivido
    printf("\nMensaje Servidor: %s\n", buffer);

    
    int opcion;
    //Menu principal
    while(opcion!=5){
        printf( "Bienvenido\n");
        printf( "1. Ingresar origen\n");
        printf( "2. Ingresar destino\n");
        printf( "3. Ingresar hora\n" );
        printf( "4. Buscar tiempo de viaje medio\n");
        printf( "5. Salir.\n");

        scanf( "%d", &opcion );

        /* Inicio del anidamiento */

        switch ( opcion )
        {
            case 1: printf( "Ingrese ID origen (1-1160): \n");
                    scanf( "%hd", &travelModelShare2.sourceId);
                    sprintf(str, "%d", travelModelShare2.sourceId);
                    // Creacion y comprobacion send
                    r = send(clientfd, &str, 4, 0);
                    if(r < 0){
                        perror("Error en send(): \n");
                        exit(-1);
                    }
                    // Creacion y comporbacion recive
                    r = recv(clientfd, buffer, 48, 0);
                    if (r > 0)
                    {
                        buffer[r] = 0;
                        
                        // Impresion del mensaje recivido
                        printf("\nMensaje Servidor: %s\n", buffer);
                    } else
                    {
                        perror("Error al recibir el dato");
                    }
                    break;
            case 2: printf( "Ingrese ID destino (1-1160): \n");
                    scanf( "%hd", &travelModelShare2.destinyId );
                    sprintf(str, "%d", travelModelShare2.destinyId);
                    // Creacion y comprobacion send
                    r = send(clientfd, &str, 4, 0);
                    if(r < 0){
                        perror("Error en send(): \n");
                        exit(-1);
                    }
                    // Creacion y comporbacion recive
                    r = recv(clientfd, buffer, 48, 0);
                    if (r > 0)
                    {
                        buffer[r] = 0;
                        
                        // Impresion del mensaje recivido
                        printf("\nMensaje Servidor: %s\n", buffer);
                    } else
                    {
                        perror("Error al recibir el dato");
                    }
                    break;
            case 3: printf( "Ingrese hora del día (0-23): \n");
                    scanf( "%hd", &travelModelShare2.hour);
                    sprintf(str, "%d", travelModelShare2.hour);
                    // Creacion y comprobacion send
                    r = send(clientfd, &str, 4, 0);
                    if(r < 0){
                        perror("Error en send(): \n");
                        exit(-1);
                    }
                    // Creacion y comporbacion recive
                    r = recv(clientfd, buffer, 48, 0);
                    if (r > 0)
                    {
                        buffer[r] = 0;
                        
                        // Impresion del mensaje recivido
                        printf("\nMensaje Servidor: %s\n", buffer);
                    } else
                    {
                        perror("Error al recibir el dato");
                    }
                    break;
            case 4: 
                r = send(clientfd, "Solicitud AVT", 14, 0);
                if(r < 0){
                    perror("Error en send(): \n");
                    exit(-1);
                }

                 // Creacion y comprobacion recive
                r = recv(clientfd, &meanTravel, sizeof(meanTravel), 0);
                if (r > 0)
                {
                    buffer[r] = 0;
                    //r1.mean = atof(&buffer[0]);
                    //  *   Impresion del mensaje recibido
                    printf("\nEl tiempo medio es: %0.2f\n", meanTravel);
                } else
                {
                    perror("Error al recibir el dato");
                }
                    break;
            case 5:
				printf("Cerrando Conexion\n");
                r = send(clientfd, "", 1, 0);
                if(r < 0){
                    perror("Error en send(): \n");
                    exit(-1);
                }
                r = recv(clientfd, &buffer, 12, 0);
                if (r > 0)
                {
                    buffer[r] = 0;
                    printf("\nMensaje del servidor: %s\n", buffer);
                    exit(0);
                } else
                {
                    perror("Error al recivir el dato");
                }
				break;
         }

         /* Fin del anidamiento */

    }

    return 0;
}