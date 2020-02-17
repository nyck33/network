//fta_server.c

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

#define MAX_FTA	128  //actual len of buf is max 125 since 3 used for csum, seqnum and mlen
					//here we have more bit == '9' for file name and message len < 123 when last fragment

int main(int argc, char *argv[])
{
    int sd, bytes_read;
    char buf[MAX_FTA];

	//vars for fopen
    int num;
    FILE *fp;
    long source_size;
    char *big_buff;
	char dest[32];

    if (argc < 2) {
		fprintf(stderr, "Usage: %s port_number\n", argv[0]);
		exit(1);
	}

	sd = swap_wait(htons(atoi(argv[1])));
	if (sd < 0) {
		fprintf(stderr, "%s cannot wait, %d\n", argv[0], sd);
		exit(1);
	}

	int reading = 1;
    
	int dest_size = 0;
	int num_fragments = 0;
	int curr_fragment = 0;
	int last_block = 0;
	int destname_len = 0;
	int pos = 0;
	while(reading)
	{
		bytes_read = swap_read(sd, buf); 
		
		//check more bit if 9 filename
		if(buf[0] == '9')
		{
			num_fragments = buf[1];
			last_block = buf[2];
			destname_len = buf[3];
			//copy destination name to char dest []
			int i = 0;
			for(i=0; i<destname_len; i++)
			{
				dest[i] = buf[i+4];
			}
			dest[i+1] = '\0';

			//allocate memory for entire content
			dest_size = (125 * num_fragments) + last_block; 
			big_buff = calloc(1, source_size+1);
    		if(!big_buff)
        		fclose(fp), fputs("memory allocation error", stderr), exit(1); 
			
			continue;
		}
		else  //
		{
			if(num_fragments==0)//message is less than 125B 
			{
				for(pos=0; pos<last_block; pos++)
				{
					big_buff[pos] = buf[pos+2];
				}
				reading = 0;
			}
			else //more than 1 fragment
			{
				if(curr_fragment < num_fragments) //read the full 123
				{
					for(int j =0; j<123; j++)
					{
						big_buff[pos++] = buf[j+2];
					}
					curr_fragment++;
				}
				else //curr_fragment == num_fragments so read last block
				{
					if(last_block==0) //nothing in last block
					{
						reading=0;
					}
					else
					{
						for(int j = 0; j< last_block; j++)
						{
							big_buff[pos++] = buf[j+2];
						}
						reading = 0;
					}
				}
			}
		}
	}
	// write big_buff to file, call fopen with dest and fwrite to write
	printf("all received from test_swap_server, closing connection\n");
	// close the file and the connection
	swap_close(sd);
}