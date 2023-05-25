#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <strings.h>
#include <string.h>
#include <time.h>

#define PORT 3535
#define BACKLOG 32

int main(){

    short int opc;
    int serverfd, clientfd, r, opt = 1;
    struct sockaddr_in server, client; 
    socklen_t   tamano;
    char buffer[4];
    char bufferAVT[14];
    char buff[1];
    unsigned char buf[sizeof(struct in_addr)];
    char str[INET_ADDRSTRLEN];

    //Estructura para leer los datos del archivo binario
    typedef struct {
        short int sourceId,destinyId,hour,siguiente;
        double meanTravel;
    } travel;
    
    //Estructura para manejar la busqueda
    travel travelModel;

    //Estructura para guardar los datos recibidos
    travel travelModelShare;

    // Obtener el tiempo actual
    time_t t = time(NULL);
    struct tm tiempoLocal = *localtime(&t);

    // Preparacion para dar formato para la fecha y hora
    char fechaHora[70];
    char *formato = "%Y-%m-%d %H:%M:%S";
    char actTime[70];

    // Formatear la fecha
    int bytesEscritos = strftime(fechaHora, sizeof(fechaHora), formato, &tiempoLocal);

    // Verificar que se pudo dar formato
    if (bytesEscritos != 0) {
        //printf("[%s]", fechaHora);
        memcpy(actTime, fechaHora, sizeof(fechaHora));
        // actTime = fechaHora;

    } else {
        printf("Error formateando fecha");
    }

    // Creacion y comprobacion socket server
    serverfd = socket(AF_INET, SOCK_STREAM, 0);
    if(serverfd < 0){
        perror("Error en socket(): \n");
        exit(-1);
    }

    // Configuracion socket
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = INADDR_ANY;
    bzero(server.sin_zero, 8);
    
    setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR, (const char *)&opt, sizeof(int));

    // Creacion y comporbacion bind
    r = bind(serverfd, (struct sockaddr*)&server, sizeof(struct sockaddr));
    if(r < 0){
        perror("Error en bind(): \n");
        exit(-1);
    }

    // Creacion y comporbacion listen
    r = listen(serverfd, BACKLOG);
    if(r < 0){
        perror("Error en listen(): \n");
        exit(-1);
    }

    // Creacion y comporbacion accept
    clientfd = accept(serverfd, (struct sockaddr *)&client, &tamano);
    if(clientfd < 0){
        perror("Error en accept(): \n");
        exit(-1);
    }
    
    // Creacion y comporbacion send
    r = send(clientfd, "Conexion establecida con el servidor", 36, 0);
    if(r < 0){
        perror("Error en send(): \n");
        exit(-1);
    }

    
    // // Creacion y comprobacion recive
    // r = recv(clientfd, &buffer, 4, 0);
    // buffer[r] = 0;
    // travelModelShare.sourceId = (short)atoi(&buffer[0]);

    // // Creacion y comprobacion send
    // r = send(clientfd, "Dato recibido", 14, 0);
    // if(r < 0){
    //     perror("Error en send(): \n");
    //     exit(-1);
    // }

    // // Creacion y comprobacion recive
    // r = recv(clientfd, &buffer, 4, 0);
    // buffer[r] = 0;
    // travelModelShare.destinyId = (short)atoi(&buffer[0]);

    // // Creacion y comprobacion send
    // r = send(clientfd, "Dato recibido", 14, 0);
    // if(r < 0){
    //     perror("Error en send(): \n");
    //     exit(-1);
    // }

    // // Creacion y comporbacion recive
    // r = recv(clientfd, &buffer, 2, 0);
    // buffer[r] = 0;
    // travelModelShare.hour = (short)atoi(&buffer[0]);

    // //Creacion y comprobacion send
    // r = send(clientfd, "Dato recibido", 14, 0);
    // if(r < 0){
    //     perror("Error en send(): \n");
    //     exit(-1);
    // }
    
    int foundValue = 0;
    int newData = 0;
    
    // short int oldSource = -1,oldDestiny = -1,oldHour = -1;
    short int oldSource = 0,oldDestiny = 0,oldHour = 0;
        //Apertura archivo binario para leer
        FILE *fDataBin;
		fDataBin=fopen("tablaBinarita.bin","rb");
		if (!fDataBin)
		{
			printf("No se pudo abrir el archivo binario\n");
			return 1;
		}
                //98,6,7,3124.57,794.39,3014.25,1.32
        // El ciclo while espera siempre el cambio de un dato en memoria compartida
        while (1) {
            if ((travelModelShare.sourceId != oldSource) || (travelModelShare.destinyId != oldDestiny) || (travelModelShare.hour != oldHour)){
                foundValue = 0;
            }
            
            printf("Viene1\n");
            // Creacion y comprobacion recive
            r = recv(clientfd, &buffer, 4, 0);
            buffer[r] = 0;
            travelModelShare.sourceId = (short)atoi(&buffer[0]);
            // Creacion y comprobacion send
            r = send(clientfd, "Dato recibido", 14, 0);
            if(r < 0){
                perror("Error en send(): \n");
                exit(-1);
            }
            printf("Viene2\n");
            //1051,58,16,2389.58,312.71,2369.15,1.14
            // Creacion y comprobacion recive
            r = recv(clientfd, &buffer, 4, 0);
            buffer[r] = 0;
            travelModelShare.destinyId = (short)atoi(&buffer[0]);
            // Creacion y comprobacion send
            r = send(clientfd, "Dato recibido", 14, 0);
            if(r < 0){
                perror("Error en send(): \n");
                exit(-1);
            }
            printf("Viene3\n");
            // Creacion y comporbacion recive
            r = recv(clientfd, &buffer, 2, 0);
            buffer[r] = 0;
            travelModelShare.hour = (short)atoi(&buffer[0]);

            //Creacion y comprobacion send
            r = send(clientfd, "Dato recibido", 14, 0);
            if(r < 0){
                perror("Error en send(): \n");
                exit(-1);
            }
        //Buscar un cambio en la estructura travel que comparten o donde capturo los datos
            //printf("Dato memoria: %hd, %hd, %hd\n", shm_ptr->sourceId,shm_ptr->destinyId,shm_ptr->hour);
            //sleep(1);
            //Lectura con apuntadores del archivo binario y guardarlos en la estructura
            fseek(fDataBin, 0, SEEK_SET);
            fread(&travelModel.sourceId, sizeof(short int), 1, fDataBin);
            fread(&travelModel.destinyId, sizeof(short int), 1, fDataBin);
            fread(&travelModel.hour, sizeof(short int), 1, fDataBin);
            fread(&travelModel.siguiente, sizeof(short int), 1, fDataBin);
            fread(&travelModel.meanTravel, sizeof(double), 1, fDataBin);
            //Ciclo para leer tantas estructuras/lineas del archivo binario como la condición lo diga
            //Mas alla de 250000 la busqueda se realentiza mas de los 2 segundos
            //printf("%hd\n",foundValue);
            for (int i = 1; i <=250000 && (foundValue == 0 ); i++){
                newData = 0; 
                //Comprobar el origen Id
                //printf("Data%hd\n",i);
                // sleep(1);
                //printf("Dato es: %hd, %hd, %hd\n", travelModelShare.sourceId, travelModelShare.destinyId, travelModelShare.hour);
                if ((travelModelShare.sourceId != oldSource) || (travelModelShare.destinyId != oldDestiny) || (travelModelShare.hour != oldHour)){
                    //printf("Ojo dato nuevo\n");
                    i = -1;
                }
                if(travelModelShare.sourceId == travelModel.sourceId){
                    //printf("entro");
                    while(foundValue == 0 && travelModel.siguiente > 0){
                        //printf("Se queda");
                        // //Encontrar el primer destino Id y la hora
                        //printf("foundValue: %hd",foundValue);
                        //printf("siguiente: %hd\n",travelModel.siguiente);
                        // printf("Dato MEMORIA2: %hd, %hd, %hd, %.2f\n", shm_ptr->sourceId,shm_ptr->destinyId,shm_ptr->hour,shm_ptr->meanTravel);
                        // printf("Dato LEIDO: %hd, %hd, %hd, %hd, %.2f\n", travelModel.sourceId, travelModel.destinyId, travelModel.hour, travelModel.siguiente,travelModel.meanTravel);
                        //printf("Compa Leido vs Memoria: %hd,%hd\n",travelModel.destinyId,shm_ptr->destinyId);
                         //sleep(1);
                        if ((travelModelShare.sourceId != oldSource) || (travelModelShare.destinyId != oldDestiny) || (travelModelShare.hour != oldHour)){
                            // printf("Ojo dato nuevo\n");
                            // sleep(1);
                            fseek(fDataBin, 0, SEEK_SET);
                            fread(&travelModel.sourceId, sizeof(short int), 1, fDataBin);
                            fread(&travelModel.destinyId, sizeof(short int), 1, fDataBin);
                            fread(&travelModel.hour, sizeof(short int), 1, fDataBin);
                            fread(&travelModel.siguiente, sizeof(short int), 1, fDataBin);
                            fread(&travelModel.meanTravel, sizeof(double), 1, fDataBin);
                            i = -1;
                            break;
                        }
                        if(travelModelShare.destinyId == travelModel.destinyId && travelModelShare.hour == travelModel.hour){
                            //Se encontro la estructura y se asigna a la memoria compartida el tiempo medio de esa
                            //estructura
                            //printf("aca2: %hd\n",travelModel.siguiente);
                            // Creacion y comporbacion recive
                            
                            printf("Tiempo medio: %.2f\n",travelModel.meanTravel);
                            // Obtener el tiempo actual
                            time_t t = time(NULL);
                            struct tm tiempoLocal = *localtime(&t);
                            //780,407,21,1934.18,463.03,1881.99,1.26
                            // Preparacion para dar formato para la fecha y hora
                            char dataBusqueda[80] = "Fecha: ";
                            char dataTravel[20];
                            char fechaHora[70];
                            char *formato = "%Y-%m-%d %H:%M:%S";
                            char actTime[70];

                            // Formatear la fecha
                            int bytesEscritos = strftime(fechaHora, sizeof(fechaHora), formato, &tiempoLocal);

                            // Verificar que se pudo dar formato
                            if (bytesEscritos != 0) {
                                //printf("[%s]", fechaHora);
                                memcpy(actTime, fechaHora, sizeof(fechaHora));
                                // actTime = fechaHora;

                            } else {
                                printf("Error formateando fecha");
                            }
                            
                            // Obtener información del socket del cliente
                            struct sockaddr_in clientAddr;
                            socklen_t clientAddrLen = sizeof(clientAddr);
                            int miV = getpeername(clientfd, (struct sockaddr*)&clientAddr, &clientAddrLen);

                            // Convertir la dirección IP a una cadena legible
                            char clientIP[INET_ADDRSTRLEN];
                            inet_ntop(AF_INET, &(clientAddr.sin_addr), clientIP, INET_ADDRSTRLEN);

                            //Abrir el archivo Log para escribir en este
                            FILE* log = fopen("Log.txt", "a"); 

                            if (log == NULL) {
                                printf("No se pudo abrir el archivo.\n");
                                return 1;
                            }

                            strcat(dataBusqueda, fechaHora);
                            strcat(dataBusqueda, " Cliente: ");
                            strcat(dataBusqueda, clientIP);
                            strcat(dataBusqueda, " Busqueda: ");
                            sprintf(dataTravel,"%d",travelModel.sourceId);
                            strcat(dataBusqueda, dataTravel);
                            strcat(dataBusqueda, "-");
                            sprintf(dataTravel,"%d",travelModel.destinyId);
                            strcat(dataBusqueda, dataTravel);
                            strcat(dataBusqueda, "\n");
                            // Escribir en el archivo log la busqueda
                            fprintf(log, "%s",dataBusqueda);
                            // Cerrar el archivo
                            fclose(log);
                            // printf("Fecha y hora formateada: %s\n", dataBusqueda);
                            //Poner a dormir 1 segundo para que los procesos no lean al tiempo la memoria compartida
                            sleep(1);
                            foundValue = 1;
                            //usleep(500 * 1000);
                        }
                        else{
                            //Busca el siguiente valor igual al origen Id y sigue comprobando destino Id y hora
                            fseek(fDataBin, (travelModel.siguiente-1)*(4*sizeof(short int) + sizeof(double)), SEEK_SET);
                            fread(&travelModel.sourceId, sizeof(short int), 1, fDataBin);
                            fread(&travelModel.destinyId, sizeof(short int), 1, fDataBin);
                            fread(&travelModel.hour, sizeof(short int), 1, fDataBin);
                            fread(&travelModel.siguiente, sizeof(short int), 1, fDataBin);
                            fread(&travelModel.meanTravel, sizeof(double), 1, fDataBin);
                        }
                        if ((travelModelShare.sourceId != oldSource) || (travelModelShare.destinyId != oldDestiny) || (travelModelShare.hour != oldHour)){
                            newData = 1;
                            i = -1;
                        }
                    }               
                }
                //Buscar en la siguiente linea/estructura el destino Id
                else{
                    fseek(fDataBin, i*(4*sizeof(short int) + sizeof(double)), SEEK_SET);
                    fread(&travelModel.sourceId, sizeof(short int), 1, fDataBin);
                    fread(&travelModel.destinyId, sizeof(short int), 1, fDataBin);
                    fread(&travelModel.hour, sizeof(short int), 1, fDataBin);
                    fread(&travelModel.siguiente, sizeof(short int), 1, fDataBin);
                    fread(&travelModel.meanTravel, sizeof(double), 1, fDataBin);
                    // printf("aca: %li\n",(travelModel.siguiente-1)*(4*sizeof(short int) + sizeof(double)));
                    // sleep(3);
                }
            //Actualizar el valor viejo para comprobar cambios en la memoria compartida
                oldSource = travelModelShare.sourceId;
                oldDestiny = travelModelShare.destinyId;
                oldHour = travelModelShare.hour;
            //Poner a dormir 1 segundo para que los procesos no lean al tiempo la memoria compartida
                // sleep(1);
            //usleep(500 * 1000);
            }
            if(foundValue == 1){
                printf("Viene4\n");
                r = recv(clientfd, &buffer, 2, 0);
                buffer[r] = 0;
                opc = (short)atoi(&buffer[0]);
                r = send(clientfd, &travelModel.meanTravel, sizeof(travelModel.meanTravel), 0);
                if(r < 0){
                    perror("Error en send(): \n");
                    exit(-1);
                }
                foundValue = 0;
            }
        //printf("La línea 5 es: %hd, %hd, %hd, %.2ld, %.2f\n", travelModel.sourceId, travelModel.destinyId, travelModel.hour, 2*(travelModel.siguiente-1)*(4*sizeof(short int) + sizeof(double)),travelModel.meanTravel);
        }
    fclose(fDataBin);
    return (0);
}