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
    printf("Classe do Token: %s\t", classToString(t->tokenClass));
    if(t->tokenClass == CLASS_INT) {
        printf("Valor do Token: %d\n", t->value.intValue);
    } else if(t->tokenClass == CLASS_FLOAT) {
        printf("Valor do Token: %f\n", t->value.floatValue);
    } else if(t->tokenClass == CLASS_DELIMITER) {
        printf("Valor do Token: %c\n", t->value.charValue);
    } else {
        printf("Valor do Token: %s\n", t->value.stringValue);
    }
}

Token* newToken(TokenClass tokenClass, void* data)
{
    char* newString;
    Token* token = (Token *) malloc(sizeof(Token));
    memset(token, 0, sizeof(Token));

    if (token == NULL)
        return NULL;

    token->tokenClass = tokenClass;
    if(tokenClass == CLASS_INT){
        token->value.intValue = strtol(data, NULL, 10);
    }else if(tokenClass == CLASS_FLOAT){
        token->value.floatValue = strtof(data, NULL);
    }else if(tokenClass == CLASS_DELIMITER){
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

    if(token->tokenClass == CLASS_STRING_LIT || token->tokenClass == CLASS_IDENTIFIER ||
        token->tokenClass == CLASS_RESERVED_WORD || token->tokenClass == CLASS_OPERATOR)
        free(token->value.stringValue);

    free(token);
}
