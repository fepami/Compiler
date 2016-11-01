
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
    fsm_program,
    fsm_var_declaration,
    fsm_instruction,
    fsm_loop,
    fsm_cond,
    fsm_return,
    fsm_expr,
    fsm_term_and,
    fsm_term_equal,
    fsm_term_relacional,
    fsm_term_add,
    fsm_term_mult,
    fsm_term_primary
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
int fsm_program(Token* t){
    switch(state.current_sub_machine_state) {
        case 0:
            if(is_type(t)) {
                return 1;
            }
            break;

        case 1:
            if(t->tokenClass == CLASS_IDENTIFIER) {
                return 2;
            }
            break;

        case 2:
            if(t->tokenClass == CLASS_DELIMITER) {
                if(t->value.charValue == '(') {
                    semantic_tbd();
                    return 3;
                } else if(t->value.charValue == '[') {
                    semantic_tbd();
                    return 4;
                } else if(t->value.charValue == ';') {
                    semantic_tbd();
                    return 5;
                }
            }
            break;

        case 3:
            if(t->tokenClass == CLASS_DELIMITER && t->value.charValue == ')'){
                semantic_tbd();
                return 7;
            } else {
                semantic_tbd();
                return call_sm(FSM_VAR_DECLARATION, 6);
            }
            break;

        case 4:
            semantic_tbd();
            return call_sm(FSM_EXPR, 9);

        case 5:
            if(is_type(t)) {
                semantic_tbd();
                return 1;
            }

            //final state
            pop(&stack);
            return state.current_sub_machine_state;

        case 6:
            if(t->tokenClass == CLASS_DELIMITER) {
                if(t->value.charValue == ',') {
                    semantic_tbd();
                    return 11;
                } else if(t->value.charValue == ')') {
                    semantic_tbd();
                    return 7;
                }
            }
            break;

        case 7:
            if(t->tokenClass == CLASS_DELIMITER && t->value.charValue == '{') {
                return 8;
            }
            break;

        case 8:
            if(t->tokenClass== CLASS_DELIMITER && t->value.charValue == '}') {
                return 5;
            } else {
                semantic_tbd();
                return call_sm(FSM_INSTRUCTION, 8);
            }
            break;

        case 9:
            if(t->tokenClass == CLASS_DELIMITER && t->value.charValue == ']') {
                semantic_tbd();
                return 10;
            }
            break;

        case 10:
            if(t->tokenClass == CLASS_DELIMITER && t->value.charValue == ';') {
                semantic_tbd();
                return 5;
            }
            break;

        case 11:
            semantic_tbd();
            return call_sm(FSM_VAR_DECLARATION, 6);
    }

    return -1;
}

int fsm_var_declaration(Token* t){
    switch(state.current_sub_machine_state) {
        case 0:
            if(is_type(t)) {
                semantic_tbd();
                return 1;
            }
            break;

        case 1:
            if(t->tokenClass== CLASS_IDENTIFIER) {
                semantic_tbd();
                return 2;
            }
            break;

        case 2:
            if(t->tokenClass == CLASS_DELIMITER && t->value.charValue == '[') {
                semantic_tbd();
                return 3;
            }
            //final state
            pop(&stack);
            return state.current_sub_machine_state;

        case 3:
            semantic_tbd();
            return call_sm(FSM_EXPR, 4);

        case 4:
            if(t->tokenClass == CLASS_DELIMITER && t->value.charValue == '[') {
                semantic_tbd();
                return 5;
            }
            break;

        case 5:
            //final state
            pop(&stack);
            return state.current_sub_machine_state;
    }
    return -1;
}

int fsm_instruction(Token* t) {
    switch(state.current_sub_machine_state) {
        case 0:
            if(t->tokenClass == CLASS_RESERVED_WORD) {
                if(strcmp(t->value.stringValue, "return") == 0) {
                    semantic_tbd();
                    return 2;
                } else if(strcmp(t->value.stringValue, "break") == 0) {
                    semantic_tbd();
                    return 1;
                } else if(strcmp(t->value.stringValue, "continue") == 0) {
                    semantic_tbd();
                    return 1;
                } else if(strcmp(t->value.stringValue, "while") == 0) {
                    semantic_tbd();
                    return 3;
                } else if(strcmp(t->value.stringValue, "if") == 0) {
                    semantic_tbd();
                    return 4;
                } else if(strcmp(t->value.stringValue, "read") == 0) {
                    semantic_tbd();
                    return 6;
                } else if(strcmp(t->value.stringValue, "write") == 0) {
                    semantic_tbd();
                    return 7;
                } else {
                    semantic_tbd();
                    return call_sm(FSM_VAR_DECLARATION, 1);
                }
            } else if(t->tokenClass == CLASS_IDENTIFIER) {
                semantic_tbd();
                return 5;
            }
            break;

        case 1:
            if(t->tokenClass == CLASS_DELIMITER && t->value.charValue == ';') {
                semantic_tbd();
                return 8;
            }
            break;

        case 2:
            semantic_tbd();
            return call_sm(FSM_RETURN, 8);

        case 3:
            semantic_tbd();
            return call_sm(FSM_LOOP, 8);

        case 4:
            semantic_tbd();
            return call_sm(FSM_COND, 8);

        case 5: // assing
            if(t->tokenClass == CLASS_OPERATOR && strcmp(t->value.stringValue, "=") == 0) {
                return 14;
            } else if(t->tokenClass == CLASS_DELIMITER && t->value.charValue == '(') {
                return 9;
            }
            break;

        case 6: // chamada de subrotina READ
            if(t->tokenClass == CLASS_DELIMITER && t->value.charValue == '(') {
                return 12;
            }
            break;

        case 7: // chamada de subrotina WRITE
            if(t->tokenClass == CLASS_DELIMITER && t->value.charValue == '(') {
                semantic_tbd();
                return 9;
            }
            break;


        case 8:
            //final state
            pop(&stack);
            return state.current_sub_machine_state;

        case 9:
            if(t->tokenClass == CLASS_DELIMITER && t->value.charValue == ')') {
                semantic_tbd();
                return 1;
            }
            semantic_tbd();
            return call_sm(FSM_EXPR, 10);

        case 10:
            if(t->tokenClass == CLASS_DELIMITER && t->value.charValue == ',') {
                semantic_tbd();
                return 11;
            } else if(t->tokenClass == CLASS_DELIMITER && t->value.charValue == ')') {
                semantic_tbd();
                return 1;
            }
            break;

        case 11:
            semantic_tbd();
            return call_sm(FSM_EXPR, 10);

        case 12:
            if(t->tokenClass == CLASS_IDENTIFIER) {
                semantic_tbd();
                return 13;
            }
            break;

        case 13:
            if(t->tokenClass == CLASS_DELIMITER && t->value.charValue == ')') {
                semantic_tbd();
                return 1;
            }
            break;

        case 14:
            semantic_tbd();
            return call_sm(FSM_EXPR, 1);

    }

    return -1;
}

int fsm_loop(Token* t) {
    switch(state.current_sub_machine_state){
        case 0:
            if(t->tokenClass== CLASS_DELIMITER && t->value.charValue == '(') {
                semantic_tbd();
                return 1;
            }
            break;

        case 1:
            semantic_tbd();
            return call_sm(FSM_EXPR, 2);

        case 2:
            if(t->tokenClass == CLASS_DELIMITER && t->value.charValue == ')') {
                semantic_tbd();
                return 3;
            }
            break;

        case 3:
            if(t->tokenClass == CLASS_DELIMITER && t->value.charValue == '{') {
                return 4;
            }
            break;

        case 4:
            if(t->tokenClass == CLASS_DELIMITER && t->value.charValue == '}') {
                return 5;
            } else {
                semantic_tbd();
                return call_sm(FSM_INSTRUCTION, 4);
            }
            break;

        case 5:
            //final state
            pop(&stack);
            return state.current_sub_machine_state;
    }
    return -1;
}

int fsm_cond(Token* t) {
    switch(state.current_sub_machine_state) {
        case 0:
            if(t->tokenClass== CLASS_DELIMITER && t->value.charValue == '(') {
                semantic_tbd();
                return 1;
            }
            break;

        case 1:
            semantic_tbd();
            return call_sm(FSM_EXPR, 2);

        case 2:
            if(t->tokenClass == CLASS_DELIMITER && t->value.charValue == ')') {
                semantic_tbd();
                return 3;
            }
            break;

        case 3:
            if(t->tokenClass == CLASS_DELIMITER && t->value.charValue == '{') {
                return 4;
            }
            break;

        case 4:
            if(t->tokenClass == CLASS_RESERVED_WORD && strcmp(t->value.stringValue, "else") == 0) {
                semantic_tbd();
                return 5;
            } else if (t->tokenClass == CLASS_DELIMITER && t->value.charValue == '}') {
                return 6;
            } else {
                semantic_tbd();
                return call_sm(FSM_INSTRUCTION, 4);
            }
            break;

        case 5:
            if(t->tokenClass == CLASS_DELIMITER && t->value.charValue == '}') {
                return 6;
            } else {
                semantic_tbd();
                return call_sm(FSM_INSTRUCTION, 5);
            }
            break;

        case 6:
            //final state
            pop(&stack);
            return state.current_sub_machine_state;
    }
    return -1;
}

int fsm_return(Token* t) {
    switch(state.current_sub_machine_state) {
        case 0:
            semantic_tbd();
            return call_sm(FSM_EXPR, 1);

        case 1:
            if(t->tokenClass == CLASS_DELIMITER && call_sm(FSM_EXPR, 1) == ';') {
                semantic_tbd();
                return 2;
            }
            break;

        case 2:
            //final state
            pop(&stack);
            return state.current_sub_machine_state;
    }
    return -1;
}

int fsm_expr(Token* t) {
    switch(state.current_sub_machine_state) {
        case 0:
            semantic_tbd();
            return call_sm(FSM_TERM_AND, 1);

        case 1:
            if((t->tokenClass == CLASS_OPERATOR && strcmp(t->value.stringValue, "|") == 0) ||
                (t->tokenClass == CLASS_OPERATOR && strcmp(t->value.stringValue, "||") == 0)) {
                semantic_tbd();
                return 2;
            }
            //final state
            pop(&stack);
            return state.current_sub_machine_state;

        case 2:
            semantic_tbd();
            return call_sm(FSM_EXPR, 3);

        case 3:
            //final state
            pop(&stack);
            return state.current_sub_machine_state;
    }
    return -1;
}

int fsm_term_and(Token* t) {
    switch(state.current_sub_machine_state) {
        case 0:
            semantic_tbd();
            return call_sm(FSM_TERM_EQUAL, 1);

        case 1:
            if((t->tokenClass == CLASS_OPERATOR && strcmp(t->value.stringValue, "&") == 0) ||
                (t->tokenClass == CLASS_OPERATOR && strcmp(t->value.stringValue, "&&") == 0)) {
                semantic_tbd();
            return 2;
                }
                //final state
                pop(&stack);
                return state.current_sub_machine_state;

        case 2:
            semantic_tbd();
            return call_sm(FSM_TERM_ADD, 3);

        case 3:
            //final state
            pop(&stack);
            return state.current_sub_machine_state;
    }
    return -1;
}

int fsm_term_equal(Token* t) {
    switch(state.current_sub_machine_state) {
        case 0:
            semantic_tbd();
            return call_sm(FSM_TERM_RELACIONAL, 1);

        case 1:
            if(t->tokenClass == CLASS_OPERATOR && (strcmp(t->value.stringValue, "==") == 0 ||
                strcmp(t->value.stringValue, "!=") == 0)) {
                semantic_tbd();
            return 2;
                }
                //final state
                pop(&stack);
                return state.current_sub_machine_state;

        case 2:
            semantic_tbd();
            return call_sm(FSM_TERM_EQUAL, 3);

        case 3:
            //final state
            pop(&stack);
            return state.current_sub_machine_state;

    }
    return -1;
}

int fsm_term_relacional(Token* t) {
    switch(state.current_sub_machine_state) {
        case 0:
            semantic_tbd();
            return call_sm(FSM_TERM_ADD, 1);

        case 1:
            if(t->tokenClass == CLASS_OPERATOR && (strcmp(t->value.stringValue, ">=") == 0 ||
                strcmp(t->value.stringValue, "<=") == 0)) {
                semantic_tbd();
                return 2;
                } else if(t->tokenClass== CLASS_OPERATOR && (strcmp(t->value.stringValue,">") == 0 ||
                    strcmp(t->value.stringValue, "<") == 0)) {
                return 2;
            }
            //final state
            pop(&stack);
            return state.current_sub_machine_state;

        case 2:
            semantic_tbd();
            return call_sm(FSM_TERM_RELACIONAL, 3);

        case 3:
            //final state
            pop(&stack);
            return state.current_sub_machine_state;

    }
    return -1;
}

int fsm_term_add(Token* t) {
    switch(state.current_sub_machine_state) {
        case 0:
            semantic_tbd();
            return call_sm(FSM_TERM_MULT, 1);

        case 1:
            if(t->tokenClass == CLASS_OPERATOR && (strcmp(t->value.stringValue, "+") == 0 ||
                strcmp(t->value.stringValue, "-") == 0)) {
                return 2;
            }
            //final state
            pop(&stack);
            return state.current_sub_machine_state;

        case 2:
            semantic_tbd();
            return call_sm(FSM_TERM_AND, 3);

        case 3:
            //final state
            pop(&stack);
            return state.current_sub_machine_state;
    }
    return -1;
}

int fsm_term_mult(Token* t) {
    switch(state.current_sub_machine_state) {
        case 0:
            semantic_tbd();
            return call_sm(FSM_TERM_PRIMARY, 1);

        case 1:
            if(t->tokenClass == CLASS_OPERATOR && (strcmp(t->value.stringValue, "*") == 0 ||
                strcmp(t->value.stringValue, "/") == 0 || strcmp(t->value.stringValue, "%") == 0)) {
                return 2;
            }
            //final state
            pop(&stack);
            return state.current_sub_machine_state;

        case 2:
            semantic_tbd();
            return call_sm(FSM_TERM_MULT, 3);

        case 3:
            //final state
            pop(&stack);
            return state.current_sub_machine_state;
    }
    return -1;
}

int fsm_term_primary(Token* t) {
    switch(state.current_sub_machine_state) {
        case 0:
            if(t->tokenClass == CLASS_IDENTIFIER){
                semantic_tbd();
                return 1;
            } else if(t->tokenClass == CLASS_INT || t->tokenClass== CLASS_FLOAT || t->tokenClass == CLASS_STRING_LIT) {
                semantic_tbd();
                return 2;
            } else if(t->tokenClass == CLASS_DELIMITER && t->value.charValue == '(') {
                semantic_tbd();
                return 3;
            }
            break;

        case 1:
            if(t->tokenClass == CLASS_DELIMITER && t->value.charValue == '(') {
                semantic_tbd();
                return 4;
            }

            //final state
            pop(&stack);
            return state.current_sub_machine_state;

        case 2:
            //final state
            pop(&stack);
            return state.current_sub_machine_state;

        case 3:
            semantic_tbd();
            return call_sm(FSM_EXPR, 5);

        case 4:
            if(t->tokenClass == CLASS_DELIMITER && t->value.charValue == ')') {
                semantic_tbd();
                return 2;
            } else {
                semantic_tbd();
                return call_sm(FSM_EXPR, 6);
            }
            break;

        case 5:
            if(t->tokenClass == CLASS_DELIMITER && t->value.charValue == ')') {
                semantic_tbd();
                return 2;
            }
            break;

        case 6:
            if(t->tokenClass == CLASS_DELIMITER && t->value.charValue == ',') {
                semantic_tbd();
                return 7;
            } else if(t->tokenClass == CLASS_DELIMITER && t->value.charValue == ')') {
                semantic_tbd();
                return 2;
            }
            break;

        case 7:
            semantic_tbd();
            return call_sm(FSM_EXPR, 6);
    }
    return -1;
}
