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
	std::string* command;
	DefaultState(int* currentState_,std::string* command_){
		currentState = currentState_;
		command = command_;
	}

	void execute() override{
		std::cout << "Default State" << std::endl;
		std::string command_temp;
		printf("Command: ");
		std::getline(std::cin, *command);
		std::cout << *command << std::endl;

		write(sockfd,command->c_str(),255);

		if(*command == "tree"){
			*currentState = 1;
		}

		if(command->at(0) == 'f' && command->at(1) == ' '){ //file request
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
	std::string* command;

	WaitingFileState(int* currentState_,char (&inputBuffer_)[256],std::string* command_){
		currentState = currentState_;
		inputBuffer = inputBuffer_;
		command = command_;
	}

	void execute() override{
		std::cout << "Waiting file." << std::endl;
		int fileSize = atoi(inputBuffer);
		std::cout << "Filesize:" << std::endl;
		std::cout << fileSize << std::endl;
		std::string path = command->substr(2);
		write(sockfd,("d " + path).c_str(),255);
		int total = 0; //total bytes received
		int r = 0;
		while(total < fileSize){
			char* file_temp = (char*) malloc(fileSize);
			r = read(sockfd,file_temp,fileSize + 1);
			std::cout << "received bytes: " << std::endl;
			std::cout << r << std::endl;
			FILE *fp = fopen("/home/rafa/Escritorio/file", "ab");
			fwrite(file_temp, 1, r, fp);
			fclose(fp);
			total = total + r;
			free(file_temp);
		}
		*currentState = 0;
	}
};
