#include<stdio.h>
#include<stdlib.h>

int main(){
    /*Estructura de un viaje para ser guardada en la tabla hash, aqui
    se le pasa el dato binario de un viaje, por eso el tipo int*/
    struct travelTable{
    int travelBinary;
    };

    /*Inicializo la estructura que permitira ir reservando memoria*/
    struct travelTable travelModelBinary;
    travelModelBinary.travelBinary = 0;


    FILE *fhashBinary;
    fhashBinary = fopen("hashTable.bin","w+b");
    if(fhashBinary==NULL){
        printf("Error con el archivo hashtable.bin");
    }
    /*Con este ciclo for, se guardan los datos binarios del dataBinario.bin, reservando
    memoria para la hashtable*/
    for(int i=0;i<1300;i++){
        fwrite(&travelModelBinary,sizeof(struct travelTable),1,fhashBinary);
    }
    
    printf("Reserva de memoria completa\n");
    fclose(fhashBinary);
    return 0;
}