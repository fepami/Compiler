
#include <stdlib.h>
#include <string.h>

#include "syntatic.h"
#include "util.h"
#include "token.h"
#include "lex.h"

/*
 *      Structure that represents the state of the analysis
 */
typedef struct {
    int current_sub_machine_state;
    sub_machine_t current_sub_machine;
    Token* curr_token;
    Token* last_token;
    int get_token_flag;
} analysis_state_t;

/*
 *      State function table
 */
int (* const sub_machines[FSM_SIZE]) (Token* t) = {
    fsm_programa,
    fsm_definicao,
    fsm_funcao,
    fsm_escopo,
    fsm_variavel,
    fsm_condicional,
    fsm_repeticao,
    fsm_comando,
    fsm_expressao,
    fsm_termodecimal,
    fsm_termocomp,
    fsm_termoprimario
};

/*
 *      Global state variable
 */
analysis_state_t state;

/*
 *      Stack node has the sub-machine and state
 */
typedef struct stack_node {
    sub_machine_t sub_machine;
    int state;
    struct stack_node* next;
} stack_node;

/*
 *      Stack has just a top node and size
 */
typedef struct stack {
    stack_node* top;
    int size;
} stack_t;

void semantic_tbd(){
    printf("TODO\n");
}

/*
 *      push into the stack
 */
void push(stack_t* s, sub_machine_t sub_machine, int ret_state) {
    stack_node* sn = malloc(sizeof(stack_node*));
    sn->next = s->top;
    sn->sub_machine = sub_machine;
    sn->state = ret_state;
    s->top = sn;
    s->size++;

    state.get_token_flag = 0;
}

/*
 *      Pop from stack
 *              This function will update the 'state' global variable and free the memory
 *              allocated for the stack_node
 */
void pop(stack_t* s) {
    if(s->size > 0) {
        //update state variable
        state.current_sub_machine = s->top->sub_machine;
        state.current_sub_machine_state = s->top->state;

        //decrement stack size
        s->size--;

        // free stack top and point it to the next item
        stack_node* aux = s->top;
        s->top = s->top->next;
        free(aux);
        state.get_token_flag = 0;
    } else {
        state.current_sub_machine_state = -1;
    }
}

int is_empty(stack_t* s) {
    return s->size == 0;
}

/*
 *      Stack global variable
 */
stack_t stack;

/*
 *      Verify if next token should be issued or not.
 */
int should_get_next_token() {
    if(state.get_token_flag) {
        return 1;
    }
    return 0;
}

/*
 *      Go to next sub_machine
 */
int call_sm(sub_machine_t sm, int ret_st) {
    // push current sub_machine and return state into the stack
    push(&stack, state.current_sub_machine, ret_st);
    // update current sub_machine
    state.current_sub_machine = sm;
    return 0; // state 0 is the next state for the other fsm
}

/*
 *      Entry point for compilation
 */
int analyze(FILE* fp) {
    int ret;
    state.get_token_flag = 1;
    state.current_sub_machine_state = 0;
    state.current_sub_machine = 0;
    state.curr_token = NULL;
    state.last_token = NULL;

    while(1) {
        if(should_get_next_token()) {
            freeToken(&state.last_token);
            state.last_token = state.curr_token;
            if((ret = nextToken(fp, &state.curr_token)) == 0) {
                break;
            }
        }

        state.get_token_flag = 1;

        state.current_sub_machine_state = sub_machines[state.current_sub_machine](state.curr_token);
        if(state.current_sub_machine_state == -1) {
            printf("Compilation error!!!!\n");
            freeToken(&state.last_token);
            freeToken(&state.curr_token);
            return 1;
        }
    }

    printf("Compilation Successful\n");
    freeToken(&state.last_token);
    freeToken(&state.curr_token);
    return 0;
}

/*
 *      Aux functions
 */
int is_type(Token* t) {
    if(t->tokenClass == CLASS_RESERVED_WORD && (strcmp(t->value.stringValue, "int") == 0
        || strcmp(t->value.stringValue, "float") == 0
        || strcmp(t->value.stringValue, "string") == 0 )){
        return 1;
        }
        return 0;
}

/*
 *      Internal state functions
 */

int fsm_programa(Token* t){
    switch (state.current_sub_machine_state) {
        case 0:
            if(t->tokenClass == CLASS_RESERVED_WORD && strcmp(t->value.stringValue, "program") == 0) {
                semantic_tbd();
                return 1;
            }
            break;
        case 1:
            semantic_tbd();
            return call_sm(FSM_ESCOPO, 2);
        case 2:
            // Estado final
            pop(&stack);
            return state.current_sub_machine_state;
    }
    return -1;
}
int fsm_definicao(Token* t){
    switch (state.current_sub_machine_state) {
        case 0:
            break;
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;
        case 5:
            break;
        case 6:
            break;
        case 7:
            break;
    }
    return -1;
}
int fsm_funcao(Token* t){
    switch (state.current_sub_machine_state) {
        case 0:
            break;
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;
        case 5:
            break;
        case 6:
            break;
        case 7:
            break;
        case 8:
            break;
        case 9:
            break;
    }
    return -1;
}
int fsm_escopo(Token* t){
    switch (state.current_sub_machine_state) {
        case 0:
            break;
        case 1:
            break;
        case 2:
            break;
    }
    return -1;
}
int fsm_variavel(Token* t){
    switch (state.current_sub_machine_state) {
        case 0:
            break;
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;
        case 5:
            break;
        case 6:
            break;
        case 7:
            break;
        case 8:
            break;
        case 9:
            break;
        case 10:
            break;
        case 11:
            break;
        case 12:
            break;
        case 13:
            break;
        case 14:
            break;
    }
    return -1;
}
int fsm_condicional(Token* t){
    switch (state.current_sub_machine_state) {
        case 0:
            break;
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;
        case 5:
            break;
        case 6:
            break;
        case 7:
            break;
        case 8:
            break;
        case 9:
            break;
    }
    return -1;
}
int fsm_repeticao(Token* t){
    switch (state.current_sub_machine_state) {
        case 0:
            break;
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;
        case 5:
            break;
    }
    return -1;
}
int fsm_comando(Token* t){
    switch (state.current_sub_machine_state) {
        case 0:
            break;
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;
        case 5:
            break;
        case 6:
            break;
        case 7:
            break;
        case 8:
            break;
        case 9:
            break;
        case 10:
            break;
    }
    return -1;
}
int fsm_expressao(Token* t){
    switch (state.current_sub_machine_state) {
        case 0:
            break;
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
    }
    return -1;
}
int fsm_termodecimal(Token* t){
    switch (state.current_sub_machine_state) {
        case 0:
            break;
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
    }
    return -1;
}
int fsm_termocomp(Token* t){
    switch (state.current_sub_machine_state) {
        case 0:
            break;
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;
        case 5:
            break;
        case 6:
            break;
        case 7:
            break;
        case 8:
            break;
    }
    return -1;
}
int fsm_termoprimario(Token* t){
    switch (state.current_sub_machine_state) {
        case 0:
            break;
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;
        case 5:
            break;
        case 6:
            break;
        case 7:
            break;
    }
    return -1;
}
