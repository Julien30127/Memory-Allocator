#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <pthread.h>
#include <string.h>
#include <time.h>

#define MAX_BLOCK_SIZE 1024  // Taille maximale des blocs
#define MIN_BLOCK_SIZE 16    // Taille minimale des blocs
#define BLOCK_SIZE_STEP 16   // Incrément de taille (puissances de 2)
#define CACHE_SIZE 64        // Taille maximale du cache pour chaque taille de bloc
#define NUM_ITERATIONS 1000000 // Nombre d'itérations pour le test



typedef struct Block {
    size_t size;           // Taille du bloc (incluant l'en-tête)
    struct Block* next;    // Pointeur vers le prochain bloc
} Block;


//Structure contenant les caches pour chaque thread
typedef struct ThreadCache {
    Block* cache_lists[MAX_BLOCK_SIZE / BLOCK_SIZE_STEP]; // Cache local pour chaque taille de bloc
} ThreadCache;

//Fonction pour obtenir l'indice de la liste correspondant à la taille
int size_to_index(size_t size);

//Fonction pour initialiser le cache par thread
void init_thread_cache();

//Fonction pour obtenir le cache d'un thread
ThreadCache* get_thread_cache();

//Fonction d'allocation avec cache
void* my_malloc_2_0(size_t size);

//Fonction de libération avec ajout au cache
void my_free_2_0(void* ptr);

//Nettoie le cache lorsque le thread se termine
void cleanup_thread_cache(void* arg);

//Initialisation du cache et de l'allocation
void init_allocator();

//Mesure le temps d'exécution des allocations et des libérations
void performance_test_2_0();