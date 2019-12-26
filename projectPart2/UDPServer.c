//UDP echo server
#include "headerFiles.h"
int main(int argc, char *argv[])
{
	//declare an define variables
	int sockfd; //socket descriptor
	int len; //length of string to be echoed
	char buffer[1024];  //data buffer
	struct sockaddr_in servAddr, clntAddr;  //server (local) socket address
	//struct sockadrr_in clntAddr;  //client (remote) socket address
	int clntAddrLen;  //length of client socket address
	char *end_signal = "!!!!!";  //last message to close socket
	//address and port number from argv
	//char *localhost = argv[1];
	int port = atoi(argv[2]);	
	//Build local (server) socket address
	memset(&servAddr, 0, sizeof(servAddr));  //allocate memory
	servAddr.sin_family = AF_INET;  //family field
	servAddr.sin_port = htons(port);  //default port number
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);  //default IP address
	//Create socket
	if((sockfd = socket(AF_INET, SOCK_DGRAM, 0))<0)
	{
		perror("Error, socket failed");
		exit(1);
	}
	//bind socket to local address and port
	if((bind(sockfd, (struct sockaddr*)&servAddr,sizeof(servAddr)))<0)
	{
		perror("Error, bind failed");
		exit(1);
	}
	for(;;)
	{
		//receive string
		len = recvfrom(sockfd, buffer, sizeof(buffer), 0, 
			(struct sockaddr*)&clntAddr, &clntAddrLen);
		if (strcmp(buffer, end_signal)==0)
			break;
		//send string
		sendto(sockfd, buffer, len, 0, (struct sockaddr*)&clntAddr, sizeof(clntAddr));
	}//End of for loop
	close(sockfd);
	exit(0);

}//End of echo server program
