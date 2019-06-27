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


int sockfd, portno, n;
struct sockaddr_in serv_addr;
struct hostent *server;
char buffer[256];

struct State {
	virtual void execute(){};
};

struct DefaultState : State {

	State* currentState;
	std::vector<State>* states;
	DefaultState(State* currentState_,std::vector<State>* states_){
		currentState = currentState_;
		states = states_;
	}

	void execute() override{
		printf("Please enter the message: ");
		bzero(buffer,256);
		fgets(buffer,255,stdin);
		n = write(sockfd,buffer,strlen(buffer));
		if (n < 0){
		}
		//Reset buffer
		bzero(buffer,256);
		n = read(sockfd,buffer,255);
		if (n < 0) {
		}
		printf("%s\n",buffer);
		close(sockfd);
	}
};

struct WaitingTreeState : State {
	void execute() override{
	}
};

struct WaitingTreeSizeState : State {
	void execute() override{
	}
};

struct WaitingFileState : State {
	void execute() override{
	}
};

struct WaitingFileSizeState : State {
	void execute() override{
	}
};
