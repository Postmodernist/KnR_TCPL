/*
Exercise 3-1.

Our binary search makes two tests inside the loop, when one would suffice
(at the price of more tests outside). Write a version with only one test
inside the loop and measure the difference in run-time.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void merge_sort(int*, int);
void rand_seed(unsigned long long int);
int  rand_int(int);


/* Find x in v[0] <= v[1] <= ... <= v[n-1]. */
int binsearch(int x, int v[], int n) {
    int low, high, mid;

    low = 0;
    high = n - 1;
    while (low <= high) {
        mid = low + (high - low) / 2;
        if (x < v[mid])
            high = mid - 1;
        else if (x > v[mid])
            low = mid + 1;
        else  /* found match */
            return mid;
    }
    return -1; /* no match */
}


/* Find x in sorted array v; make one test inside the loop.*/
int binsearch2(int x, int v[], int n) {
    int low, high, mid;

    low = 0;
    high = n - 1;
    while (low < high) {
        mid = low + (high - low + 1) / 2;
        if (x < v[mid])
            high = mid - 1;
        else
            low = mid;
    }

    return v[low] == x ? low : v[high] == x ? high : -1;
}


int main(int argc, char* argv[]) {
    int upper, n, i;
    clock_t t1, t2;

    if (argc < 5) {
        printf("Generate and sort an array of random integers, then return an index\n"
            "of a given number in it, or -1 if no such number is found.\n\n"
            "Usage:\tbinsearch number seed upper n\n\n"
            "\tnumber   Number to search for.\n"
            "\tseed     Seed of the random number generator (RNG).\n"
            "\tupper    Upper limit of a number returned by RNG.\n"
            "\tn        Size of array to be generated.\n\n");
        return 0;
    }

    rand_seed(atoi(argv[2]));
    upper = atoi(argv[3]);
    n = atoi(argv[4]);
    
    int a[n];
    for (i = 0; i < n; i++)
        a[i] = rand_int(upper);
    merge_sort(a, n);

    /* print array */
    for (i = 0; i < n-1; i++)
        printf("%d: %d, ", i, a[i]);
    printf("%d: %d\n\n", i, a[i]);

    /* print search result */
    i = binsearch(atoi(argv[1]), a, n);
    printf("%d: %d\n\n", i, a[i]);

    printf("Time to search every array member\n");

    /* measure binsearch (original) time */
    t1 = clock();
    for (i = 0; i < n-1; i++)
        if (binsearch(a[i], a, n) == -1)
            fprintf(stderr, "Search error: %d not found\n", a[i]);
    t2 = clock();
    printf("binsearch:  %ld\n", t2 - t1);
        
    /* measure binsearch2 time */
    t1 = clock();
    for (i = 0; i < n-1; i++)
        if (binsearch2(a[i], a, n) == -1)
            fprintf(stderr, "Search error: %d not found\n", a[i]);
    t2 = clock();
    printf("binsearch2: %ld\n", t2 - t1);
        
    return 0;
}


/* ---------------------------------------------------------------------------- */
/* Sorting */

/* Sorting routine for merge sort. */
void msrt_r(int v[], int v_tmp[], int l, int r) {
    int i, j, k, m;

    if (r - l > 0) {      // current partition has more than one element
        m = (r + l) / 2;  // median index

        // recursively subdivide...
        msrt_r(v, v_tmp, l, m);
        msrt_r(v, v_tmp, m+1, r);

        // ...and merge
        for (i = m + 1; i > l; i--)
            v_tmp[i-1] = v[i-1];    // copy left partition to v_tmp

        for (j = m; j < r; j++)
            v_tmp[r+m-j] = v[j+1];  // copy reversed right partition to v_tmp

        for (k = l; k <= r; k++)    // traverse the [l,r] range of v
            if (v_tmp[i] < v_tmp[j]) {
                v[k] = v_tmp[i];    // pull element from left partition
                i++;
            } else {
                v[k] = v_tmp[j];    // pull element from right partition
                j--;
            }
    }
}


/* Sort array v in-place. */
void merge_sort(int v[], int n) {
    int v_tmp[n];
    msrt_r(v, v_tmp, 0, n-1);
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
