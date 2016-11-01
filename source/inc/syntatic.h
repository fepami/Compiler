#ifndef SYNTATIC_H
#define SYNTATIC_H

#include <stdio.h>
#include "token.h"

typedef enum {
    FSM_PROGRAM,
    FSM_VAR_DECLARATION,
    FSM_INSTRUCTION,
    FSM_LOOP,
    FSM_COND,
    FSM_RETURN,
    FSM_EXPR,
    FSM_TERM_AND,
    FSM_TERM_EQUAL,
    FSM_TERM_RELACIONAL,
    FSM_TERM_ADD,
    FSM_TERM_MULT,
    FSM_TERM_PRIMARY,
    FSM_SIZE
} sub_machine_t;

/*
 *      Entry point for compilation
 */
int analyze(FILE *fp);

/*
 *      Internal state functions
 */
int fsm_program(Token* t);
int fsm_var_declaration(Token* t);
int fsm_instruction(Token* t);
int fsm_loop(Token* t);
int fsm_cond(Token* t);
int fsm_return(Token* t);
int fsm_expr(Token* t);
int fsm_term_and(Token* t);
int fsm_term_equal(Token* t);
int fsm_term_relacional(Token* t);
int fsm_term_add(Token* t);
int fsm_term_mult(Token* t);
int fsm_term_primary(Token* t);


#endif