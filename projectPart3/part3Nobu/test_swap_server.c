#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <netdb.h>  //
#include <sys/socket.h> //
#include <netinet/in.h> //
#include <arpa/inet.h>  //

extern int swap_wait(unsigned short port);
extern int swap_read(int sd, char *buf);
extern void swap_close(int sd);


#define MAX_FTA	128

// These arguments must be passed to the program
// argv[1]: port number, e.g., "6789"

int main (int argc, char *argv[])
{
	int	sd, bytes_read;
	char	buf[MAX_FTA];

	if (argc < 2) {
		fprintf(stderr, "Usage: %s port_number\n", argv[0]);
		exit(1);
	}

	sd = swap_wait(htons(atoi(argv[1])));
	if (sd < 0) {
		fprintf(stderr, "%s cannot wait, %d\n", argv[0], sd);
		exit(1);
	}

	// read message from the client and store them into the above file
	int count = 0;
	int p_count = 1;
	while((bytes_read = swap_read(sd, buf)) > 0) {
		buf[bytes_read] = '\0';
		printf("p_count: %d\n bytes_read %d\n %s\n", p_count++, bytes_read, buf);
		
	}
	printf("all received from test_swap_server, closing connection\n");
	// close the file and the connection
	swap_close(sd);
}

