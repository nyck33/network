/*program to copy files using system calls, commented out int main() is from the Kerrigan C book, also GeeksforGeeks 
was heavily referred to here:  https://www.geeksforgeeks.org/input-output-system-calls-c-create-open-close-read-write/
in addition to stackoverflow.com and https://www.unix.com/programming/105575-copy-file-using-unix-system-calls.html
Other sources:  man pages and Unix permissions calculator 
compile command: gcc -Wall copy_syscalls.c -o syscalls 
run command: ./syscalls source.txt destination.txt*/

#include <stdio.h>
#include <stdlib.h>
//three includes to use open
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h> //system calls if sys/syscalls.h does not work
//this give specific error code
#include <errno.h>
extern int errno;

void check_same(char *argv1, char *argv2);

/*open the read file, open a write file, create if it doesn't exist, read from 0 and write to 1*/
int main(int argc, char* argv[]){
    //mimic fd[0] for read and fd[1] for write
    int fd0, fd1; 
    //return value from read greater than 0 is ok, 0 is EOF, <0 is an error
    int num_bytes;
    //BUFSIZ is 4096 or can be set to 1024, etc but larger is more efficient (bytes)
    char buf[BUFSIZ]; //char *buf for read and write, tracks file pos.
    
    if(argc !=3){
        printf("Usage binary {source} {destination}\n");
        exit(1);
    }
    if((fd0 = open(argv[1], O_RDONLY, 0555))==-1){
        printf("Cannot open read file, %d\n", errno);
        exit(1);
    }
    if((fd1 = open(argv[2], O_WRONLY | O_CREAT, 0755))==-1){ //create if not exist, don't create if exists
        printf("Cannot open write file, %d\n", errno);
        exit(1);
    }
    while((num_bytes = read(fd0, buf, BUFSIZ))>0){ //0 is EOF, -1 is error, returns bytes read = file pos.
        if((write(fd1, buf, num_bytes))!=num_bytes){
            printf("Write error %d\n", num_bytes);
            exit(1);
        }
    }

    //close files, 0 success -1 fail
    if((close(fd0)) < 0){
        printf("Read file not closed, %d\n", errno);
        exit(1);
    }
    if((close(fd1)) < 0){
        printf("Write file not closed, %d\n", errno);
        exit(1);
    }

    //compare 2 files for sameness
    check_same(argv[1], argv[2]);

    return 0;
}

/* check that files are the same from: 
https://stackoverflow.com/questions/22641843/compare-2-files-using-posix-system-calls*/
void check_same(char *argv1, char *argv2){
    printf("in check_same\n");
    //to open files and count bytes read
    int f0, f1;
    int count =0;
    //position in each file
    int pos1, pos2;
    //buffer size 1 to check each char
    char orig[1];
    char copy[1];

    if((f0 = open(argv1, O_RDONLY, 0555))==-1){
        printf("Cannot open read file, %d\n", errno);
        exit(1);
    }
    if((f1 = open(argv2, O_RDONLY, 0555))==-1){
        printf("Cannot open read file, %d\n", errno);
        exit(1);
    }
    while(1){
        pos1=read(f0, orig, 1);  
        pos2=read(f1, copy, 1);

        //check for errors
        if(pos1 <0 || pos2 <0){
            printf("Error pos1 %d, pos2 %d", pos1, pos2);
            exit(1);
        }

        if((orig[0] != copy[0]) || (pos1 != pos2)){
            printf("Files differ at read pos: %d, char: %c\n write pos: %d, char: %c\n",\
                    pos1, orig[0], pos2, copy[0]);
        }
        if(count%10==0){
            printf("Success, total %d chars same\n", count);
        }

        //exit condition
        if(pos1==0 && pos2==0){
            printf("%s is the same as %s\n", argv1, argv2);
            return;
        }
        count++;
    }
    return;
}