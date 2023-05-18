#include <stdio.h>
#include <stdlib.h>
struct travel{
    /*Posiciones de la estructura para ir recorriendo datos y guardarlos con una posicion*/
    short int sourceId,destinyId,hour,siguiente;
    double meanTravel;
};
//Función para buscar el siguiente igual a un valor de origen ID
short int buscarSiguiente(int posCurrent,int sourceId){
    FILE *p = fopen("dataLimpio.csv", "r");
    short int or,des,h,siguiente = 0;
    double tm;
    int flag = 0;
    for(int i=posCurrent;i<=1000000;i++){
        int dataRow = fscanf(p,"%hd,%hd,%hd,%le,%*s",&or,&des,&h,&tm);
        if(dataRow==EOF){
            fclose(p);
            return 0;
        }
        if(sourceId == or){
            siguiente = i;
            fclose(p);
            return siguiente;
        }
    }
    fclose(p);
    return 0;
}
int main() {
    
    short int origen, destino, hora, siguiente = 0;
    double tiempo_medio;
    int size = 100;
    char *linea; 
    //Uso de memoria dinamica para captar la linea entrada del archivo csv
    linea = (char*) malloc(size * sizeof(char));
    if (linea == NULL) {
        printf("No se pudo asignar la memoria\n");
        return 1;
    }
    //Estructura para guardar los datos leidos
    struct travel travelModel;
    //Apertura archivo para leer y archivo binario a escribir
    FILE *input_file = fopen("dataLimpio.csv", "r");
    FILE *output_file = fopen("b.bin", "wb");
    //Ciclo para leer lineas del archivo, en este caso, lee 1000, para leer mas simplemente cambiar la condición
    for(int i=1;i<=10000;i++){
        int dataRow = fscanf(input_file,"%hd,%hd,%hd,%lf,%*s",&travelModel.sourceId,&travelModel.destinyId,&travelModel.hour,&travelModel.meanTravel);
        printf("Dato: %i, %hd, %hd, %hd, %.2f\n",i,travelModel.sourceId,travelModel.destinyId,travelModel.hour,travelModel.meanTravel);
        if(dataRow==EOF){
            printf("Lectura finalizada y datos guardados\n");
            break;
        }
        //Buscar el siguiente igual a el origen Id encontrado y enlazarlo
        travelModel.siguiente = buscarSiguiente(i,travelModel.sourceId);
        //Escribir el dato en el archivo binario
        fwrite(&travelModel,sizeof(struct travel),1,output_file);
    }
    //Liberar memoria
    free(linea);
    fclose(input_file);
    fclose(output_file);

    return 0;
}