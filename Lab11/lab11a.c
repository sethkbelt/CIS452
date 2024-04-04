#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define KILO_BYTE 1024
#define AMOUNT 2
#define LOOP 200

int main()
{
    int count, *intPtr;
    int pgSize;
    long int i, j, dim = AMOUNT * KILO_BYTE;
    pgSize = getpagesize();
    if ((intPtr = malloc(dim * dim * sizeof(int))) == 0) {
	    perror("totally out of space");
	    exit(1);
    }
    for (count = 1; count <= LOOP; count++) {
        printf ("Loop count %d\n", count);
	    for (i = 0; i < dim; i++) {
	        for (j = 0; j < dim; j++) {
                // void* addr = intPtr + i*dim +j;
                // printf ("At address %p in page %lu\n", addr, (unsigned long) addr/pgSize);
		        usleep (1000); // 1000 microseconds
	            intPtr[i * dim + j] = (i + j) % count;
            }
	    }
    }

    free(intPtr);
    return 0;
}
