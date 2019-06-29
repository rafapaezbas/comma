#include "connect.h"

int sockfd, portno, n;
struct sockaddr_in serv_addr;
struct hostent *server;

void init_socket(int port, char host[]){
	//Open socket file descriptor
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		std::cout << "ERROR opening socket" << std::endl;
	}
	//translate host in the args to ip
	server = gethostbyname(host);
	if (server == NULL) {
		fprintf(stderr,"ERROR, no such host\n");
		exit(0);
	}
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr,(char *)&serv_addr.sin_addr.s_addr,server->h_length);
	serv_addr.sin_port = htons(port);
	if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) {
		std::cout << "ERROR connecting" << std::endl;
	}
}
