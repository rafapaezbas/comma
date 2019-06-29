#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <string.h>
#include <unistd.h>
#include <experimental/filesystem>
#include <vector>


void error(std::string msg)
{
	std::cout << msg << std::endl;
	exit(1);
}

namespace fs = std::experimental::filesystem;

std::vector<std::string> readTree(std::string path){
	std::vector<std::string> paths;
	for (auto entry : fs::recursive_directory_iterator("/media/rafa")){
		paths.push_back(entry.path());
	}
	return paths;
}

std::string joinTree(std::vector<std::string> tree){
	std::string result = "";
	for(std::string element : tree){
		result = result + element;
	}
	return result;
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
		if (n < 0) {
			error("ERROR reading from socket");
		}
		std::string command(buffer);
		std::cout << command << std::endl;

		if(command == "tree"){
			std::string tree = joinTree(readTree("/home/rafa/Escritorio"));
			std::cout << tree << std::endl;
			std::string treeSize = std::to_string(strlen(tree.c_str()));
			n = write(newsockfd,treeSize.c_str(),sizeof(treeSize.c_str()));
			if (n < 0){
				error("ERROR writing to socket");
			}
		}

		if(command == "tree-ready"){
			std::string tree = joinTree(readTree("/home/rafa/Escritorio"));
			n = write(newsockfd,tree.c_str(),strlen(tree.c_str() + 1));
			if (n < 0){
				error("ERROR writing to socket");
			}
		}
	}

	close(newsockfd);
	close(sockfd);
	return 0; 
}


