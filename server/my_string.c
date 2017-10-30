#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "server.h"

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

char			*my_strncpy(char *dest, char *src, int n)
{
  int           i;

  i = 0;
  while (n > 0 && *(src + i))
    {
      *(dest + i) = *(src + i);
      ++i;
      --n;
    }
  if (n > 0)
    *(dest + i) = '\0';
  return (dest);
}

char            *my_strncat(char *dest, const char *src, int nb)
{
	int         len;
	int         i;

	len = my_strlen(dest);
	i = 0;
	while(i < nb && src[i])
	{
		dest[len + i] = src[i];
		i = i + 1;
	}
	dest[len + i] = '\0';
	return (dest);
}

void usage_message(char **argv)
{
    my_putstr("Usage :  ");
    my_putstr(argv[0]);
    my_putstr(" [MAX_CLIENTS]\n");
}

