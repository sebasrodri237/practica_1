#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>

/* En este punto se debe realizar la comunicacion con la busqueda. Sea con tuberias o con memoria compartida */
typedef struct {
    short int sourceId,destinyId,hour;
    double meanTravel;
    } travel;

int main()
{
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

    travel datos;
    
    short int opcion;

    do
    {
        printf( "Bienvenido\n");
        printf( "1. Ingresar origen\n");
        printf( "2. Ingresar destino\n");
        printf( "3. Ingresar hora\n" );
        printf( "4. Buscar tiempo de viaje medio\n");
        printf( "5. Salir.\n");

        scanf( "%hd", &opcion );

        /* Inicio del anidamiento */

        switch ( opcion )
        {
            case 1: printf( "Ingrese ID origen (1-1160): \n");
                    scanf( "%hd", &datos.sourceId );
                    memcpy(shm_ptr, &datos, sizeof(travel));
                    break;
            case 2: printf( "Ingrese ID destino (1-1160): \n");
                    scanf( "%hd", &datos.destinyId );
                    memcpy(shm_ptr, &datos, sizeof(travel));
                    break;
            case 3: printf( "Ingrese hora del día (0-23): \n");
                    scanf( "%hd", &datos.hour );
                    memcpy(shm_ptr, &datos, sizeof(travel));
                    break;
            case 4: printf( "Tiempo de viaje medio: %2f.\n", shm_ptr->meanTravel);
                    break;
         }

         /* Fin del anidamiento */

    } while ( opcion != 5 );

    // Desvincula la memoria compartida
    shmdt(shm_ptr);

    // Elimina la memoria compartida
    shmctl(shm_id, IPC_RMID, NULL);

    return 0;
}