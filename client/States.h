#include "connect.h"

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
		std::string command;
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
		write(sockfd,"tree-ready",255);
		char tree[treeSize];
		int r = 0;
		while(r < treeSize){
			r = r + read(sockfd,tree,treeSize + 1);
			std::cout << tree << std::endl;
		}
		std::cout << r << std::endl;
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
