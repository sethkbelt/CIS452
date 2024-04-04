#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

const size_t GIGA = 1024*1024*1024;

int main() {
    for (int k = 1; k <= 128; k++) {
        char *p = malloc(k * GIGA);
        if (p == NULL || errno != 0) {
            perror("After malloc() failure");
            fprintf (stderr, "Failed at %d GBytes\n", k);
            return -1;
        }
        free(p);
    }
    printf ("Max attempted allocation 128G\n");
    return 0;
}
