#ifndef CLIENT_H
#define CLIENT_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket(s) close(s)
typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;

#define CRLF	 "\r\n"
#define PORT	 2000
#define BUF_SIZE 1024

void app(const char *address, const char *name);
int init_connection(const char *address);
void end_connection(int sock);
int read_server(SOCKET sock, char *buffer);
void write_server(SOCKET sock, const char *buffer);
void exit_command(SOCKET sock);
void command_list();
int my_getnbr(char *str);
int my_strlen(const char *str);
void my_putstr(const char *str);
void usage_message(char **argv);
int	my_strcmp(char *s1, char *s2);

#endif