#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "client.h"
#include "command.h"

static const Command    orders_command[] =
{
    {"/command_list", command_list},
    {"/exit", exit_command},
    {NULL, NULL}
};

void                    app(const char *address, const char *name)
{
    SOCKET              sock = init_connection(address);
    char                buffer[BUF_SIZE];
    int                 i = 0;
    int                 command_input = 0;

    fd_set              rdfs;
    my_putstr("Connection to the server...\n");
    my_putstr("Type \"/command_list\" to see the command list\n");
    write_server(sock, name);
    while (1)
    {
        FD_ZERO(&rdfs);
        FD_SET(STDIN_FILENO, &rdfs);
        FD_SET(sock, &rdfs);
        if (select(sock + 1, &rdfs, NULL, NULL, NULL) == -1)
        {
            perror("select()");
            exit(errno);
        }
        if (FD_ISSET(STDIN_FILENO, &rdfs))
        {
            fgets(buffer, BUF_SIZE - 1, stdin);
            char *p = NULL;
            p = strstr(buffer, "\n");
            if (p != NULL)
            {
                *p = 0;
            }
            else
            {
                buffer[BUF_SIZE - 1] = 0;
            }
            i = 0;
            command_input = 0;
            while (orders_command[i].orders != NULL)
            {
                if (my_strcmp(buffer, orders_command[i].orders) == 0)
                {
                    orders_command[i].func(sock);
                    command_input = 1;
                }
                i++;
            }
            if (!command_input)
                write_server(sock, buffer);
        }
        else if (FD_ISSET(sock, &rdfs))
        {
            int n = read_server(sock, buffer);
            if (n == 0)
            {
                my_putstr("You are disconnected !\n");
                break;
            }
            puts(buffer);
        }
    }
    end_connection(sock);
}