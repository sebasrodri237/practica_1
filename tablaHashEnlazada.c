#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definición de la estructura para un nodo de la lista enlazada
typedef struct node {
    short int sourceId, destinyId, hour;
    float meanTravel;
    int index;
    struct node *next;
} node_t;

// Definición de la estructura para la tabla hash
typedef struct table {
    node_t **nodes;
    int last_index;
} table_t;

// Función para crear un nodo de la lista enlazada
node_t *create_node(short int sourceId, short int destinyId, short int hour, float meanTravel, int index) {
    node_t *node = (node_t *) malloc(sizeof(node_t));

    node->sourceId = sourceId;
    node->destinyId = destinyId;
    node->hour = hour;
    node->meanTravel = meanTravel;
    node->index = index;
    node->next = NULL;
    return node;
}

// Función para crear la tabla hash
table_t *create_table(int size) {
    table_t *table = (table_t *) malloc(sizeof(table_t));
    table->nodes = (node_t **) malloc(size * sizeof(node_t *));
    for (int i = 0; i < size; i++) {
        table->nodes[i] = NULL;
    }
    table->last_index = -1;
    return table;
}

// Función para insertar un valor en la tabla hash
void insert(table_t *table, short int sourceId, short int destinyId, short int hour, float meanTravel) {
    table->last_index++;
    node_t *node = create_node(sourceId, destinyId, hour, meanTravel, table->last_index);
    table->nodes[table->last_index] = node;
}

// Función para enlazar los nodos de la tabla hash que tienen el mismo valor
void link_nodes(table_t *table) {
    for (int i = 0; i <= table->last_index; i++) {
        node_t *node = table->nodes[i];
        int sourceId = node->sourceId;
        for (int j = i+1; j <= table->last_index; j++) {
            if (table->nodes[j]->sourceId == sourceId) {
                node->next = table->nodes[j];
                node = node->next;
            }
        }
    }
}

// Función para buscar un valor en la tabla hash y obtener el índice de los siguientes valores iguales
void search(table_t *table, short int sourceId, int *indices, int *num_indices) {
    *num_indices = 0;
    for (int i = 0; i <= table->last_index; i++) {
        if (table->nodes[i] != NULL && table->nodes[i]->sourceId == sourceId) {
            indices[*num_indices] = i;
            (*num_indices)++;
        }
    }
}
void print_table(table_t *table) {
    for (int i = 0; i <= table->last_index; i++) {
        printf("[%d]: %d\n", table->nodes[i]->index, table->nodes[i]->sourceId);
    }
}

int main() {
    FILE *fData;
    fData = fopen("dataLimpio.csv","r");
    if(fData==NULL){
        printf("Error al abrir el archivo de datos de viaje\n");
        exit(0);
    }
    int size = 100;
    table_t *table = create_table(size);
    printf("Table created with size %d\n", size);
    for (int i = 1; i <= size; i++)
    {   
        short int sId,dId,h;
        float mTr;
        int dataRow = fscanf(fData,"%hd,%hd,%hd,%f,%*s",&sId,&dId,&h,&mTr);
        insert(table,sId,dId,h,mTr);
        if(dataRow==EOF){
            printf("Lectura finalizada y datos guardados\n");
            break;
        }
        
        print_table(table);
        link_nodes(table);
        print_table(table);
        
    }
    int indices[size];
    int num_indices = 0;
    int sourceId = 1004;
    search(table, sourceId, indices, &num_indices);
    printf("Valores iguales a %d en la tabla hash:\n", sourceId);
    for (int i = 0; i < num_indices; i++) {
        printf("[%d]: %d\n", table->nodes[indices[i]]->index, table->nodes[indices[i]]->sourceId);
    }

    for(int i = 0; i <= table->last_index; i++) {
        node_t *node = table->nodes[i];
        while(node != NULL) {
            node_t *tmp = node;
            node = node->next;
            free(tmp);
        }
    }
    free(table->nodes);
    free(table);
    return 0;
}