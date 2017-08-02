/*
Exercise 5-3.

Write a pointer version of the function strcat that we showed in Chapter 2:
strcat(s, t) copies the string t to the end of s.

Exercise 5-4.

Write the function strend(s, t), which returns 1 if the string t occurs at
the end of the string s, and zero otherwise.

Exercise 5-5.

Write versions of the library functions strncpy, strncat and strncmp, which
operate on at most the first n characters of their argument strings. For
example, strncpy(s, t, n) copies at most n characters of t to s. Full
descriptions are in Appendix B.
*/

#include <stdio.h>

/* Return length of s. */
size_t strlen_n(const char *s) {
    const char *p = s;
    for (; *p; ++p);
    return p - s;
}

/* Copy src to dst. */
char *strcpy_n(char *dst, const char *src) {
    while ((*dst++ = *src++));
    return dst;
}

/* Copy src to dst, stop at n characters. */
char *strncpy_n(char *dst, const char *src, size_t n) {
    for (; n && (*dst++ = *src++); n--);
    return dst;
}

/* Return <0 if s<t, 0 if s==t, >0 if s>t. */
int strcmp_n(const char *s1, const char *s2) {
    for (; *s1 && *s2 && *s1 == *s2; s1++, s2++);
    return *s1 - *s2;
}

/* Return <0 if s<t, 0 if s==t, >0 if s>t; compare at most n characters. */
int strncmp_n(const char *s1, const char *s2, size_t n) {
    for (; n && *s1 && *s2 && *s1 == *s2; n--, s1++, s2++);
    return *s1 - *s2;
}

/* Concatenate src to the end of dst. */
char *strcat_n(char* dst, const char* src) {
    strcpy_n(dst + strlen_n(dst), src);
    return dst;
}

/* Concatenate src to the end of dst, stop at n characters. */
char *strncat_n(char* dst, const char* src, size_t n) {
    strncpy_n(dst + strlen_n(dst), src, n);
    return dst;
}

/* Returns 1 if s1 ends with s2, 0 otherwise. */
int strend(const char *s1, const char *s2) {
    size_t s1len = strlen_n(s1), s2len = strlen_n(s2);
    if (s1len < s2len)
        return 0;
    return !strcmp_n(s1 + s1len - s2len, s2);
}

int main() {
    char s[64];
    strcpy_n(s, "aabbcc");
    strcat_n(s, "ddee");
    printf("%s\n", s);
    if (strend(s, "dee"))
        printf("OK\n");
}
