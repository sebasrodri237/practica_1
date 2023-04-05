#include<stdio.h>
#include<stdlib.h>

int main(){
    
    /*Estructura de un viaje, según los datos necesarios de
    id de ida y destino, hora y tiempo medio*/
    struct travel{
    /*Posiciones de la estructura para ir recorriendo datos y guardarlos con una posicion*/
    int actualPosition,nextPosition;
    short int sourceId,destinyId,hour;
    float meanTravel;
    };

    /*Declaracion de una estrucura para guardar los datos en esta e ir agregandolo a la table
    e inicializamos la posicion de inicio*/
    struct travel travelModel;
    travelModel.nextPosition = 0;

    /*Se crea un apuntador al fichero(archivo) que se quiere abir e indicamos
    el modo, en este caso lectura*/
    FILE *fData;
    fData = fopen("dataLimpio.csv","r");
    if(fData==NULL){
        printf("Error al abrir el archivo de datos de viaje\n");
        exit(0);
    }
    
   /*Se crea un apuntador al fichero(archivo) binario que tendra la información de los viajes
    en una estructura*/
    FILE *fDataSave;
    fDataSave = fopen("dataBinario.bin","w+b");
    if(fDataSave==NULL){
        printf("Error al abrir el archivo binario\n");
        exit(0);
    }
    
    /*Toma los datos del archivo csv que abrimos antes y los va pasando hacia el archivo binario
    dandoles una posicion y guardandolos en forma de estructura.*/

    for(int i=1;1;i++){
        
        int dataRow = fscanf(fData,"%hd,%hd,%hd,%f,%*s",&travelModel.sourceId,&travelModel.destinyId,&travelModel.hour,&travelModel.meanTravel);
        travelModel.actualPosition=i;
        if(dataRow==EOF){
            printf("Lectura finalizada y datos guardados\n");
            break;
        }

        fwrite(&travelModel,sizeof(struct travel),1,fDataSave);
    }
    /*Cierre de los archivos abiertos anteriormente*/
    fclose(fData);
    fclose(fDataSave);

    return 0;
}