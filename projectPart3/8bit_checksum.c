#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>


#define	MAXLINE 128	// maximum characters to receive and send at once
#define	MAXFRAME 256

/*my 8 bit checksum based on Wikipedia
As an example, the data may be a message to be transmitted consisting of 136 characters, 
each stored as an 8-bit byte, making a data word of 1088 bits in total. 
A convenient block size would be 8 bits, although this is not required. 
Similarly, a convenient modulus would be 255, although, again, others could be chosen. 
So, the simple checksum is computed by adding together all the 8-bit bytes of the message, 
dividing by 255 and keeping only the remainder. (In practice, the modulo operation is performed 
during the summation to control the size of the result.) */

//https://stackoverflow.com/questions/26478827/8-bit-checksum-with-wraparound-in-c
unsigned char checksum(unsigned char message [], int num_bytes)
{
    
    unsigned char sum = 0;

    //loop through (decrement and compare original with 0) and sum
    while (num_bytes-- > 0)
    {   
        //if expr 1 true, return expr 2 else return expr 3
        int carry = (sum + *message > 255) ? 1:0;
        sum += *(message++) + carry;
    }
    return (~sum);
}
//unsigned 0 to 255, signed, -127 to 127
char checksum_simple(char message2 [], int num_bytes)
{

    char sum = 0;

    while(num_bytes-- > 0)
    {
        sum += *(message2++);
    }

    char remainder = sum % 255;

    return remainder;
    //return (sum);
}
//need to use unsigned type for wrapping and shifting
//int chksum8(const unsigned char *buff)

int main(int argc, char* argv[]){

    if(argc <3){
        printf("usage ./program {message} {message2} ");
        return 1;
    }

    //char temp [] = argv[1];

    unsigned char message [strlen(argv[1]+1)]; //malloc((strlen(argv[1])+1) * sizeof(unsigned char));
    char message2[strlen(argv[2]+1)]; 
    //strlen no \0
    memcpy(message, argv[1], strlen(argv[1])+1);
    memcpy(message2, argv[2], strlen(argv[2]+1));

    int num_bytes = strlen((char*)message);  //+1;
    int num_bytes2 = strlen(message2);  //+1;

    char simple_ans = checksum_simple(message2, num_bytes2);
    //char simple_ans_conv = (char)simple_ans;

    unsigned char ans = checksum(message, num_bytes);
    //char ans_conv = (char)ans;
    int ans_len = sizeof(ans);

    printf("\nans: %u\n simple ans: %d\n", (unsigned)ans, (int)simple_ans);
    printf("\nans conv: %d\n simple ans_conv %c \n", (int)(char)ans, (char)simple_ans);
    printf("len of ans: %d\n", ans_len);
    printf("conversion_test: %u\n", (unsigned)(unsigned char)(char)ans);
    
    return 0;

}