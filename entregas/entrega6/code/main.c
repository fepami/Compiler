#include <stdio.h>
#include <stdlib.h>

#include "token.h"
#include "syntatic.h"

int main(int argc, char **argv) {
    FILE *fp = NULL;
    
    if(argc != 2){
        printf("Usage: lex <file>\n");
        return -1;
    }
    
    fp = fopen(argv[1], "r");
    if(fp == NULL){
        printf("Could not open file: %s\n", argv[1]);
        return -1;
    }
    
    compile(fp);
    
    fclose(fp);
    return 0;
}
