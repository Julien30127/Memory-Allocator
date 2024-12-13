#include"mem_alloc_2.0.h"


//Table des caches globaux
ThreadCache* thread_caches;

int size_to_index(size_t size) {
    int index = 0;
    size = size + sizeof(Block); // Ajouter la taille de l'en-tête
    while ((1 << (index + 4)) < size) {
        index++;
    }
    return index;
}

//Déclaration de la clé pour le cache par thread
pthread_key_t thread_caches_key_2_0;

void init_thread_cache() {
    thread_caches = calloc(1, sizeof(ThreadCache));
}

ThreadCache* get_thread_cache() {
    ThreadCache* cache = pthread_getspecific(thread_caches_key_2_0);
    if (cache == NULL) {
        //Au cas où le cache n'existe pas encore
        cache = calloc(1, sizeof(ThreadCache));
        pthread_setspecific(thread_caches_key_2_0, cache);
    }
    return cache;
}

//Cache global
Block* global_free_lists[MAX_BLOCK_SIZE / BLOCK_SIZE_STEP];


void* my_malloc_2_0(size_t size) {
    if (size == 0) return NULL;

    int index = size_to_index(size);
    ThreadCache* cache = get_thread_cache();

    //Vérifier d'abord dans le cache du thread
    if (cache->cache_lists[index] != NULL) {
        Block* block = cache->cache_lists[index];
        cache->cache_lists[index] = block->next;  // Retirer du cache

        //Si le bloc est plus grand que nécessaire, le diviser
        if (block->size > size + sizeof(Block)) {
            Block* new_block = (Block*)((char*)block + size);
            new_block->size = block->size - size;
            block->size = size;
            //Ajouter le bloc restant au cache
            cache->cache_lists[size_to_index(new_block->size)] = new_block;
        }

        return (char*)block + sizeof(Block);  // Retourner le pointeur après l'en-tête
    }

    //Si le cache est vide, allouer un nouveau bloc avec mmap
    int mmap_size = (1 << (index + 4));
    Block* block = mmap(NULL, mmap_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (block == MAP_FAILED) {
        return NULL;
    }

    block->size = mmap_size;
    return (char*)block + sizeof(Block);
}


void my_free_2_0(void* ptr) {
    if (ptr == NULL) return;

    Block* block = (Block*)((char*)ptr - sizeof(Block));
    int index = size_to_index(block->size);

    ThreadCache* cache = get_thread_cache();
    //Ajout du bloc au cache
    block->next = cache->cache_lists[index];
    cache->cache_lists[index] = block;
}

void cleanup_thread_cache(void* arg) {
    free(arg);
}

void init_allocator() {
    // Création de la clé pour le cache des threads
    if (pthread_key_create(&thread_caches_key_2_0, cleanup_thread_cache) != 0) {
        perror("Erreur de création de clé de cache");
        exit(1);
    }
}

void performance_test_2_0() {
    clock_t start, end;
    double cpu_time_used;

    //Initialiser les variables de test
    void* ptrs[NUM_ITERATIONS];

    start = clock();

    //Effectuer 1 million d'allocations
    for (int i = 0; i < NUM_ITERATIONS; i++) {
        size_t size = MIN_BLOCK_SIZE + (i % (MAX_BLOCK_SIZE / MIN_BLOCK_SIZE)) * BLOCK_SIZE_STEP;
        ptrs[i] = my_malloc_2_0(size);
    }

    //Effectuer 1 million de libérations
    for (int i = 0; i < NUM_ITERATIONS; i++) {
        my_free_2_0(ptrs[i]);
    }

    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Temps d'exécution pour %d allocations et libérations: %.2f secondes\n", NUM_ITERATIONS, cpu_time_used);
}


