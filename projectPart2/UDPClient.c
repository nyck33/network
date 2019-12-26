//UDP echo client program
#include "headerFiles.h"
int main(int argc, char* argv[]) //Three ags to be checked later
{
	//declare and define vars
	int s; 
	int len
	char* servName;
	int servPort;
	char *string;
	char buffer[256+1];
	struct sockaddr_in servAddr;
	//check and set args
	if(argc!=3)
	{
		printf("Error: usage program {serv_name} {port} {string}...");
		exit(1);
	}
	servName=argv[1];
	servPort = atoiargv[2];

}