#include <stdlib.h> 

struct person_node { 
    char* name;
    int age;
};

struct pet_node { 
    char* name; 
    char type[10]; 
};

typedef struct person_node person; 
typedef struct pet_node pet; 

int main() { 
    /* TODO: dynamically allocate enough memory, assign the blocks to these pointers */
    
    int* A;     // Construct a 10-elmeent integer array 
    A = (int *) malloc(10 * sizeof(int));

    int** B;    // Construct a 2D array of 10 x 10 integers,  
    B = (int **) malloc(10 * sizeof(int *));
    for (int i=0; i<10; i++) {
        B[i] = (int *) malloc(10 * sizeof(int));
    }

    person* C;  // Construct an array of 10 persons, allocate 10 characters for each name
    C = (person *) malloc(10 * sizeof(person));
    for (int i=0; i<10; i++) {
        (C[i]).name = (char *) malloc(10 * sizeof(char));
    }

    pet* D;     // Construct an array of 10 pets, allocate 10 characters for each name;
    D = (pet *) malloc(10 * sizeof(pet));
    for (int i=0; i<10; i++) {
        (D[i]).name = (char *) malloc(10 * sizeof(char));
    }

    // free any memory you just allocated before you return!
    free(A);

    for (int i=0; i<10; i++) {
        free(B[i]);
    }
    free(B);

    for (int i=0; i<10; i++) {
        free((C[i]).name);
    }
    free(C);

    for (int i=0; i<10; i++) {
        free((D[i]).name);
    }
    free(D);

    return 0; 
}
