#include <stdlib.h>

#include "client.h"
#include "command.h"

void    exit_command(SOCKET sock)
{
    my_putstr("You are leaving the server\n");
    shutdown(sock, SHUT_RDWR);
    end_connection(sock);
    exit(EXIT_SUCCESS);
}

void    command_list()
{
    my_putstr("COMMAND LIST:\n");
    my_putstr("/exit\n\n");
}