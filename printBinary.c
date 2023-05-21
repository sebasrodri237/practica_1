#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <unistd.h>


int main(){
    //Estrucutra para leer los datos del archivo binario
    typedef struct {
        short int sourceId,destinyId,hour,siguiente;
        double meanTravel;
    } travel;
    
    //Llave/clave memoria compartida
    key_t key = 1234;
    int shm_id;//identificador de la memoria compartida
    travel *shm_ptr;// apuntador a la memoria compartida

    // Crea la memoria compartida, con la llave, un tamaño y los permisos, luego se asigna al identificador
    shm_id = shmget(key, 1024, IPC_CREAT | 0666);
    if (shm_id == -1) {
        perror("Error en shmget");
        exit(1);
    }

    // Vincula la memoria compartida a una estructura de datos
    shm_ptr = (travel*) shmat(shm_id, NULL, 0);
    if (shm_ptr == (travel*) -1) {
        perror("Error en shmat");
        exit(1);
    }
    int foundValue = 0;
    int newData = 0;
    /*Declaracion de una estrucura para guardar los datos en esta e ir agregandolo a la table
    e inicializamos unas variables para detectar el cambio en la memoria compartida*/
    travel travelModel;
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

        // El ciclo while espera siempre el cambio de un dato en memoria compartida
        while (1) {
            if ((shm_ptr->sourceId != oldSource) || (shm_ptr->destinyId != oldDestiny) || (shm_ptr->hour != oldHour)){
                foundValue = 0;
            }
        //Buscar un cambio en la memoria compartida
            printf("Dato memoria: %hd, %hd, %hd\n", shm_ptr->sourceId,shm_ptr->destinyId,shm_ptr->hour);
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
            for (int i = 1; i <=250000 && (foundValue == 0 ); i++){
                newData = 0; 
                //Comprobar el origen Id
                // printf("Data%hd\n",i);
                // sleep(1);
                //printf("Dato es: %hd, %hd, %hd, %hd, %.2f\n", travelModel.sourceId, travelModel.destinyId, travelModel.hour, travelModel.siguiente,travelModel.meanTravel);
                if ((shm_ptr->sourceId != oldSource) || (shm_ptr->destinyId != oldDestiny) || (shm_ptr->hour != oldHour)){
                    //printf("Ojo dato nuevo\n");
                    i = -1;
                }
                if(shm_ptr->sourceId == travelModel.sourceId){
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
                        if ((shm_ptr->sourceId != oldSource) || (shm_ptr->destinyId != oldDestiny) || (shm_ptr->hour != oldHour)){
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
                        if(shm_ptr->destinyId == travelModel.destinyId && shm_ptr->hour == travelModel.hour){
                            //Se encontro la estructura y se asigna a la memoria compartida el tiempo medio de esa
                            //estructura
                            //printf("aca2: %hd\n",travelModel.siguiente);
                            shm_ptr->meanTravel = travelModel.meanTravel;
                            printf("Tiempo medio: %.2f\n",shm_ptr->meanTravel);
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
                        if ((shm_ptr->sourceId != oldSource) || (shm_ptr->destinyId != oldDestiny) || (shm_ptr->hour != oldHour)){
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
                oldSource = shm_ptr->sourceId;
                oldDestiny = shm_ptr->destinyId;
                oldHour = shm_ptr->hour;
            //Poner a dormir 1 segundo para que los procesos no lean al tiempo la memoria compartida
                // sleep(1);
            //usleep(500 * 1000);
            }
        //printf("La línea 5 es: %hd, %hd, %hd, %.2ld, %.2f\n", travelModel.sourceId, travelModel.destinyId, travelModel.hour, 2*(travelModel.siguiente-1)*(4*sizeof(short int) + sizeof(double)),travelModel.meanTravel);
        }
    fclose(fDataBin);
    return (0);
}