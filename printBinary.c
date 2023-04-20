#include<stdio.h>
#include<stdlib.h>


int main(){
    
    struct travel{
        /*Posiciones de la estructura para ir recorriendo datos y guardarlos con una posicion*/
        short int sourceId,destinyId,hour,siguiente;
        double meanTravel;
    };

    /*Declaracion de una estrucura para guardar los datos en esta e ir agregandolo a la table
    e inicializamos la posicion de inicio*/
    struct travel travelModel;
    short int sId, dId, h;

    
        printf("Introduzca un numero entero: ");
        scanf("%hd %hd %hd", &sId, &dId, &h);

        FILE *fDataBin;

		fDataBin=fopen("a.bin","rb");
		if (!fDataBin)
		{
			printf("No se pudo abrir el archivo binario\n");
			return 1;
		}
        fseek(fDataBin, 0, SEEK_SET);
        fread(&travelModel.sourceId, sizeof(short int), 1, fDataBin);
        fread(&travelModel.destinyId, sizeof(short int), 1, fDataBin);
        fread(&travelModel.hour, sizeof(short int), 1, fDataBin);
        fread(&travelModel.siguiente, sizeof(short int), 1, fDataBin);
        fread(&travelModel.meanTravel, sizeof(double), 1, fDataBin);
        for (int i = 1; i <=100; i++)
        {   
            if(sId == travelModel.sourceId){
                if(dId == travelModel.destinyId && h == travelModel.hour){
                    printf("El tiempo de viaje promedio es: %.2f\n",travelModel.meanTravel);
                    break;
                }
                else{
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

        fclose(fDataBin);
        return (0);
}
        