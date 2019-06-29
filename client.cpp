#include "States.h"

int main(int argc, char *argv[])
{
	int currentState = 0; //defaultState
	char inputBuffer[256];
	std::vector<State*> states = std::vector<State*>();

	//States instantiation
	states.push_back(new DefaultState(&currentState));
	states.push_back(new WaitingTreeSizeState(&currentState,inputBuffer));
	states.push_back(new WaitingTreeState(&currentState,inputBuffer));


	if (argc < 3) {
		fprintf(stderr,"usage %s hostname port\n", argv[0]);
		exit(0);
	}

	//Initialize listening socket
	portno = atoi(argv[2]);
	//Open socket file descriptor
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		std::cout << "ERROR opening socket" << std::endl;
	}
	//translate host in the args to ip
	server = gethostbyname(argv[1]);
	if (server == NULL) {
		fprintf(stderr,"ERROR, no such host\n");
		exit(0);
	}
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr,(char *)&serv_addr.sin_addr.s_addr,server->h_length);
	serv_addr.sin_port = htons(portno);
	if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) {
		std::cout << "ERROR connecting" << std::endl;
	}
	while(true){
		states.at(currentState)->execute();
	}
	return 0;
}
