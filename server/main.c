#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "server.h"
#include "client.h"

int test_name_availability(Client *clients, SOCKET sock, const char *name, int actual)
{
    int i;
    for (i = 0; i < actual; i++)
    {
        if(strcmp(clients[i].name, name) == 0)
        {
            write_client(sock, "This username is already used");
            shutdown(sock, SHUT_RDWR);
            remove_client(clients, i, &actual);
            return 0;
        }
    }
    return 1;
}

void app(int maxClients)
{
    SOCKET sock = init_connection(maxClients);
    char buffer[BUF_SIZE];
    int actual = 0;
    int max = sock;
    Client clients[maxClients];
    fd_set rdfs;
    my_putstr("\nServeur run...\n");

   while(1)
   {
      int i = 0;

      FD_ZERO(&rdfs);

      FD_SET(STDIN_FILENO, &rdfs);

      FD_SET(sock, &rdfs);

      for(i = 0; i < actual; i++)
      {
         FD_SET(clients[i].sock, &rdfs);
      }

      if(select(max + 1, &rdfs, NULL, NULL, NULL) == -1)
      {
         perror("select()");
         exit(errno);
      }

      if(FD_ISSET(STDIN_FILENO, &rdfs))
      {
         break;
      }

      else if(FD_ISSET(sock, &rdfs))
      {
         SOCKADDR_IN csin;
         unsigned int sinsize = sizeof csin;
         int csock = accept(sock, (SOCKADDR *)&csin, &sinsize);
         int test = 0;
         
         if(csock == SOCKET_ERROR)
         {
            perror("accept()");
            continue;
         }

         if(read_client(csock, buffer) == -1)
         {
            continue;
         }

         max = csock > max ? csock : max;

         FD_SET(csock, &rdfs);

         Client c = { csock, "" };
         strncpy(c.name, buffer, BUF_SIZE - 1);
         test = test_name_availability(clients, c.sock, c.name, actual);
         if (test)
         {
            clients[actual] = c;
            write_client(c.sock, "You are connected to the server");
            strncpy(buffer, c.name, BUF_SIZE - 1);
            strncat(buffer, " is connected !", BUF_SIZE - strlen(buffer) - 1);
            send_message_to_all_clients(clients, c, actual, buffer, 1);
            actual++;
         }
      }
      else
      {
         int i = 0;
         for(i = 0; i < actual; i++)
         {    
            if(FD_ISSET(clients[i].sock, &rdfs))
            {
               Client client = clients[i];
               int c = read_client(clients[i].sock, buffer);
               if(c == 0)
               {
                  closesocket(clients[i].sock);
                  remove_client(clients, i, &actual);
                  strncpy(buffer, client.name, BUF_SIZE - 1);
                  strncat(buffer, " disconnected !", BUF_SIZE - strlen(buffer) - 1);
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

void clear_clients(Client *clients, int actual)
{
   int i = 0;
   for(i = 0; i < actual; i++)
   {
      closesocket(clients[i].sock);
   }
}

void remove_client(Client *clients, int to_remove, int *actual)
{
   memmove(clients + to_remove, clients + to_remove + 1, (*actual - to_remove - 1) * sizeof(Client));
   (*actual)--;
}

void send_message_to_all_clients(Client *clients, Client sender, int actual, const char *buffer, char from_server)
{
   int i = 0;
   char message[BUF_SIZE];
   message[0] = 0;
   for(i = 0; i < actual; i++)
   {
      if(sender.sock != clients[i].sock)
      {
         if(from_server == 0)
         {
            strncpy(message, sender.name, BUF_SIZE - 1);
            strncat(message, " : ", sizeof message - strlen(message) - 1);
         }
         strncat(message, buffer, sizeof message - strlen(message) - 1);
         write_client(clients[i].sock, message);
      }
   }
}

int init_connection(int maxClients)
{
   SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
   SOCKADDR_IN sin;

   if(sock == INVALID_SOCKET)
   {
      perror("socket()");
      exit(errno);
   }

   sin.sin_addr.s_addr = htonl(INADDR_ANY);
   sin.sin_port = htons(PORT);
   sin.sin_family = AF_INET;

   if(bind(sock,(SOCKADDR *) &sin, sizeof sin) == SOCKET_ERROR)
   {
      perror("bind()");
      exit(errno);
   }

   if (listen(sock, maxClients) == SOCKET_ERROR)
   {
      perror("listen()");
      exit(errno);
   }
   return sock;
}

void end_connection(int sock)
{
   closesocket(sock);
}

int read_client(SOCKET sock, char *buffer)
{
   int n = 0;

   if((n = recv(sock, buffer, BUF_SIZE - 1, 0)) < 0)
   {
      perror("recv()");
      n = 0;
   }
   buffer[n] = 0;
   return n;
}

void write_client(SOCKET sock, const char *buffer)
{
   if(send(sock, buffer, strlen(buffer), 0) < 0)
   {
      perror("send()");
      exit(errno);
   }
}

int isNumber(char *str) 
{
    while(*str != '\0')
    {
        if(*str < '0' || *str > '9')
            return 0;
        str++;
    }
    return 1;
}

int getMaxClient(int argc, char **argv)
{
    int maxClients;

    maxClients = 0;
    if (argc > 1)
    {
        maxClients = my_getnbr(argv[1]);
        if (maxClients < 2 || maxClients > 100)
        {
            my_putstr("Please enter a number between 2 and 100\n");
            exit(EXIT_FAILURE);
        }
    }
    my_putstr("Maximum clients : ");
    my_putstr(argv[1]);
    return maxClients;
}

void usage_message(char **argv)
{
    printf("Usage : %s [MAX_CLIENTS]\n", argv[0]);
}

int main(int argc, char **argv)
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

// MY FONCTIONS -----------------------------------------------------------------------------------------------
int my_getnbr(char *str)
{
    int i;
    int a;
    int b;

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

int			my_strlen(const char *str)
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