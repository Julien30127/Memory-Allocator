#include"mem_alloc.h"

int main() 
{
    //Test unitaire

    void* ptr1 = my_malloc_test(100);
    void* ptr2 = my_malloc_test(256);
    void* ptr3 = my_malloc_test(512);
    void* ptr4 = my_malloc_test(800);
    void* ptr5 = my_malloc_test(1024);

    //Test de my_free

    my_free_test(ptr1); 
    my_free_test(ptr2); 
    my_free_test(ptr3); 
    my_free_test(ptr4); 
    my_free_test(ptr5);
}