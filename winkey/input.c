#include "winkey.h"

char    *input_buffer(void)
{
    static char buf[2048];
    return (buf);
}

void    input_add_key(char c)
{
    char    *buf = input_buffer();
    int     i = 0;

    while (i < 2048 - 1 && buf[i])
        i++;

    if (i < 2048 - 1)
        buf[i] = c;
}

void    input_add_string(char *str)
{
    char    *buf = input_buffer();
    int     i = 0;
    int     j = 0;

    while (i < 2048 - 1 && buf[i])
        i++;

    while (i < 2048 - 1 && str[j])
    {
        buf[i] = str[j];
        i++;
        j++;
    }
}

char    *input_get_buffer(void)
{
    return (input_buffer());
}

void    input_clean_buffer(void)
{
    char    *buf = input_buffer();
    for (int i = 0; i < 2048; i++) {
        buf[i] = 0;
    }
}