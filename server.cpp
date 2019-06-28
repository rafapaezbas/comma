#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <string.h>
#include <unistd.h>

void error(std::string msg)
{
	std::cout << msg << std::endl;
	exit(1);
}

int main (int argc, char** argv){

	//sock file descriptor, network file descriptor
	int sockfd, newsockfd,n;
	//Needed structs for holding server and client information
	socklen_t clilen;
	char buffer[256];
	sockaddr_in serv_addr, cli_addr;
	//get file descriptor of server socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0){ 
		error("ERROR opening socket");
	}
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(10000);
	//Bind socket to machine port
	if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) {
		error("ERROR on binding");
	}

	listen(sockfd,5);
	clilen = sizeof(cli_addr);
	newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr,&clilen);
	while(1){
		n = read(newsockfd,buffer,255);
		if (n < 0) {
			error("ERROR reading from socket");
		}
		//printf("Here is the message: %s\n",buffer);
		std::cout << buffer << std::endl;
		n = write(newsockfd,"I got your message",18);
		if (n < 0){
			error("ERROR writing to socket");
		}
	}
	close(newsockfd);
	close(sockfd);
	return 0; 
}


