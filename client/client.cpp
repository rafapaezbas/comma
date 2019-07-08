#include "States.h"

int main(int argc, char *argv[]){

	if (argc < 3) {
		fprintf(stderr,"usage %s hostname port\n", argv[0]);
		exit(0);
	}

	init_socket(atoi(argv[2]),argv[1]);

	int currentState = 0;
	std::string command;
	char inputBuffer[256];

	std::vector<State*> states = std::vector<State*>();
	states.push_back(new DefaultState(&currentState,&command));
	states.push_back(new WaitingTreeSizeState(&currentState,inputBuffer));
	states.push_back(new WaitingTreeState(&currentState,inputBuffer));
	states.push_back(new WaitingFileSizeState(&currentState,inputBuffer));
	states.push_back(new WaitingFileState(&currentState,inputBuffer,&command));

	while(true){
		states.at(currentState)->execute();
	}

	return 0;
}
