#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "token.h"

const char* classToString(const TokenClass c)
{
    switch(c) {
    case CLASS_INT:
        return "Inteiro            ";
    case CLASS_FLOAT:
        return "Decimal            ";
    case CLASS_STRING_LIT:
        return "String Literal     ";
    case CLASS_IDENTIFIER:
        return "Identificador      ";
    case CLASS_RESERVED_WORD:
        return "Palavra Reservada: ";
    case CLASS_OPERATOR:
        return "Operador           ";
    case CLASS_DELIMITER:
        return "Delimitador        ";
    default:
        return "UNDEFINED          ";
    }
}

void printToken(const Token* t)
{
    printf("Classe do Token: %s\t", classToString(t->class));
    if(t->class == CLASS_INT) {
        printf("Valor do Token: %d\n", t->value.intValue);
    } else if(t->class == CLASS_FLOAT) {
        printf("Valor do Token: %f\n", t->value.floatValue);
    } else if(t->class == CLASS_DELIMITER) {
        printf("Valor do Token: %c\n", t->value.charValue);
    } else {
        printf("Valor do Token: %s\n", t->value.stringValue);
    }
}

Token* newToken(TokenClass class, void* data)
{
    char* newString;
    Token* token = (Token *) malloc(sizeof(Token));
    memset(token, 0, sizeof(Token));
    
    if (token == NULL) 
        return NULL;
    
    token->class = class;
    if(class == CLASS_INT){
        token->value.intValue = strtol(data, NULL, 10);
    }else if(class == CLASS_FLOAT){
        token->value.floatValue = strtof(data, NULL);
    }else if(class == CLASS_DELIMITER){
        token->value.charValue = *((char *) data);
    }else {
        newString = malloc(strlen(data) + 1);
        if (newString == NULL){
            free(token);
            return NULL;
        }
        strcpy(newString, data);
        token->value.stringValue = newString;
    }
    
    return token;
}

void freeToken(Token** t)
{
    Token* token = *t;
    if(t == NULL || token == NULL)
        return;
    
    if(token->class == CLASS_STRING_LIT || token->class == CLASS_IDENTIFIER ||
        token->class == CLASS_RESERVED_WORD || token->class == CLASS_OPERATOR)
        free(token->value.stringValue);
    
    free(token);
}
