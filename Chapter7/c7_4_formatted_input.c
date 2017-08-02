#include <stdio.h>

/* Rudimentary calculator. */
void calc() {
    double sum, v;

    sum = 0;
    while (scanf("%lf", &v) == 1)
        printf("\t%.2f\n", sum += v);
}

/* Read date either in 'dd Month yyyy' or 'mm/dd/yy' format. */
void readdate() {
    int day, month, year;
    char line[100], monthname[20];

    while (fgets(line, sizeof(line), stdin))
        if (sscanf(line, "%d %s %d", &day, monthname, &year) == 3)
            printf("valid: %s\n", line);
        else if (sscanf(line, "%d/%d/%d", &month, &day, &year) == 3)
            printf("valid: %s\n", line);
        else
            printf("invalid: %s\n", line);
}
