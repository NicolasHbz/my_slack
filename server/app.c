#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "server.h"
#include "client.h"

void        app(int maxClients)
{
    SOCKET  sock = init_connection(maxClients);
    char    buffer[BUF_SIZE];
    int     actual = 0;
    int     max = sock;
    Client  clients[maxClients];
    fd_set  rdfs;
    my_putstr("\nServeur run...\n");

    while (1)
    {
        int i;
        FD_ZERO(&rdfs);
        FD_SET(STDIN_FILENO, &rdfs);
        FD_SET(sock, &rdfs);
        for (i = 0; i < actual; i++)
        {
            FD_SET(clients[i].sock, &rdfs);
        }
        if (select(max + 1, &rdfs, NULL, NULL, NULL) == -1)
        {
            perror("select()");
            exit(errno);
        }
        if (FD_ISSET(STDIN_FILENO, &rdfs))
        {
            break;
        }
        else if (FD_ISSET(sock, &rdfs))
        {
            SOCKADDR_IN csin;
            unsigned int sinsize = sizeof csin;
            int csock = accept(sock, (SOCKADDR *)&csin, &sinsize);
            int test = 0;
            if (csock == SOCKET_ERROR)
            {
                perror("accept()");
                continue;
            }
            if (read_client(csock, buffer) == -1)
            {
                continue;
            }
            max = csock > max ? csock : max;
            FD_SET(csock, &rdfs);
            Client c = {csock, ""};
            my_strncpy(c.name, buffer, BUF_SIZE - 1);
            test = test_name_availability(clients, c.sock, c.name, actual);
            if (test)
            {
                clients[actual] = c;
                write_client(c.sock, "You are connected to the server");
                my_strncpy(buffer, c.name, BUF_SIZE - 1);
                my_strncat(buffer, " is connected !", BUF_SIZE - strlen(buffer) - 1);
                send_message_to_all_clients(clients, c, actual, buffer, 1);
                actual++;
            }
        }
        else
        {
            int i = 0;
            for (i = 0; i < actual; i++)
            {
                if (FD_ISSET(clients[i].sock, &rdfs))
                {
                    Client client = clients[i];
                    int c = read_client(clients[i].sock, buffer);
                    if (c == 0)
                    {
                        closesocket(clients[i].sock);
                        remove_client(clients, i, &actual);
                        my_strncpy(buffer, client.name, BUF_SIZE - 1);
                        my_strncat(buffer, " disconnected !", BUF_SIZE - strlen(buffer) - 1);
                        send_message_to_all_clients(clients, client, actual, buffer, 1);
                    }
                    else
                    {
                        send_message_to_all_clients(clients, client, actual, buffer, 0);
                    }
                    break;
                }
            }
        }
    }
    clear_clients(clients, actual);
    end_connection(sock);
}