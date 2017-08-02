/*
Exercise 2-2.

    for (i = 0; i < lim - 1 && (c = getchar()) != '\n' && c != EOF); ++i)
        s[i] = c;

Write a loop equivalent to the for loop above without using && or ||.
*/

for (i = 0; ; ++i)
    if (i >= lim - 1)
        break;
    else if ((c = getchar()) == '\n')
        break;
    else if (c == EOF)
        break;
    else
        s[i] = c;
