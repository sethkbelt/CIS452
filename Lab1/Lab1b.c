/****************************************************************************
 * Title: CIS452 Lab1B
 * Author: Seth Konynenbelt
 * Created: January 11, 2024
 * Description: Use valgrind to detect memory leaks
 *              for dynamic memory debugging practice.
 ***************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 16

int main()
{
    char *data1, *data2;
    int k;
    do {
        data1 = malloc(SIZE);
        printf ("Please input your EOS username: ");
        scanf ("%s", data1);
        if (! strcmp (data1, "quit"))
            break;
        data2 = malloc(SIZE);
        for (k = 0; k < SIZE; k++)
            data2[k] = data1[k];
        printf ("data2 :%s:\n", data2);
        // free data1 and data 2 pointer, moved here for readabilty
        free(data1);
        free(data2);
    } while(1);
    // free data1 before we exit
    // we know only data1 malloced if we are here
    free(data1);
    return 0;
}