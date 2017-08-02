#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ARRAY_SIZE 20


/* Sort v[left]...v[right] into increasing order in-place. */
void quicksort_r(int v[], int left, int right) {
    int i, last;
    void swap(int v[], int i, int j);

    if (left >= right)  /* do nothing if array contains */
        return;         /* fewer than two elements */
    swap(v, left, left + (right - left) / 2);   /* move partition elem */
    last = left;                                /* to v[0] */
    for (i = left + 1; i <= right; ++i)         /* partition */
        if (v[i] < v[left])
            swap(v, ++last, i);
    swap(v, left, last);                        /* restore partition elem */
    quicksort_r(v, left, last - 1);
    quicksort_r(v, last + 1, right);
}


/* Interchange v[i] and v[j]. */
void swap(int v[], int i, int j) {
    int temp;

    temp = v[i];
    v[i] = v[j];
    v[j] = temp;
}


int main() {
    int i, v[ARRAY_SIZE];

    /* generate */
    srand((unsigned)clock());
    for (i = 0; i < ARRAY_SIZE; ++i)
        v[i] = rand() % 1001;
    /* sort */
    quicksort_r(v, 0, ARRAY_SIZE - 1);
    /* print */
    for (int i = 0; i < ARRAY_SIZE; ++i)
        printf("%d ", v[i]);
    putchar('\n');
}
