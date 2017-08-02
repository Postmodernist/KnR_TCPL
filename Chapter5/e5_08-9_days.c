/*
Exercise 5-8.

There is no error checking in day_of_year or month_day. Remedy this defect.

Exercise 5-9.

Rewrite the routines day_of_year and month_day with pointers instead of
indexing.
*/

#include <stdio.h>
#include <stdlib.h>

static const char daytab[][13] = {
    {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
};

/* Return day of year from month & day. */
int day_of_year(int year, int month, int day) {
    int leap = (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;

    if (year < 0 || month < 1 || month > 12 || day < 1 || day > daytab[leap][month]) {
        printf("Error: invalid date\n");
        exit(EXIT_FAILURE);
    }

    const char *mp = *(daytab + leap);
    const char *mp_end = mp + month;
    for (; mp < mp_end; day += *mp++);
    return day;
}

/* Set month, day from day of year. */
void month_day(int year, int yearday, int *pmonth, int *pday) {
    int leap = (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;

    if (year < 0 || yearday < 1 || yearday > 365 + leap) {
        printf("Error: invalid year / day\n");
        exit(EXIT_FAILURE);
    }

    const char *mp = *(daytab + leap);
    for (; yearday > *mp; yearday -= *mp++);
    *pmonth = mp - *(daytab + leap);
    *pday = yearday;
}

/* Return name of n-th month. */
const char *month_name(int n) {
    static const char *name[] = {
        "Illegal month", "January", "February", "March", "April", "May", "June", "July",
        "August", "September", "October", "November", "December"
    };

    return (n < 1 || n > 12) ? name[0] : name[n];
}

int main(int argc, const char *argv[]) {
    if (argc == 1) {
        printf("mm dd yyyy       Convert date to day of year.\n"
               "dd yyyy          Convert day of year month & day.\n");
    } else if (argc == 4) {
        int m = atoi(argv[1]), d = atoi(argv[2]), y = atoi(argv[3]);
        int dy = day_of_year(y, m, d);
        printf("%s %d %d is %d day of the year.\n", month_name(m), d, y, dy);
    } else if (argc == 3) {
        int dy = atoi(argv[1]), y = atoi(argv[2]), m, d;
        month_day(y, dy, &m, &d);
        printf("%d day of %d is %s %d.\n", dy, y, month_name(m), d);
    }
}
