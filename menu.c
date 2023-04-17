#include <stdio.h>

int main()
{
    int opcion,id_origen,id_destino,hora,tiempo_m;

    do
    {
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
                    scanf( "%d", &id_origen );
                    break;
            case 2: printf( "Ingrese ID destino (1-1160): \n");
                    scanf( "%d", &id_destino );
                    break;
            case 3: printf( "Ingrese hora del día (0-23): \n");
                    scanf( "%d", &hora );
                    break;
            case 4: printf( "Tiempo de viaje medio: %d.\n", tiempo_m);
                    break;
         }

         /* Fin del anidamiento */

    } while ( opcion != 5 );

    return 0;
}