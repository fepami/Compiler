#ifndef SYNTATIC_H
#define SYNTATIC_H

#include <stdio.h>
#include "token.h"

typedef enum {
    FSM_PROGRAMA,
    FSM_DEFINICAO,
    FSM_FUNCAO,
    FSM_ESCOPO,
    FSM_VARIAVEL,
    FSM_CONDICIONAL,
    FSM_REPETICAO,
    FSM_COMANDO,
    FSM_EXPRESSAO,
    FSM_TERMODECIMAL,
    FSM_TERMOCOMP,
    FSM_TERMOPRIMARIO
} sub_machine_t;

/*
 *      Entry point for compilation
 */
int analyze(FILE *fp);

/*
 *      Internal state functions
 */
int fsm_programa(Token* t);
int fsm_definicao(Token* t);
int fsm_funcao(Token* t);
int fsm_escopo(Token* t);
int fsm_variavel(Token* t);
int fsm_condicional(Token* t);
int fsm_repeticao(Token* t);
int fsm_comando(Token* t);
int fsm_expressao(Token* t);
int fsm_termodecimal(Token* t);
int fsm_termocomp(Token* t);
int fsm_termoprimario(Token* t);

#endif