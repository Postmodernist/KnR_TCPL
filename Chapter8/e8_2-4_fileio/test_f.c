#include "fileio.h"

int main(int argc, char const *argv[]) {
    FILE *fp1, *fp2;
    int i;

    /* test opening more that OPEN_MAX files */
    fp1 = fopen("test1.tmp", "w");
    fp2 = fopen("test2.tmp", "w");
    if (fp2)
        return 1;
    fclose(fp1);
    fp2 = fopen("test2.tmp", "w");
    if (!fp2)
        return 2;
    fclose(fp2);

    /* test writing file */
    fp1 = fopen("test1.tmp", "a");
    for (i = 0; i < 10; i++)
        putc('A' + i, fp1);
    putc('\n', fp1);
    fclose(fp1);
    
    /* test reading file */
    fp1 = fopen("test1.tmp", "r");
    stdout->_UNBUF = 1;
    for (i = getc(fp1); !feof(fp1); i = getc(fp1))
        putchar(i);
    fflush(stdout);
    fclose(fp1);

    /* test seek */
    fp1 = fopen("test1.tmp", "r");
    for (i = 0; i < 5; i++)
        putchar(getc(fp1));
    fseek(fp1, -3, SEEK_END);
    for (i = getc(fp1); !feof(fp1); i = getc(fp1))
        putchar(i);
    fflush(stdout);
    fclose(fp1);

    return 0;
}
