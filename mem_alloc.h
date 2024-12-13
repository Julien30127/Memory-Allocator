#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include <time.h>

typedef struct Block {
    size_t size;   // Taille du bloc
    struct Block* next;  // Pointeur vers le bloc suivant
} Block;

void* my_malloc_test(size_t size);

void* my_malloc(size_t size); // Fonction d'allocation de mémoire

void my_free_test(void* ptr);

void my_free(void* ptr); // Fonction de libération de mémoire

void performance_test();