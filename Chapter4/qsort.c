/*
Failing to hit true median while picking pivot is a weak spot of qsort. Here
we experiment with qsort pivot picking. Also, subdivision convergence do not
go all the way to one element, but rather partitions of small size are sorted
by insertion sort.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ARRAY_SIZE 100000

int error;

void swap(int[], int, int);
void insertsort(int[], int, int);
int  getpivot(int[], int, int);


/* Sort v[left]...v[right] into increasing order in-place. */
void quicksort_r(int v[], int left, int right) {
    int i, last, pivot;

    if (left >= right)
        return;
    else if (right - left < 5) {
        insertsort(v, left, right);
        return;
    }

    pivot = (left + right) / 2;
    /* uncomment next line to search for better pivot (slow) */
    /*pivot = getpivot(v, left, right);*/
    swap(v, left, pivot);  /* move pivot elem to v[0] */
    last = left;

    for (i = left + 1; i <= right; ++i)  /* partition */
        if (v[i] < v[left])
            swap(v, ++last, i);

    swap(v, left, last);  /* restore pivot element */

    /* increase error by difference of partitions sizes */
    if (last - left > right - last)
        error += last * 2 - (left + right);
    else
        error += (left + right) - last * 2;

    quicksort_r(v, left, last - 1);
    quicksort_r(v, last + 1, right);
}

/* Interchange v[i] and v[j]. */
void swap(int v[], int i, int j) {
    register int tmp;

    if (i == j)
        return;
    
    tmp = v[i];
    v[i] = v[j];
    v[j] = tmp;
}

/* Sort v[left]...v[right] into increasing order in-place. */
void insertsort(int v[], int left, int right) {
    int i, j;
    register int tmp;
    for (i = left + 1; i <= right; ++i) {
        tmp = v[i];
        for (j = i; j > left && v[j-1] > tmp; --j)
            v[j] = v[j-1];
        v[j] = tmp;
    }
}

/* Take a random sample of array elements and return index of a sample median. */
int getpivot(int v[], int left, int right) {
    int i, j, sample_sz;
    register int tmp, tmp2;

    // take a sample
    sample_sz = (right - left) / 10;
    if (!sample_sz)
        sample_sz = 1;
    int idx[sample_sz];
    int elems[sample_sz];
    for (i = 0; i < sample_sz; ++i) {
        idx[i] = left + rand() % sample_sz;
        elems[i] = v[idx[i]];
    }

    // sort sample by values
    for (i = 1; i < sample_sz; ++i) {
        tmp = elems[i];
        tmp2 = idx[i];
        for (j = i; j > 0 && elems[j-1] > tmp; --j) {
            elems[j] = elems[j-1];
            idx[j] = idx[j-1];
        }
        elems[j] = tmp;
        idx[j] = tmp2;
    }

    return idx[sample_sz >> 1];
}


/* ---------------------------------------------------------------------------- */
/* Main */

void printa(int[], int);
void checksorted(int[], int);

int main() {
    int i, v[ARRAY_SIZE];

    srand((unsigned)clock());
    for (i = 0; i < ARRAY_SIZE; ++i)
        v[i] = rand();

    quicksort_r(v, 0, ARRAY_SIZE - 1);
    checksorted(v, ARRAY_SIZE);

    printf("Partitioning entropy: %g\n", (double)error / ARRAY_SIZE);
}

/* ---------------------------------------------------------------------------- */
/* Utils */

/* Print array. */
void printa(int v[], int len) {
    for (int i = 0; i < len; ++i)
        printf("%d ", v[i]);
    putchar('\n');
}

/* Check if array is sorted. */
void checksorted(int v[], int len) {
    for (int i = 1; i < len; ++i)
        if (v[i] < v[i-1]) {
            printf("FAIL - array is not sorted.\n");
            return;
        }
    printf("OK - array is sorted.\n");
}