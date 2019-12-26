#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

//#include "copy_len_cat.h"

/*take c string as input, make copy in heap, caller takes ownership of new string and must free it
This code is taken from Stanford, Nick Parlante guide to C programming*/
char* MakeStringInHeap(char* source){
    char* newString;

    //demonstrate use of string_length
    newString = (char*) malloc(string_length(source)+1);  //+1 for '\0'
    assert(newString!=NULL);
    //demo use of string_copy
    string_copy(newString, source); //use my function, not strcpy()
    return(newString);
}

//char **argv same as char *argv[]
int main(int argc, char **argv){
    
    //argv[0] is num arguments 
    
    if(argc < 2){
        printf("Usage main1.c {string}");
        return -1;
    }
    //find length of argv[1]
    int len_argv = 0;
    //demo string length, argv[1] or argv[1][0]???
    len_argv = string_length(argv[1]);
    //allocate heap mem for argv[1]
    char* name;
    name = (char*) malloc(len_argv + 1);
    assert(name!=NULL);
    //allocate mem for student number assumes <100
    char number[100];
    int c;
    int count=0;
    printf("Enter your student number, up to 100 characters\n");
    //store chars one by one 
    while((c=getchar())!='\n' && (c!=EOF) && count<100){
        //this should include the null character '\0' since EOF should be -1
        number[count] = c;
        ++count;
    }
    number[count] = '\0';
    
    //print argv[1] and student number on same line
    printf("argv[1]: %s\n length of argv[1]: %d\n student number: %s\n length of student number: %d\n", \
        argv[1], len_argv, number, count);
    
    char *destination= NULL;
    destination = (char *) malloc(len_argv + 1); //+1 for null
    assert(destination!=NULL);
    string_copy(destination, argv[1]);
    printf("%s is at %p\n %s is at %p\n", destination, (void *) &destination, argv[1],(void *) &argv[1]);
    
    //demo string_cat(), shows number twice concatenated
    char * concatenated;
    concatenated = (char*) malloc(len_argv + count +1);
    //copy destination (name) into concatenated including null
    //hold the start
    char *beg_concat = concatenated;
    char *d;
    d = destination;
    while(*d){
        *concatenated = *d;
        concatenated++;
        d++;
    }
    //reset ptr to front
    concatenated = beg_concat;
    //ptr to start of number array
    char *beg_number;
    beg_number = &number[0];
    string_cat(concatenated, beg_number);
    printf("concatenated string %s\n", concatenated);
    free(name);
    free(destination);
    free(concatenated);
    return 0;

}