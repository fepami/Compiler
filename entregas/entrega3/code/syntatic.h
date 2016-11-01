#ifndef SYNTATIC_H
#define SYNTATIC_H

#include <stdio.h>
#include "token.h"

typedef enum {
    SUB_PROGRAMA,
    SUB_DEFINICAO,
    SUB_FUNCAO,
    SUB_ESCOPO,
    SUB_VARIAVEL,
    SUB_CONDICIONAL,
    SUB_REPETICAO,
    SUB_COMANDO,
    SUB_EXPRESSAO,
    SUB_TERMODECIMAL,
    SUB_TERMOCOMP,
    SUB_TERMOPRIMARIO,
    SUB_SIZE
} SubMachine;

typedef struct SyntaticState{
    int currentSubMachineState;
    SubMachine currentSubMachine;
    Token* currentToken;
    Token* lastToken;
    int getToken;
} SyntaticState;

typedef struct StackNode{
    SubMachine subMachine;
    int state;
    struct StackNode* next;
} StackNode;

typedef struct Stack{
    StackNode* top;
    int size;
} Stack;

int compile(FILE *fp);

int subPrograma(Token* t);
int subDefinicao(Token* t);
int subFuncao(Token* t);
int subEscopo(Token* t);
int subVariavel(Token* t);
int subCondicional(Token* t);
int subRepeticao(Token* t);
int subComando(Token* t);
int subExpressao(Token* t);
int subTermoDecimal(Token* t);
int subTermoComp(Token* t);
int subTermoPrimario(Token* t);

#endif
