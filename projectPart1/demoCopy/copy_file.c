/*program to copy files using system calls, commented out int main() is from the Kerrigan C book, also GeeksforGeeks 
was heavily referred to here:  https://www.geeksforgeeks.org/input-output-system-calls-c-create-open-close-read-write/
in addition to stackoverflow.com and https://www.unix.com/programming/105575-copy-file-using-unix-system-calls.html*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h> //system calls if sys/syscalls.h does not work
#include <stdarg.h>

#define PERMS 0666 /* RW for owner, group, others, cannot execute*/

//This is an imported function from unistd.h I think
void error(char *, ...);

/* cp: copy f1 to f2 */
int main(int argc, char *argv[]){
    int f1, f2, n;
    char buf[BUFSIZ]; //BUFSIZE
    if (argc != 3)
        error("Usage: cp from to");
    if ((f1 = open(argv[1], O_RDONLY, 0)) == -1)
        error("cp: can't open %s", argv[1]);
    if ((f2 = creat(argv[2], PERMS)) == -1)
        error("cp: can't create %s, mode %03o",argv[2], PERMS);
    //read BUFSIZ bites into buf from f1, returns num bytes read = file position, 0 is EOF,
    while ((n = read(f1, buf, BUFSIZ)) > 0)
        if (write(f2, buf, n) != n)
            error("cp: write error on file %s", argv[2]);
    return 0;
}

/* error: print an error message and die */
void error(char *fmt, ...){
    va_list args;

    va_start(args, fmt);
    fprintf(stderr, "error: ");
    vprintf(fmt, args);
    fprintf(stderr, "\n");
    va_end(args);
    exit(1);
}


