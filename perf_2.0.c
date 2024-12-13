#include"mem_alloc_2.0.h"

int main() 
{
    printf("Démarrage du test de performance\n");

    init_allocator();

    performance_test_2_0();

    return 0;
}