#include<stdio.h>
#include<stdlib.h>

int main(){
    
    int count = 0;
    struct travel{
    /*Posiciones de la estructura para ir recorriendo datos y guardarlos con una posicion*/
    int actualPosition,nextPosition;
    short int sourceId,destinyId,hour;
    float meanTravel;
    };

    /*Declaracion de una estrucura para guardar los datos en esta e ir agregandolo a la table
    e inicializamos la posicion de inicio*/
    struct travel travelModel;
    
        FILE *fDataBin;

		fDataBin=fopen("dataBinario.bin","rb");
		if (!fDataBin)
		{
			printf("No se pudo abrir el archivo binario");
			return 1;
		}

        while (fread(&travelModel, sizeof(travelModel), 1, fDataBin) == 1 && count != 100) {
            printf("%hd,%hd\n",travelModel.sourceId,travelModel.destinyId);
            count = count + 1;
        }
        fclose(fDataBin);
        return (0);
}
        