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

#define MAX_FTA	256  //actual len of buf is max 125 since 3 used for csum, seqnum and mlen
					//here we have more bit == '9' for file name and message len < 123 when last fragment

int main(int argc, char *argv[])
{
    int sd, bytes_read;
    char buf[MAX_FTA];

	//vars for fopen
    int num, out;
    FILE *fp;
    long dest_size;
    char *big_buff;
	char *dest;

	
    if (argc < 3) {
		fprintf(stderr, "fs,Usage: %s port_number file_name\n", argv[0]);
		exit(1);
	}

	sd = swap_wait(htons(atoi(argv[1])));
	if (sd < 0) {
		fprintf(stderr, "fs,%s cannot wait, %d\n", argv[0], sd);
		exit(1);
	}

	//open file for writing
	if ((out = open(argv[2], O_RDWR|O_CREAT,0644)) == -1)
      printf("Can't open output file %s\n", argv[2]);
    else
	  printf("Opened output file: %s\n", argv[2]);
	
	big_buff = (char*)malloc((size_t)MAX_FTA);
	int count = 0;
	int bytes_written = 0;
	int reading = 1;

	//128B - 3 for swap API level flags = 125B
	//loop exits when bytes read < 125 B so messages up to 124 B can be sent without issues.
	while(reading)
	{
		bytes_read = swap_read(sd, buf);
		//if in the last chunk
		if((bytes_read >0) & (bytes_read < (MAX_FTA-3)))
			reading=0;
		printf("rec'd %d B\n", bytes_read);
		
		if(bytes_read > 0){
			bytes_written = write(out, buf, (bytes_read));
		} 
		printf("wrote %d B\n", bytes_written);
		//buf[0]='\0';
	} 
	
	free(big_buff);
	swap_close(sd);
	close(out);
}
	/*
	int reading = 1;
    
	//int dest_size = 0;
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
			num_fragments = (int)buf[1];
			last_block = (int)buf[2];
			destname_len = (int)buf[3];
			printf("fs, numfrag: %d, lastblock: %d, destname_len: %d\n", num_fragments, last_block, destname_len);
			//copy destination name to char dest []
			dest = (char*)malloc((size_t)destname_len+1);
			int i = 0;
			for(i=0; i<destname_len; i++)
			{
				dest[i] = buf[i+4];
				printf("fs, dest[%d]: %c\n", i, dest[i]);
			}
			dest[destname_len] = '\0';
			printf("desname check: %s\n", dest);

			//allocate memory for entire content
			dest_size = (125 * num_fragments) + last_block; 
			printf("125* numfrags +lastblock is: %zu\n", (size_t)dest_size);
			big_buff = (char*)malloc((size_t)dest_size);
    		if(!big_buff)
        		fclose(fp), fputs("fs,memory allocation error\n", stderr), exit(1); 
			printf("fs,dest rec'd: %s, big_buff %zu allocated\n", dest, sizeof(big_buff));
			continue;
		}
		else  //buff[0] is 1 for more fragments or 0 for no more
		{
			if(buf[0] == '0')//message is less than 125B or last block
			{
				int len = (int)buf[1];
				printf("fs,last_block check: %d\n", last_block);
				for(int p=0; p<len+2; p++)
				{
					big_buff[pos] = buf[p+2];
					printf("fs, big_buff[%d]: %c\n", pos, big_buff[pos]);
					pos++;
				}
				reading = 0;
				
			}
			else if (buf[0] == '1') //more than 1 fragment and more coming
			{
				int len = (int)buf[1];
				if(curr_fragment < num_fragments) //read the full 123B
				{
					for(int j=2; j<len+2; j++)
					{
						big_buff[pos] = buf[j];
						pos++;
					}
					printf("fs,read fragment %d\n", curr_fragment);
					curr_fragment++;
					buf[0]='\0';
				}
				else //curr_fragment == num_fragments so read last block
				{
					if(last_block==0) //nothing in last block
					{
						reading=0;
					}
					else
					{
						int len = (int)buf[1];
						for(int j = 2; j< len+2; j++)
						{
							big_buff[pos] = buf[j];
							pos++;
						}
						reading = 0;
						printf("fs,read last block %dB\n", last_block);
					}
				}
			}
		}
	}
	// write big_buff to file, call fopen with dest and fwrite to write
	if((fp = fopen(dest, "wb")) == NULL){
        printf("fs,Error!  Opening file\n");

        exit(1);
    }
	//check big_buff
	printf("big buff is %zu and print\n", sizeof(big_buff));
	for(int z=0; z< dest_size; z++)
	{
		printf("%c", big_buff[z]);
	}
	printf("\n");
	size_t writtenB = fwrite(big_buff, sizeof(char), (size_t)dest_size, fp);
	printf("fwrite returned: %zu\n", writtenB);
	fclose(fp);
	free(big_buff);
	free(dest);
	printf("\nfs,all received from client, closing connection\n");
	// close the file and the connection
	swap_close(sd);
}
*/