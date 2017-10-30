#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "client.h"
#include "command.h"

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        usage_message(argv);
        return EXIT_FAILURE;
    }
    app(argv[1], argv[2]);
    return EXIT_SUCCESS;
}