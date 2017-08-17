/*
Exercise 3-3.

Write a function expand(s1, s2) that expands shorthand notations like a-z in
the string s1 into the equivalent complete list abc...xyz in s2. Allow for
letters of either case and digits, and be prepared to handle cases like a-b-c
and a-z0-9 and -a-z. Arrange that a leading or trailing - is taken literally.
*/

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>


/* Expand shorthand notations like a-z in the string s1 into the equivalent complete
list abc...xyz in s2. */
char *expand(const char s1[], char s2[]) {
    int i, j, k, step;
    size_t s1_len;

    i = j = 0;
    s1_len = strlen(s1);
    if (s1_len == 0)
        goto end;
    
    for (; i < strlen(s1) - 1; i++) {
        s2[j++] = s1[i];    /* copy current character */

        if (s1[i+1] == '-' &&
            ((isupper(s1[i]) && isupper(s1[i+2])) ||
             (islower(s1[i]) && islower(s1[i+2])) ||
             (isdigit(s1[i]) && isdigit(s1[i+2])))) {
            if (s1[i] != s1[i+2]) {
                step = (s1[i] > s1[i+2]) ? -1 : 1;
                for (k = s1[i] + step; k != s1[i+2]; k += step)
                    s2[j++] = k;  /* expand the shorthand sequence */
            } else
                i++;        /* skip duplicate character*/
            i++;            /* skip '-' */
        }
    }

    if (s1[i] != '\0')
        s2[j++] = s1[i];    /* copy the last character */
end:
    s2[j] = '\0';
    return s2;
}


void test_expand(void) {
    char s[1024];
    assert(!strcmp(expand("abc", s), "abc"));
    assert(!strcmp(expand("", s), ""));
    assert(!strcmp(expand("a", s), "a"));
    assert(!strcmp(expand("-a", s), "-a"));
    assert(!strcmp(expand("a-", s), "a-"));
    assert(!strcmp(expand("-abc-", s), "-abc-"));
    assert(!strcmp(expand("a-d", s), "abcd"));
    assert(!strcmp(expand("d-a", s), "dcba"));
    assert(!strcmp(expand("1-5a-c", s), "12345abc"));
    assert(!strcmp(expand("A-C-F", s), "ABCDEF"));
    assert(!strcmp(expand("a-C", s), "a-C"));
    assert(!strcmp(expand("a-a", s), "a"));
    assert(!strcmp(expand("a-b-c", s), "abc"));
    printf("tests passed\n");
}


int main(void) {
    test_expand();
}
