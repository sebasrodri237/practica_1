#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 100

struct travel{
    /*Posiciones de la estructura para ir recorriendo datos y guardarlos con una posicion*/
    int actualPosition,nextPosition;
    short int sourceId,destinyId,hour;
    float meanTravel;
    };

// Definimos una estructura para los nodos de la lista enlazada
//En este caso la estructura del nodo debe ser la de un viaje
typedef struct node {
    int key;
    struct travel tr;
    struct node *next;
} Node;

// Definimos una estructura para la tabla hash
typedef struct hash_table {
    int size;
    Node **table;
} HashTable;

// Función hash: devuelve un índice para la clave dada
int hash(int key, int size) {
    return key % size;
}

// Función para crear un nuevo nodo
Node* create_node(int key, struct travel tr) {
    Node *new_node = (Node*) malloc(sizeof(Node));
    new_node->key = key;
    new_node->tr = tr;
    new_node->next = NULL;
    return new_node;
}

// Función para crear una nueva tabla hash
HashTable* create_table(int size) {
    HashTable *table = (HashTable*) malloc(sizeof(HashTable));
    table->size = size;
    table->table = (Node**) malloc(size * sizeof(Node*));
    for (int i = 0; i < size; i++) {
        table->table[i] = NULL;
    }
    return table;
}

// Función para insertar un par clave-valor en la tabla hash
void insert(HashTable *table, int key, struct travel tr) {
    int index = hash(key, table->size);
    Node *new_node = create_node(key, tr);
    if (table->table[index] == NULL) {
        // Si la posición en la tabla está vacía, agregamos el nuevo nodo allí
        table->table[index] = new_node;
    } else {
        // Si ya hay un nodo en la posición de la tabla, lo agregamos a la lista enlazada
        Node *current = table->table[index];
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_node;
    }
}

// Función para buscar el valor de una clave en la tabla hash
int search(HashTable *table, int key) {
    int index = hash(key, table->size);
    Node *current = table->table[index];
    while (current != NULL) {
        if (current->key == key) {
            // Si encontramos la clave, devolvemos su valor
            return current->tr.destinyId;
        }
        current = current->next;
    }
    // Si la clave no se encuentra en la tabla, devolvemos -1
    return -1;
}
void print_list(HashTable *table, int key) {
    int index = hash(key, table->size);
    Node *current = table->table[index];
    printf("Lista enlazada correspondiente a la clave %d:\n", key);
    while (current != NULL) {
        if (current->key == key) {
            printf("%hd\n", current->tr.destinyId);
        }
        current = current->next;
    }
}
int main() {
    HashTable *table = create_table(MAX_SIZE);
    int count = 0;
    
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
            
            insert(table, travelModel.sourceId, travelModel);
            count = count + 1;
        }


    // Buscamos el valor correspondiente a una clave en la tabla hash
    print_list(table, 1004);
    // printf("%d\n", search(table, 1)); // debería imprimir 10
    // printf("%d\n", search(table, 4)); // debería imprimir -1

    return 0;
}
