#ifndef SERVER_H
#define SERVER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> /* close */
#include <netdb.h> /* gethostbyname */
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket(s) close(s)
typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;

#define CRLF		"\r\n"
#define PORT	 	2000
#define BUF_SIZE	512

#include "client.h"

void app(int maxClients);
int init_connection(int maxClients);
void end_connection(int sock);
int read_client(SOCKET sock, char *buffer);
void write_client(SOCKET sock, const char *buffer);
void send_message_to_all_clients(Client *clients, Client client, int actual, const char *buffer, char from_server);
void remove_client(Client *clients, int to_remove, int *actual);
void clear_clients(Client *clients, int actual);
int getMaxClient(int argc, char **argv);
int my_getnbr(char *str);
void my_putstr(const char *str);
char *my_strncpy(char *dest, char *src, int n);
char *my_strncat(char *dest, const char *src, int nb);
int test_name_availability(Client *clients, SOCKET sock, const char *name, int actual);
int isNumber(char *str);
void usage_message(char **argv);
void app(int maxClients);

#endif