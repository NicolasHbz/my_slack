#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "client.h"

int             my_getnbr(char *str)
{
    int         i;
    int         a;
    int         b;

    a = 1;
    i = 0;
    while (str[i] != '\0' && (str[i] == '-' || str[i] == '+'))
    {
        if (str[i] == '-')
        {
            a = a * -1;
        }
        i++;
    }
    str = str + i;
    b = 0;
    i = 0;
    while (str[i] <= '9' && str[i] >= '0')
    {
        b = b * 10;
        b = b - (str[i] - '0');
        i++;
    }
    return (b * a * -1);
}

int			    my_strlen(const char *str)
{
    int			i;

    i = 0;
    while (str[i] != '\0')
        i++;
    return (i);
}

void			my_putstr(const char *str)
{
    write(1, str, my_strlen(str));
}

void usage_message(char **argv)
{
    my_putstr("Usage :  ");
    my_putstr(argv[0]);
    my_putstr(" [IP] [USERNAME]\n");
}

int			my_strcmp(char *s1, char *s2)
{
  int			i;

  i = 0;
  while ((s1[i] == s2[i]) && (s1[i] != '\0') && (s2[i] != '\0'))
    {
      i++;
    }
  return (s1[i] - s2[i]);
}