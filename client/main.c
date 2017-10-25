#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "client.h"
#include "command.h"

static const Command orders_command[] =
{
    {"/command_list", command_list},
    {"/exit", exit_command},
    {NULL, NULL}
};

void exit_command(SOCKET sock)
{
    printf("You are leaving the server\n");
    shutdown(sock, SHUT_RDWR);
    end_connection(sock);
    exit(EXIT_SUCCESS);
}

void command_list()
{
    printf("COMMAND LIST:\n");
    printf("/exit\n\n");
}

static void app(const char *address, const char *name)
{
   SOCKET sock = init_connection(address);
   char buffer[BUF_SIZE];
   int i = 0;
   int command_input = 0;

   fd_set rdfs;

   /* send our name */
   write_server(sock, name);

   while(1)
   {
      FD_ZERO(&rdfs);

      /* add STDIN_FILENO */
      FD_SET(STDIN_FILENO, &rdfs);

      /* add the socket */
      FD_SET(sock, &rdfs);

      if(select(sock + 1, &rdfs, NULL, NULL, NULL) == -1)
      {
         perror("select()");
         exit(errno);
      }

      /* something from standard input : i.e keyboard */
      if(FD_ISSET(STDIN_FILENO, &rdfs))
      {
        fgets(buffer, BUF_SIZE - 1, stdin);
        char *p = NULL;
        p = strstr(buffer, "\n");
        if(p != NULL)
        {
            *p = 0;
        }
        else
        {
            /* fclean */
            buffer[BUF_SIZE - 1] = 0;
        }
        i = 0;
        command_input = 0;
        while (orders_command[i].orders != NULL)
        {
            if (strcmp(buffer, orders_command[i].orders) == 0)
            {
                orders_command[i].func(sock);
                command_input = 1;
            }
            i++;
        }
        if (!command_input)
            write_server(sock, buffer);
      }
      else if(FD_ISSET(sock, &rdfs))
      {
         int n = read_server(sock, buffer);
         /* server down */
         if(n == 0)
         {
            printf("You are disconnected !\n");
            break;
         }
         puts(buffer);
      }
   }
   end_connection(sock);
}

static int init_connection(const char *address)
{
   SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
   SOCKADDR_IN sin;
   struct hostent *hostinfo;

   if(sock == INVALID_SOCKET)
   {
      perror("socket()");
      exit(errno);
   }

   hostinfo = gethostbyname(address);
   if (hostinfo == NULL)
   {
      fprintf (stderr, "Unknown host %s.\n", address);
      exit(EXIT_FAILURE);
   }

   sin.sin_addr = *(IN_ADDR *) hostinfo->h_addr;
   sin.sin_port = htons(PORT);
   sin.sin_family = AF_INET;

   if(connect(sock,(SOCKADDR *) &sin, sizeof(SOCKADDR)) == SOCKET_ERROR)
   {
      perror("connect()");
      exit(errno);
   }
   return sock;
}

static void end_connection(int sock)
{
   closesocket(sock);
}

static int read_server(SOCKET sock, char *buffer)
{
   int n = 0;

   if((n = recv(sock, buffer, BUF_SIZE - 1, 0)) < 0)
   {
      perror("recv()");
      exit(errno);
   }

   buffer[n] = 0;

   return n;
}

static void write_server(SOCKET sock, const char *buffer)
{
   if(send(sock, buffer, strlen(buffer), 0) < 0)
   {
      perror("send()");
      exit(errno);
   }
}

int main(int argc, char **argv)
{
   if(argc < 2)
   {
      printf("Usage : %s [address] [pseudo]\n", argv[0]);
      return EXIT_FAILURE;
   }
   printf("Connection to the server...\n");
   printf("Type \"/command_list\" to see the command list\n");
   app(argv[1], argv[2]);
   return EXIT_SUCCESS;
}