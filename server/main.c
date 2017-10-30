#include <stdlib.h>

#include "server.h"
#include "client.h"

int     main(int argc, char **argv)
{
    int maxClients;
    int is_argv_number;

    is_argv_number = 0;
    if(argc != 2)
    {
       usage_message(argv);
       return EXIT_FAILURE;
    }
    is_argv_number = isNumber(argv[1]);
    if(!is_argv_number)
    {
        usage_message(argv);
        return EXIT_FAILURE;
    }
    maxClients = getMaxClient(argc, argv);
    app(maxClients);
    return EXIT_SUCCESS;
}