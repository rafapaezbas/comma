#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <string.h>
#include <unistd.h>
#include <experimental/filesystem>
#include <vector>
#include <arpa/inet.h>


void error(std::string msg)
{
	std::cout << msg << std::endl;
	exit(1);
}

namespace fs = std::experimental::filesystem;

std::vector<std::string> readTree(std::string path){
	std::vector<std::string> paths;
	for (auto entry : fs::recursive_directory_iterator("/home/rafa/Escritorio")){
		paths.push_back(entry.path());
	}
	return paths;
}

std::string joinTree(std::vector<std::string> tree){
	std::string result = "";
	for(std::string element : tree){
		result = result + element + "\n";
	}
	return result;
}

int get_file_size(std::string filename){
	FILE *p_file = NULL;
	p_file = fopen(filename.c_str(),"rb");
	fseek(p_file,0,SEEK_END);
	int size = ftell(p_file);
	fclose(p_file);
	return size;
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

	//main loop
	while(1){
		n = read(newsockfd,buffer,255);
		//obtain client ip
		char *clientip = new char[20];
		strcpy(clientip, inet_ntoa(cli_addr.sin_addr));
		if (n < 0) {
			error("ERROR reading from socket");
		}

		std::string command(buffer); //Store message received in buffer
		std::cout << command << std::endl;

		if(command == "tree"){
			std::string tree = joinTree(readTree("/home/ubuntu"));
			std::string treeSize = std::to_string(strlen(tree.c_str()));
			n = write(newsockfd,treeSize.c_str(),sizeof(treeSize.c_str()));
			if (n < 0){
				error("ERROR writing to socket");
			}
		}

		if(command == "tree-ready"){
			std::string tree = joinTree(readTree("/home/ubuntu"));
			std::cout << tree << std::endl;
			n = write(newsockfd,tree.c_str(),strlen(tree.c_str()) + 1);
			if (n < 0){
				error("ERROR writing to socket");
			}
		}

		if(command[0] == 'f' && command[1] == ' '){
			std::string path = command.substr(2);
			std::cout << path << std::endl;
			int fileSize = get_file_size(path);
			std::cout << fileSize << std::endl;
			n = write(newsockfd,std::to_string(fileSize).c_str(),fileSize + 1);
			if (n < 0){
				error("ERROR writing to socket");
			}
		}

		if(command[0] == 'd' && command[1] == ' '){
			std::string path = command.substr(2);
			int fileSize = get_file_size(path);
			FILE* fp = fopen(path.c_str(), "rb");
			if (fp == NULL) 
			{
				perror("Can't open file");
				exit(1);
			}
			std::cout << fileSize << std::endl;
			char* fileBuffer = (char*) malloc(fileSize);
			std::cout << "buffer ready" << std::endl;
			fread(fileBuffer,1,fileSize,fp);
			int n = 0;
			while(n < fileSize){
				int s = write(newsockfd,fileBuffer + n,sizeof(fileBuffer));
				if (n < 0){
					error("ERROR writing to socket");
				}
				n = n + s;
				std::cout << "sended:" << std::endl;
				std::cout << n << std::endl;
			}
		}
	}
	close(newsockfd);
	close(sockfd);
	return 0; 
}
