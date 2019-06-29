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
std::string command;

/**
 * States table
 * defaultState = 0,
 * waitingTreeSize = 1,
 * waitingTree = 2,
 */

struct State {
	virtual void execute(){};
};

struct DefaultState : State {

	int* currentState;
	DefaultState(int* currentState_){
		currentState = currentState_;
	}

	void execute() override{
		printf("Command: ");
		std::cin >> command;
		write(sockfd,command.c_str(),255);
		if(command == "tree"){
			*currentState = 1;
		}
	}
};

struct WaitingTreeSizeState : State {

	int* currentState;
	char* inputBuffer = new char[256];

	WaitingTreeSizeState(int* currentState_,char (&inputBuffer_)[256]){
		currentState = currentState_;
		inputBuffer = inputBuffer_;
	}

	void execute() override{
		std::cout << "Waiting tree size" << std::endl;
		read(sockfd,inputBuffer,255);
		*currentState = 2;
	}
};

struct WaitingTreeState : State {

	int* currentState;
	char* inputBuffer = new char[256];

	WaitingTreeState(int* currentState_,char (&inputBuffer_)[256]){
		currentState = currentState_;
		inputBuffer = inputBuffer_;
	}

	void execute() override{
		std::cout << "Waiting tree" << std::endl;
		int treeSize = atoi(inputBuffer);
		std::cout << treeSize << std::endl;
		char tree[treeSize];
		write(sockfd,"tree-ready",255);
		read(sockfd,tree,treeSize + 1);
		std::cout << tree << std::endl;
		*currentState = 0;
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
