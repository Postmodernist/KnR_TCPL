/* Initialize array of random numbers and sort it. */

#include <stdio.h>
#include <stdlib.h>

/* Sort v[0]...v[n-1] into increasing order. */
void shellsort(int v[], int n) {
    int gap, i, j, temp;
    
    for (gap = n/2; gap > 0; gap /= 2)
        for (i = gap; i < n; i++)
            for (j = i - gap; j >= 0 && v[j] > v[j+gap]; j -= gap) {
                temp = v[j];
                v[j] = v[j+gap];
                v[j+gap] = temp;
            }
}

void rand_seed(unsigned long long int);
int  rand_int(int);
void printarr(int*, int);

int main(int argc, const char* argv[]) {
    if (argc < 4) {
        printf("Generates an array of random integers, then sorts it using Shell sort.\n\n"
            "Usage:   shellsort SEED UPPER N\n\n"
            "SEED     Seed of the random number generator (RNG).\n"
            "UPPER    Upper limit of a number returned by RNG.\n"
            "N        Size of array to be generated.\n\n");
        return 0;
    }
    
    rand_seed(atoi(argv[1]));
    int upper = atoi(argv[2]);
    int n = atoi(argv[3]);
    int a[n];

    printf("Initializing array...\n\n");
    for (int i = 0; i < n; i++)
        a[i] = rand_int(upper);

    printarr(a, n);
    shellsort(a, n);
    printarr(a, n);

    return 0;
}

#define PRINT_ARRAY_MAX 100

/* Print array contents. */
void printarr(int v[], int n) {
    int i;
    printf("[");
    for (i = 0; i < n; i++)
        if (i > PRINT_ARRAY_MAX) {
            printf("...]\n");
            break;
        } else if (i == n - 1)
            printf("%d]\n", v[i]);
        else
            printf("%d, ", v[i]);
}

/* ---------------------------------------------------------------------------- */
/* Random number generator */

unsigned long long rand_next = 0;

/* Seed random number generator. */
void rand_seed(unsigned long long seed) {
    extern unsigned long long rand_next;
    rand_next = seed;
}

/* Return random integer in range [0, upper] */
int rand_int(int upper) {
    extern unsigned long long rand_next;
    rand_next = rand_next * 1103515245 + 12345;
    return (int)((rand_next >> 32) % (upper + 1));
}
