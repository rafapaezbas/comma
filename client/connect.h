#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <vector>
#include <iostream>

extern int sockfd, portno, n;
extern struct sockaddr_in serv_addr;
extern struct hostent *server;
void init_socket(int port, char host[]);
