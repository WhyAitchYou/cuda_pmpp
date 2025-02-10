#include <stdio.h>
#include <stdlib.h>

float f = 3.14;
float *A_d = &f;

// ref: https://forums.developer.nvidia.com/t/void-d-why-two/12609
void wrongMallocWrapper(void* ptr, int size) {
    // ptr is a void*, which is a pointer points to void type
    // ptr pass in is a copy, so modifying local copy doesn't
    // allow you to access the address that malloc returns
    ptr = malloc(size);
}

void correctMallocWrapper(void** ptr, int size) {
    // on the other hand, instead of passing in a pointer_d
    // you pass in the pointer to that pointer_d
    // ptr is still a local copy,
    // but *ptr dereferences to the value of pointer_d
    // which can then be used for storing the address malloc returns
    *ptr = malloc(size);
}

void main() {
    int* ptr = NULL; // ptr is a pointer with type int*
    // *(&ptr) is equal to ptr
    // (void*) &ptr casts ptr from type int* to void*
    // (void**) &ptr is the address of above void* pointer
    correctMallocWrapper((void**) &ptr, 10);
    ptr[0] = 888;
    printf("ptr[0] = %d\n", ptr[0]);

    printf("A_d is a pointer points to a float with value: %f\n", *A_d);
    // unlike *A_d which dereferences the pointer A_d, accessing the float it points to
    // (void*)A_d casts A_d to a void pointer, changes the type without affecting
    // the address it holds
    // since A_d itself is a pointer, below two print statements give same outputs
    printf("(void*)A_d: %p\n", (void*)A_d);
    printf("A_d: %p\n", A_d);

    // A_d is a pointer pointing to a float
    // &A_d takes the address of A_d
    // (void*)&A_d casts &A_d to a void pointer
    printf("&A_d: %p\n", &A_d);
    printf("&A_d: %p\n", (void*)&A_d);

    // A_d is a pointer to float, A_d is float*
    // (void*)A_d casts A_d to a void pointer, A_d is now void*
    // (void**)A_d casts (void*)A_d to a void pointer
    // it is a pointer, which points to a void pointer,
    // or you can say (void**)A_d is a pointer to a void pointer
    printf("(void**)A_d: %p\n", (void**)A_d);

    // cast a pointer to void* is dangerous
    // for example, A_d has type float*, but after casting to void*
    // and if you treat it like a integer, it prints out trash
    printf("Wrongly casted &A_d to int: %d\n", (void*) A_d);
    printf("Wrongly casted &A_d to str: %s\n", (void*) A_d);
}