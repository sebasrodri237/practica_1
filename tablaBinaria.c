
#include <stdio.h>

struct travel{
    /*Posiciones de la estructura para ir recorriendo datos y guardarlos con una posicion*/
    short int sourceId,destinyId,hour,siguiente;
    double meanTravel;
};

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
    char linea[100]; // Suponemos que cada línea tiene una longitud máxima de 100 caracteres
    struct travel travelModel;
    FILE *input_file = fopen("dataLimpio.csv", "r");
    FILE *output_file = fopen("a.bin", "wb");
    for(int i=1;i<=1000000;i++){
        int dataRow = fscanf(input_file,"%hd,%hd,%hd,%lf,%*s",&travelModel.sourceId,&travelModel.destinyId,&travelModel.hour,&travelModel.meanTravel);
        printf("Dato: %i, %hd, %hd, %hd, %.2f\n",i,travelModel.sourceId,travelModel.destinyId,travelModel.hour,travelModel.meanTravel);
        if(dataRow==EOF){
            printf("Lectura finalizada y datos guardados\n");
            break;
        }
        travelModel.siguiente = buscarSiguiente(i,travelModel.sourceId);
        fwrite(&travelModel,sizeof(struct travel),1,output_file);
    }

    fclose(input_file);
    fclose(output_file);

    return 0;
}