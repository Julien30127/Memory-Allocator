#include"mem_alloc.h"

#define PAGE_SIZE 4096 // Taille d'une page mémoire


void* my_malloc_test(size_t size) 
{
    if (size == 0) {
        return NULL; // Ne rien allouer pour une taille de 0
    }

    // Arrondir la taille à la taille d'une page mémoire
    size_t alloc_size = size + sizeof(Block);

    // Utiliser mmap pour allouer de la mémoire
    void* ptr = mmap(NULL, alloc_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    
    if (ptr == MAP_FAILED) {
        printf("Allocation malheureusement échouée.\n");
        return NULL; // Si l'allocation échoue
    }

    // Initialiser l'en-tête du bloc
    Block* block = (Block*)ptr;
    block->size = size; // Enregistrer la taille du bloc
    block->next = NULL;  // Aucun bloc suivant pour l'instant

    // Afficher l'adresse et la taille du bloc alloué
    printf("Bloc alloué à %p, taille: %zu octets\n", ptr, block->size);

    // Retourner le pointeur à l'utilisateur après l'en-tête
    return (void*)((char*)ptr + sizeof(Block));
}   


void* my_malloc(size_t size) {
    if (size == 0) {
        return NULL; // Ne rien allouer pour une taille de 0
    }

    size_t alloc_size = size + sizeof(Block);

    void* ptr = mmap(NULL, alloc_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    
    if (ptr == MAP_FAILED) {
        printf("Allocation malheureusement échouée.\n");
        return NULL; // Si l'allocation échoue
    }

    Block* block = (Block*)ptr;
    block->size = size; // Enregistrer la taille du bloc
    block->next = NULL;  // Aucun bloc suivant pour l'instant

    return (void*)((char*)ptr + sizeof(Block));
}


void my_free_test(void* ptr) {
    if (ptr == NULL) {
        return; // Ne rien faire si le pointeur est NULL
    }

    //Retour en arrière de sizeof(Block) octets
    Block* block = (Block*)((char*)ptr - sizeof(Block));

    //Affichage de l'adresse et de la taille du bloc libéré
    printf("Mémoire libérée pour le bloc à %p, taille: %zu octets\n", (void*)block, block->size);

    //Libéreration du bloc mémoire
    if (munmap(block, block->size + sizeof(Block)) == -1) {
        perror("munmap échoué");
    }
}

void my_free(void* ptr) {
    if (ptr == NULL) {
        return; // Ne rien faire si le pointeur est NULL
    }

    Block* block = (Block*)((char*)ptr - sizeof(Block));

    if (munmap(block, block->size + sizeof(Block)) == -1) {
        perror("munmap échoué");
    }
}

void performance_test() 
{
    //Test de performance 
    
    const int taille = 124;
    const int nbre = 1000000; // Nombre d'allocations à effectuer
    void* ptrs[nbre]; // Tableau pour stocker les pointeurs alloués
    clock_t start, end;

    //Mesure du temps d'allocation
    start = clock();
    for (int i = 0; i < nbre; ++i) 
    {
        ptrs[i] = my_malloc(taille);
    }
    end = clock();
    double alloc_time = ((double)(end - start)) / CLOCKS_PER_SEC; // Temps écoulé en secondes
    printf("Temps pour allouer %d blocs: %f secondes\n", nbre, alloc_time);

    //Mesure du temps de libération
    start = clock();
    for (int i = 0; i < nbre; ++i) 
    {
        my_free(ptrs[i]);
    }
    end = clock();
    double free_time = ((double)(end - start)) / CLOCKS_PER_SEC; // Temps écoulé en secondes
    printf("Temps pour libérer %d blocs: %f secondes\n", nbre, free_time);
}

