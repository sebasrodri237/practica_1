#include<stdio.h>
#include<stdlib.h>

int main(){
    
    int buff[8];

    struct travel{
    /*Posiciones de la estructura para ir recorriendo datos y guardarlos con una posicion*/
    int actualPosition,nextPosition;
    short int sourceId,destinyId,hour;
    float meanTravel;
    };

    /*Declaracion de una estrucura para guardar los datos en esta e ir agregandolo a la table
    e inicializamos la posicion de inicio*/
    struct travel travelModel;
    
        FILE *ptr_myfile;

		ptr_myfile=fopen("dataBinario.bin","rb");
		if (!ptr_myfile)
		{
			printf("Unable to open file!");
			return 1;
		}

        while (fread(&travelModel, sizeof(travelModel), 1, ptr_myfile) == 1) {
            printf("%hd,%hd,%hd,%f\n",travelModel.sourceId,travelModel.destinyId,travelModel.hour,travelModel.meanTravel);
        }

        return (0);
}
        