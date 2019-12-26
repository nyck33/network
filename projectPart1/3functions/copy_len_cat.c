#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


//count string length, null character not counted
int string_length (char *s){
    printf("in string_length\n");
    int count = 0;
    while((*s!='\0') && (*s!='\n')){
        count++;
        s++;
    }
    return count;
}

// copy string from source to dest
// null character is copied
void string_copy(char *destination, char *source){
    printf("in string_copy\n");
    //set ptr to start of destination
    char* original;
    original = destination;
    //copy source to copy
    while(*source){
        *destination = *source;
        destination++;
        source++;
    }
    *destination = '\0';
    destination = original;
    printf("in string_copy, copy is %s\n", destination);
}

//concatenate source to end of dest, null dropped from destination but left on source
//destination is malloced to include size of both char arrays with destination copied to front of array
void string_cat (char *destination, char *source){
    printf("in string_cat\n");
    //set temp ptr to start
    char *beg_dest;
    beg_dest = destination;
    //concatenate to total
    while(*destination!='\0'){
        destination++;
    }
    while(*source){
        *destination = *source;
        destination++;
        source++;
    }
    //reset ptr to start of total
    destination = beg_dest;
    
    printf("concatenated string: %s\n", destination);
    
}
