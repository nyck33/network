/*
*	SWAP
*
*		client and server should not exist in the same file,
*			beause the common session_id is used for the both sides.
*
*		packet format:
*			SN(1B) + DATA + CCHECKSUM(2B)
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>


#define	MAXLINE 128	// maximum bytes to receive and send at once
#define	MAXFRAME 256


extern int swap_connect(unsigned int addr, unsigned short port);
extern int swap_accept(unsigned short port);
extern int swap_disconnect(int sd);
extern int sdp_send(int sd, unsigned char *buf, int length);
extern int sdp_receive(int sd, unsigned char *buf);
extern int sdp_receive_with_timer(int sd, unsigned char *buf, unsigned int expiration);

extern unsigned short checksum(char buf[], int length);

static int session_id = 0;
int R = 0;	// frame number to receive

int swap_wait(unsigned short port)
{
	/*
	*	if the session is already open, then return error
	*/

	if (session_id != 0)
		return -1;

	/*
	*	accept a connection
	*/

	session_id = swap_accept(port);	// in sbp.o

	/*
	*	return a ssession id
	*/

	return session_id;
}

// used by the both of client and server.

void swap_close(int sd)
{
	if (session_id == 0 || sd != session_id)
		return;

	else
		session_id = 0;

	swap_disconnect(sd);	// in sbp.o
}

// used by server

int swap_read(int sd, unsigned char buf[])
{
	int	n, m, original_n;
	unsigned char	data_frame[MAXFRAME];	// SEQ_NO(1B) + PAYLOAD + CHECKSUM(2B)
	unsigned char	ack_frame[3];		// SEQ_NO(1B) + CHECKSUM(2B)
	unsigned short	cs;
	int	i;

	while(1) {
		// Receive a frame without a timer

		n = sdp_receive(sd, data_frame);
		original_n = n;
		if (n <= 0)
			return -1;	// something wrong
		else if (n % 2) {	// odd, then need to include the padding before 2B checksum at the end
			data_frame[n] = data_frame[n-1];
			data_frame[n-1] = data_frame[n-2];
			data_frame[n-2] = 0;
			n++;
		}

		/*
		*	several cases
		*/

		// If there is an error
		
		cs = checksum(data_frame, n);
		if (cs != 0) {
			printf("SWAP_READ: wrong checksum, SN=%d, CHECKSUM=%x\n", R, cs);
			continue;
		}

		// If the SEQ_NO is not correct

		if (data_frame[0] != R) {
			printf("SWAP_READ: wrong seq_no %d, R=%d\n", data_frame[0], R);

			ack_frame[0] = R & 0xff;
			ack_frame[1] = 0;	// padding
			cs = checksum(ack_frame, 2);
			ack_frame[1] = (unsigned char)((cs & 0xff00) >> 8);
			ack_frame[2] = (unsigned char)(cs & 0xff);
			m = sdp_send(sd, ack_frame, 3);
			if (m < 0)
				return -1;
			
			continue;
		}

		// If the SEQ_NO is correct
		
		R++;
		if (R > 1)
			R = 0;

		ack_frame[0] = R & 0xff;
		ack_frame[1] = 0;	// padding
		cs = checksum(ack_frame, 2);
		ack_frame[1] = (unsigned char)((cs & 0xff00) >> 8);
		ack_frame[2] = (unsigned char)(cs & 0xff);
		m = sdp_send(sd, ack_frame, 3);
		if (m < 0)
			return -1;

		// Copy the payload field in the frame into buf, and 
		// return the length

		for (i = 0; i < original_n - 3; i++)
			buf[i] = data_frame[i+1];

		return original_n - 3;
	}
}
