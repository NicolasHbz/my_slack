#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "client.h"
#include "command.h"

int                 init_connection(const char *address)
{
    SOCKET          sock = socket(AF_INET, SOCK_STREAM, 0);
    SOCKADDR_IN     sin;
    struct hostent  *hostinfo;

    if (sock == INVALID_SOCKET)
    {
        perror("socket()");
        exit(errno);
    }
    hostinfo = gethostbyname(address);
    if (hostinfo == NULL)
    {
        my_putstr("Unknown host ");
        my_putstr(address);
        my_putstr("\n");
        exit(EXIT_FAILURE);
    }
    sin.sin_addr = *(IN_ADDR *)hostinfo->h_addr;
    sin.sin_port = htons(PORT);
    sin.sin_family = AF_INET;
    if (connect(sock, (SOCKADDR *)&sin, sizeof(SOCKADDR)) == SOCKET_ERROR)
    {
        perror("connect()");
        exit(errno);
    }
    return sock;
}

void                end_connection(int sock)
{
    closesocket(sock);
}

int                 read_server(SOCKET sock, char *buffer)
{
    int n;

    n = 0;
    if ((n = recv(sock, buffer, BUF_SIZE - 1, 0)) < 0)
    {
        perror("recv()");
        exit(errno);
    }
    buffer[n] = 0;
    return n;
}

void                write_server(SOCKET sock, const char *buffer)
{
    if (send(sock, buffer, strlen(buffer), 0) < 0)
    {
        perror("send()");
        exit(errno);
    }
}