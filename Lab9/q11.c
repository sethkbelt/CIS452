#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Global variables
int global_initialized_var = 5; // global initialized
char global_initialized_var_char = 'x'; // global initialized
char global_initialized_var_array[3] = {'x', 'x', 'x'}; // global initialized

int global_uninitialized_var; // global uninitialized
char global_uninitialized_var_char; // global initialized
char global_uninitialized_var_array[3]; // global initialized


// (local, initialized, uninitialized, arrays, pointers, etc.)
// Function declarations
void func1();
void func2(int x);
void func3(int *x);

int main() {
    // Local variables
    int local_var_initialized = 10;
    int local_var_uninitialized;

    int *heap_var = malloc(sizeof(int)); // heap variable
    *heap_var = 15;

    char *heap_var_char = malloc(sizeof(char)); // heap variable
    *heap_var = 15;

    float* heap_float;
    int n = 10;
    heap_float = (float*)malloc(n * sizeof(float));


    char array_unit[10];
    int array_init[3] = {1,2,3};

    global_uninitialized_var = 3;
    // Print addresses
    printf("%p Address of global_initialized_var (Initialized Data) \n", &global_initialized_var);
    printf("%p Address of global_initialized_var_char (Initialized Data) \n", &global_initialized_var_char);
    printf("%p Address of global_initialized_var_array (Initialized Data) \n", &global_initialized_var_array);
    printf("%p Address of global_uninitialized_var (Unitialized Data) \n", &global_uninitialized_var);
    printf("%p Address of global_uninitialized_var_char (Unitialized Data) \n", &global_uninitialized_var_char);
    printf("%p Address of global_uninitialized_var_array (Unitialized Data) \n", &global_uninitialized_var_array);
    printf("%p Address of local_var (Run time Stack Space.) \n", &local_var_initialized);
    printf("%p Address of heap_var (Run Time Heap Space. Created first.)\n", heap_var);
    printf("%p Address of heap_var_char (Run Time Heap Space. Created Second.)\n", heap_var_char);
    printf("%p Address of heap_float (Run Time Heap Space. Created Third.)\n", heap_float);
    printf("%p Address of local_var_uninitialized (Run time Stack Space) \n", &local_var_uninitialized);
    printf("%p Address of array_init (Run time Stack Space) \n", &array_init);
    printf("%p Address of array_unit (Run time Stack Space. Created first.) \n", &array_unit);



    // Call function
    func1();

    // Free heap memory
    free(heap_var);

    return 0;
}

void func1() {
    int *heap_var_f = malloc(sizeof(int)); // heap variable
    memset(heap_var_f, 0, sizeof(int)); 
    int func1_var = 20;
    printf("%p Address of func1_var (Run time Stack Space) \n", &func1_var);
    printf("%p Address of heap_var_f (Run time Stack Space) \n", &heap_var_f);
    func2(3);
    free(heap_var_f);
}

void func2(int x) {
    int func2_var = 25;
    printf("%p Address of func2_var (Run time Stack Space) \n", &func2_var);
    printf("%p Address of funct2 parameter  (Run time Stack Space) \n", &x);
    printf("%p Address of function1 (Program Text) \n", &func1);
    printf("%p Address of function2 (Program Text) \n", &func2);
    printf("%p Address of function3 (Program Text) \n", &func3);
    printf("%p Address of main (Program Text) \n", &main);
    func3(&x);
}

void func3(int *x) {
    char func2_var = 'x';
    printf("%p Address of func3_var (Run time Stack Space) \n", &func2_var);
    printf("%p Address of funct3 parameter (Run time Stack Space. Created Last.)\n", &x);

}
