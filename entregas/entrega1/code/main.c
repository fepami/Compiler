#include <stdio.h>
#include <stdlib.h>

#include "token.h"
#include "lex.h"

int main(int argc, char **argv) {
    int ret = -1;
    Token* token = NULL;
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
    
    while(1){
        ret = nextToken(fp, &token);
        if(ret == 0)
            break;
        if(ret != 1 || token == NULL){
            printf("lexical analysis failed!\n");
            return -1;
        }
        printToken(token);
        freeToken(&token);
    }
    fclose(fp);
    return 0;
}
