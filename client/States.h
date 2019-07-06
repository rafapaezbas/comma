#include "connect.h"

/**
 * States table
 * defaultState = 0,
 * waitingTreeSize = 1,
 * waitingTree = 2,
 * waitingFileSize = 3,
 * waitingFile = 4,
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

		if(command == "f"){ //file request
			*currentState = 3;
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
		std::cout << "Waiting tree size in bytes for next message received." << std::endl;
		read(sockfd,inputBuffer,255);
		*currentState = 2;
	}
};

struct WaitingFileSizeState : State {

	int* currentState;
	char* inputBuffer = new char[256];

	WaitingFileSizeState(int* currentState_,char (&inputBuffer_)[256]){
		currentState = currentState_;
		inputBuffer = inputBuffer_;
	}

	void execute() override{
		std::cout << "Waiting file size in bytes for next message received." << std::endl;
		read(sockfd,inputBuffer,255);
		*currentState = 4;
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
		std::cout << "Waiting tree." << std::endl;
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

	int* currentState;
	char* inputBuffer = new char[256];

	WaitingFileState(int* currentState_,char (&inputBuffer_)[256]){
		currentState = currentState_;
		inputBuffer = inputBuffer_;
	}

	void execute() override{
		std::cout << "Waiting file." << std::endl;
		int fileSize = atoi(inputBuffer);
		std::cout << fileSize << std::endl;
		write(sockfd,"file-ready",255);
		char file[fileSize];
		int r = 0;
		while(r < fileSize){
			r = r + read(sockfd,file,fileSize + 1);
		}
		std::cout << r << std::endl;
		*currentState = 0;
		FILE *fp = fopen("/home/rafa/Escritorio/file", "wb");
		if (fp == NULL) 
		{
			perror("Can't open file");
			exit(1);
		}
		fwrite(file, sizeof(char), fileSize, fp);
	}
};
