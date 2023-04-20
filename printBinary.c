#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>

int main(){
    
    typedef struct {
        short int sourceId,destinyId,hour,siguiente;
        double meanTravel;
    } travel;

    key_t key = 1234;
    int shm_id;
    travel *shm_ptr;

    // Crea la memoria compartida
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
    /*Declaracion de una estrucura para guardar los datos en esta e ir agregandolo a la table
    e inicializamos la posicion de inicio*/
    travel travelModel;
    short int oldSource = -1,oldDestiny = -1,oldHour = -1;

        FILE *fDataBin;

		fDataBin=fopen("b.bin","rb");
		if (!fDataBin)
		{
			printf("No se pudo abrir el archivo binario\n");
			return 1;
		}
        

        // Esperar siempre el cambio de un dato en memoria compartida
        while (1) {
            
            if ((shm_ptr->sourceId != oldSource) || (shm_ptr->destinyId != oldDestiny) || (shm_ptr->hour != oldHour)) {
                
                short int sId = shm_ptr->sourceId;
                short int dId = shm_ptr->destinyId;
                short int h = shm_ptr->hour;
                fseek(fDataBin, 0, SEEK_SET);
                fread(&travelModel.sourceId, sizeof(short int), 1, fDataBin);
                fread(&travelModel.destinyId, sizeof(short int), 1, fDataBin);
                fread(&travelModel.hour, sizeof(short int), 1, fDataBin);
                fread(&travelModel.siguiente, sizeof(short int), 1, fDataBin);
                fread(&travelModel.meanTravel, sizeof(double), 1, fDataBin);
                for (int i = 1; i <=100; i++){
                    if(sId == travelModel.sourceId){
                        if(dId == travelModel.destinyId && h == travelModel.hour){
                            shm_ptr->meanTravel = travelModel.meanTravel;
                            break;
                        }
                        else{
                            shm_ptr->meanTravel = 0;
                            fseek(fDataBin, (travelModel.siguiente-1)*(4*sizeof(short int) + sizeof(double)), SEEK_SET);
                            fread(&travelModel.sourceId, sizeof(short int), 1, fDataBin);
                            fread(&travelModel.destinyId, sizeof(short int), 1, fDataBin);
                            fread(&travelModel.hour, sizeof(short int), 1, fDataBin);
                            fread(&travelModel.siguiente, sizeof(short int), 1, fDataBin);
                            fread(&travelModel.meanTravel, sizeof(double), 1, fDataBin);
                        }                
                    }
                    else{
                        fseek(fDataBin, i*(4*sizeof(short int) + sizeof(double)), SEEK_SET);
                        fread(&travelModel.sourceId, sizeof(short int), 1, fDataBin);
                        fread(&travelModel.destinyId, sizeof(short int), 1, fDataBin);
                        fread(&travelModel.hour, sizeof(short int), 1, fDataBin);
                        fread(&travelModel.siguiente, sizeof(short int), 1, fDataBin);
                        fread(&travelModel.meanTravel, sizeof(double), 1, fDataBin);
                    }
                }
            // printf("La línea 5 es: %hd, %hd, %hd, %hd, %.2f\n", sourceId, destinyId, hour, siguiente,meanTravel);
            }
        oldSource = shm_ptr->sourceId;
        }
    fclose(fDataBin);
    return (0);
}
          