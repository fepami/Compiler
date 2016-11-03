
#include <stdlib.h>
#include <string.h>

#include "syntatic.h"
#include "util.h"
#include "token.h"
#include "lex.h"

SyntaticState state;
Stack stack;

int (* const subMachines[SUB_SIZE]) (Token* t) = {
    subPrograma,
    subDefinicao,
    subFuncao,
    subEscopo,
    subVariavel,
    subCondicional,
    subRepeticao,
    subComando,
    subExpressao,
    subTermoDecimal,
    subTermoComp,
    subTermoPrimario
};

void semantic_tbd(){
    printf("TODO\n");
}

void push(Stack* s, SubMachine subMachine, int retState) {
    StackNode* sn = (StackNode*) malloc(sizeof(StackNode));
    sn->next = s->top;
    sn->subMachine = subMachine;
    sn->state = retState;
    s->top = sn;
    s->size++;

    state.getToken = 0;
}

void pop(Stack* s) {
    if(s->size <= 0) {
        state.currentSubMachineState = -1;
        return;
    }

    state.currentSubMachine= s->top->subMachine;
    state.currentSubMachineState= s->top->state;

    s->size--;
    StackNode* aux = s->top;
    s->top = s->top->next;
    free(aux);
    state.getToken = 0;
}

void cleanUp(){
    freeToken(&state.lastToken);
    freeToken(&state.currentToken);
    while(stack.size > 1){
        stack.size--;
        StackNode* aux = stack.top;
        stack.top = stack.top->next;
        free(aux);
    }
    free(stack.top);
}

int callSubMachine(SubMachine subMach, int retSt) {
    push(&stack, state.currentSubMachine, retSt);
    state.currentSubMachine = subMach;
    return 0;
}

int returnSubMachine(){
    pop(&stack);
    return state.currentSubMachineState;
}

int compile(FILE* fp) {
    int ret;
    stack.size = 0;
    stack.top = NULL;
    state.getToken = 1;
    state.currentSubMachineState = 0;
    state.currentSubMachine = 0;
    state.currentToken = NULL;
    state.lastToken = NULL;

    while(1) {
        if(state.getToken) {
            freeToken(&state.lastToken);
            state.lastToken = state.currentToken;
            if((ret = nextToken(fp, &state.currentToken)) == 0)
                break;
            printToken(state.currentToken);
        }

        state.getToken = 1;

        state.currentSubMachineState= subMachines[state.currentSubMachine](state.currentToken);
        if(state.currentSubMachineState == -1) {
            printf("Erro de compilação!\n");
            cleanUp();
            return 1;
        }
    }

    printf("Compilação completa!\n");
    cleanUp();
    return 0;
}


int isType(Token* t) {
    if(t->tokenClass == CLASS_RESERVED_WORD && (strcmp(t->value.stringValue, "int") == 0
        || strcmp(t->value.stringValue, "float") == 0 || strcmp(t->value.stringValue, "bool") == 0
        || strcmp(t->value.stringValue, "string") == 0 )){
        return 1;
    }else{
        return 0;
    }
}

int subPrograma(Token* t){
    switch (state.currentSubMachineState) {
        case 0:
            if(t->tokenClass == CLASS_RESERVED_WORD && strcmp(t->value.stringValue, "program") == 0) {
                semantic_tbd();
                return 1;
            }
            break;
        case 1:
            return callSubMachine(SUB_ESCOPO, 2);
        case 2:
            return returnSubMachine();
    }

    return -1;
}
int subDefinicao(Token* t){
    switch (state.currentSubMachineState) {
        case 0:
            if(t->tokenClass == CLASS_RESERVED_WORD && strcmp(t->value.stringValue, "deftype") == 0) {
                semantic_tbd();
                return 1;
            }
            break;
        case 1:
            if (t->tokenClass == CLASS_IDENTIFIER) {
                semantic_tbd();
                return 2;
            }
        case 2:
            if (t->tokenClass == CLASS_DELIMITER && t->value.charValue == '{') {
                semantic_tbd();
                return 3;
            }
            break;
        case 3:
            if (isType(t)) {
                semantic_tbd();
                return 4;
            }
            break;
        case 4:
            if (t->tokenClass == CLASS_IDENTIFIER) {
                semantic_tbd();
                return 5;
            }
            break;
        case 5:
            if (t->tokenClass == CLASS_DELIMITER && t->value.charValue == ',') {
                semantic_tbd();
                return 3;
            } else if (t->tokenClass == CLASS_DELIMITER && t->value.charValue == '}') {
                semantic_tbd();
                return 6;
            }
            break;
        case 6:
            if (t->tokenClass == CLASS_DELIMITER && t->value.charValue == ';') {
                semantic_tbd();
                return 6;
            }
            break;
        case 7:
            return returnSubMachine();
    }
    return -1;
}
int subFuncao(Token* t){
    switch (state.currentSubMachineState) {
        case 0:
            if (t->tokenClass == CLASS_RESERVED_WORD && strcmp(t->value.stringValue, "function") == 0) {
                semantic_tbd();
                return 1;
            }
            break;
        case 1:
            if (t->tokenClass == CLASS_IDENTIFIER) {
                semantic_tbd();
                return 2;
            }
            break;
        case 2:
            if (t->tokenClass == CLASS_DELIMITER && t->value.charValue == '(') {
                semantic_tbd();
                return 3;
            }
            break;
        case 3:
            if (isType(t)) {
                semantic_tbd();
                return 4;
            }
            break;
        case 4:
            if (t->tokenClass == CLASS_IDENTIFIER) {
                semantic_tbd();
                return 5;
            }
            break;
        case 5:
            if (t->tokenClass == CLASS_DELIMITER && t->value.charValue == ',') {
                semantic_tbd();
                return 3;
            } else if (t->tokenClass == CLASS_DELIMITER && t->value.charValue == ')') {
                semantic_tbd();
                return 6;
            }
            break;
        case 6:
            if (t->tokenClass == CLASS_DELIMITER && t->value.charValue == ':') {
                semantic_tbd();
                return 8;
            } else {
                return callSubMachine(SUB_ESCOPO, 7);
            }
            break;
        case 7:
            return returnSubMachine();
        case 8:
            if (isType(t)) {
                semantic_tbd();
                return 9;
            }
            break;
        case 9:
            return callSubMachine(SUB_ESCOPO, 7);
    }
    return -1;
}
int subEscopo(Token* t){
    switch (state.currentSubMachineState) {
        case 0:
            if (t->tokenClass == CLASS_DELIMITER && t->value.charValue == '{') {
                semantic_tbd();
                return 1;
            }
            break;
        case 1:
            if (t->tokenClass == CLASS_DELIMITER && t->value.charValue == '}') {
                semantic_tbd();
                return 2;
            } else {
                return callSubMachine(SUB_COMANDO, 1);
            }
            break;
        case 2:
            return callSubMachine(SUB_ESCOPO, 7);
    }
    return -1;
}
int subVariavel(Token* t){
    switch (state.currentSubMachineState) {
        case 0:
            if (isType(t)) {
                semantic_tbd();
                return 1;
            }
            break;
        case 1:
            if (t->tokenClass == CLASS_IDENTIFIER) {
                semantic_tbd();
                return 2;
            } else if (t->tokenClass == CLASS_DELIMITER && t->value.charValue == '[') {
                semantic_tbd();
                return 3;
            }
            break;
        case 2:
            if (t->tokenClass == CLASS_OPERATOR && strcmp(t->value.stringValue, "=") == 0) {
                semantic_tbd();
                return 4;
            } else if (t->tokenClass == CLASS_DELIMITER && t->value.charValue == ';') {
                semantic_tbd();
                return 5;
            } else if (t->tokenClass == CLASS_DELIMITER && t->value.charValue == ',') {
                semantic_tbd();
                return 6;
            }
            break;
        case 3:
            if (t->tokenClass == CLASS_DELIMITER && t->value.charValue == ']') {
                semantic_tbd();
                return 7;
            } else if (t->tokenClass == CLASS_INT){
                semantic_tbd();
                return 8;
            }
            break;
        case 4:
            return callSubMachine(SUB_EXPRESSAO, 10);
        case 5:
            return returnSubMachine();
        case 6:
            if (t->tokenClass == CLASS_IDENTIFIER) {
                semantic_tbd();
                return 9;
            }
            break;
        case 7:
            if (t->tokenClass == CLASS_IDENTIFIER) {
                semantic_tbd();
                return 11;
            }
            break;
        case 8:
            if (t->tokenClass == CLASS_DELIMITER && t->value.charValue == ']') {
                semantic_tbd();
                return 6;
            }
            break;
        case 9:
            if (t->tokenClass == CLASS_DELIMITER && t->value.charValue == ';') {
                semantic_tbd();
                return 5;
            } else if (t->tokenClass == CLASS_DELIMITER && t->value.charValue == ',') {
                semantic_tbd();
                return 6;
            }
            break;
        case 10:
            if (t->tokenClass == CLASS_DELIMITER && t->value.charValue == ';') {
                semantic_tbd();
                return 5;
            }
            break;
        case 11:
            if (t->tokenClass == CLASS_OPERATOR && strcmp(t->value.stringValue, "=") == 0) {
                semantic_tbd();
                return 12;
            }
            break;
        case 12:
            if (t->tokenClass == CLASS_DELIMITER && t->value.charValue == '{') {
                semantic_tbd();
                return 13;
            }
            break;
        case 13:
            callSubMachine(SUB_EXPRESSAO, 14);
            break;
        case 14:
            if (t->tokenClass == CLASS_DELIMITER && t->value.charValue == ',') {
                semantic_tbd();
                return 13;
            } else if (t->tokenClass == CLASS_DELIMITER && t->value.charValue == '}') {
                semantic_tbd();
                return 10;
            }
            break;
    }
    return -1;
}
int subCondicional(Token* t){
    switch (state.currentSubMachineState) {
        case 0:
            if (t->tokenClass == CLASS_DELIMITER && t->value.charValue == '(') {
                semantic_tbd();
                return 1;
            } else if (t->tokenClass == CLASS_RESERVED_WORD && strcmp(t->value.stringValue, "if") == 0) {
                semantic_tbd();
                return 2;
            }
            break;
        case 1:
            return callSubMachine(SUB_EXPRESSAO, 3);
        case 2:
            if (t->tokenClass == CLASS_DELIMITER && t->value.charValue == '(') {
                semantic_tbd();
                return 4;
            }
            break;
        case 3:
            if (t->tokenClass == CLASS_DELIMITER && t->value.charValue == ')') {
                semantic_tbd();
                return 5;
            }
            break;
        case 4:
            return callSubMachine(SUB_EXPRESSAO, 6);
        case 5:
            return callSubMachine(SUB_ESCOPO, 7);
        case 6:
            if (t->tokenClass == CLASS_DELIMITER && t->value.charValue == ')') {
                semantic_tbd();
                return 8;
            }
            break;
        case 7:
            return returnSubMachine();
        case 8:
            return callSubMachine(SUB_ESCOPO, 9);
        case 9:
            if (t->tokenClass == CLASS_RESERVED_WORD && strcmp(t->value.stringValue, "elsif") == 0) {
                semantic_tbd();
                return 2;
            } else if (t->tokenClass == CLASS_RESERVED_WORD && strcmp(t->value.stringValue, "else") == 0) {
                semantic_tbd();
                return 5;
            }
            break;
    }
    return -1;
}
int subRepeticao(Token* t){
    switch (state.currentSubMachineState) {
        case 0:
            if (t->tokenClass == CLASS_RESERVED_WORD && strcmp(t->value.stringValue, "while") == 0) {
                semantic_tbd();
                return 1;
            }
            break;
        case 1:
            if (t->tokenClass == CLASS_DELIMITER && t->value.charValue == '(') {
                semantic_tbd();
                return 2;
            }
            break;
        case 2:
            return callSubMachine(SUB_EXPRESSAO, 3);
        case 3:
            if (t->tokenClass == CLASS_DELIMITER && t->value.charValue == ')') {
                semantic_tbd();
                return 4;
            }
            break;
        case 4:
            return callSubMachine(SUB_ESCOPO, 5);
        case 5:
            return returnSubMachine();
    }
    return -1;
}
int subComando(Token* t){
    switch (state.currentSubMachineState) {
        case 0:
            if (t->tokenClass == CLASS_RESERVED_WORD) {
                if (strcmp(t->value.stringValue, "return") == 0) {
                    semantic_tbd();
                    return 2;
                } else if (strcmp(t->value.stringValue, "if") == 0) {
                    semantic_tbd();
                    return 4;
                } else if (strcmp(t->value.stringValue, "var") == 0 || strcmp(t->value.stringValue, "persistent") == 0
                        || strcmp(t->value.stringValue, "const") == 0) {
                    // DEF_VARIAVEL
                    semantic_tbd();
                    return 3;
                } else if (strcmp(t->value.stringValue, "while") == 0) {
                    semantic_tbd();
                    return 5;
                }
            } else if (t->tokenClass == CLASS_IDENTIFIER) {
                semantic_tbd();
                return 1;
            }
            break;
        case 1:
            if (t->tokenClass == CLASS_OPERATOR && strcmp(t->value.stringValue, "=") == 0) {
                semantic_tbd();
                return 2;
            } else if (t->tokenClass == CLASS_DELIMITER && t->value.charValue == '(') {
                semantic_tbd();
                return 6;
            }
            break;
        case 2:
            return callSubMachine(SUB_EXPRESSAO, 9);
        case 3:
            return callSubMachine(SUB_VARIAVEL, 7);
        case 4:
            return callSubMachine(SUB_CONDICIONAL, 7);
        case 5:
            return callSubMachine(SUB_REPETICAO, 7);
        case 6:
            if (t->tokenClass == CLASS_DELIMITER && t->value.charValue == ')') {
                semantic_tbd();
                return 9;
            } else {
                return callSubMachine(SUB_EXPRESSAO, 8);
            }
            break;
        case 7:
            return returnSubMachine();
        case 8:
            if (t->tokenClass == CLASS_DELIMITER && t->value.charValue == ',') {
                semantic_tbd();
                return 10;
            } else if (t->tokenClass == CLASS_DELIMITER && t->value.charValue == ')') {
                semantic_tbd();
                return 9;
            }
            break;
        case 9:
            if (t->tokenClass == CLASS_DELIMITER && t->value.charValue == ';') {
                semantic_tbd();
                return 7;
            }
            break;
        case 10:
            return callSubMachine(SUB_EXPRESSAO, 8);
    }
    return -1;
}
int subExpressao(Token* t){
    switch (state.currentSubMachineState) {
        case 0:
            return callSubMachine(SUB_TERMODECIMAL, 1);
        case 1:
            if(t->tokenClass == CLASS_OPERATOR &&
                (  strcmp(t->value.stringValue, "+" ) == 0 || strcmp(t->value.stringValue, "-") == 0
                || strcmp(t->value.stringValue, "/" ) == 0 || strcmp(t->value.stringValue, "*" ) == 0)){
                semantic_tbd();
                return 2;
            }else{
                return returnSubMachine();
            }
        case 2:
            return callSubMachine(SUB_EXPRESSAO, 3);
        case 3:
            return returnSubMachine();
    }
    return -1;
}
int subTermoDecimal(Token* t){
    switch (state.currentSubMachineState) {
        case 0:
            return callSubMachine(SUB_TERMOCOMP, 1);
        case 1:
            if(t->tokenClass == CLASS_OPERATOR &&
                (  strcmp(t->value.stringValue, "&" ) == 0 || strcmp(t->value.stringValue, "|") == 0
                || strcmp(t->value.stringValue, "^" ) == 0 )){
                semantic_tbd();
                return 2;
            }else{
                return returnSubMachine();
            }
            break;
        case 2:
            return callSubMachine(SUB_TERMODECIMAL, 3);
        case 3:
            return returnSubMachine();
    }
    return -1;
}
int subTermoComp(Token* t){
    switch (state.currentSubMachineState) {
        case 0:
            return callSubMachine(SUB_TERMOPRIMARIO, 1);
        case 1:
            if(t->tokenClass == CLASS_OPERATOR &&
                (  strcmp(t->value.stringValue, "<" ) == 0 || strcmp(t->value.stringValue, "<=") == 0
                || strcmp(t->value.stringValue, ">" ) == 0 || strcmp(t->value.stringValue, ">=") == 0
                || strcmp(t->value.stringValue, "==") == 0 || strcmp(t->value.stringValue, "!=") == 0
                || strcmp(t->value.stringValue, "&&") == 0 || strcmp(t->value.stringValue, "||") == 0 )){
                semantic_tbd();
                return 2;
            }else{
                return returnSubMachine();
            }
            break;
        case 2:
            return callSubMachine(SUB_TERMOCOMP, 3);
        case 3:
            return returnSubMachine();
    }
    return -1;
}

int subTermoPrimario(Token* t){
    switch (state.currentSubMachineState) {
        case 0:
            if(t->tokenClass == CLASS_IDENTIFIER){
                semantic_tbd();
                return 1;
            }else if(t->tokenClass == CLASS_DELIMITER && t->value.charValue == '('){
                semantic_tbd();
                return 2;
            }else if(t->tokenClass == CLASS_INT || t->tokenClass == CLASS_FLOAT || t->tokenClass == CLASS_STRING_LIT){
                semantic_tbd();
                return 3;
            }
            break;
        case 1:
            if(t->tokenClass == CLASS_DELIMITER && t->value.charValue == '('){
                semantic_tbd();
                return 4;
            }else{
                return returnSubMachine();
            }
            break;
        case 2:
            return callSubMachine(SUB_EXPRESSAO, 8);
        case 3:
            return returnSubMachine();
        case 4:
            if(t->tokenClass == CLASS_DELIMITER && t->value.charValue == ')'){
                semantic_tbd();
                return 3;
            }else{
                return callSubMachine(SUB_EXPRESSAO, 5);
            }
            break;
        case 5:
            if(t->tokenClass == CLASS_DELIMITER && t->value.charValue == ')'){
                semantic_tbd();
                return 3;
            }else if(t->tokenClass == CLASS_DELIMITER && t->value.charValue == ','){
                semantic_tbd();
                return 6;
            }
            break;
        case 6:
            return callSubMachine(SUB_EXPRESSAO, 5);
        case 7:
            if(t->tokenClass == CLASS_DELIMITER && t->value.charValue == ')'){
                semantic_tbd();
                return 3;
            }
            break;

    }
    return -1;
}
