#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>  //
#include <arpa/inet.h>  //


#define	MAXLINE 128	// maximum characters to receive and send at once
#define	MAXFRAME 256

extern int swap_connect(unsigned int addr, unsigned short port);
extern int swap_disconnect(int sd);
extern int sdp_send(int sd, char *buf, int length);
//writes data of length stored in buf to session sd, max size to send is 256B, returns num_bytes written to sd or -1
extern int sdp_receive(int sd, char *buf);
extern int sdp_receive_with_timer(int sd, char *buf, unsigned int expiration);

unsigned char checksum(unsigned char message [], int num_bytes)
