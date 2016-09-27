#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"
#include "token.h"
#include "lex.h"

LexStateData lexStateData;

/*
 *	Table that represents the Transductor states
 *
 *
 *			digit	alpha	operator	DELIMITER	COMM_INIT	DOT   STRING_QUOTE
 *	INIT
 *	COMMENT
 *	N_INT
 *	FLOAT
 *	STRING_LIT
 *	ALPHAN
 *	OP
 *	DELIM
 *	LEX_ERROR
 */
const LexState nextState[STATES_SIZE][IN_CLASS_SIZE] = {
    { ST_NUM_INT,   ST_APLHANUM,    ST_OPERATOR,    ST_DELIMITER,   ST_COMMENT,     ST_LEX_ERROR,   ST_STR_LIT      },
    { ST_COMMENT,   ST_COMMENT,     ST_COMMENT,     ST_COMMENT,     ST_COMMENT,     ST_COMMENT,     ST_COMMENT      },
    { ST_NUM_INT,   ST_TOKEN_END,   ST_TOKEN_END,   ST_TOKEN_END,   ST_TOKEN_END,   ST_NUM_FLOAT,   ST_TOKEN_END    },
    { ST_NUM_FLOAT, ST_TOKEN_END,   ST_TOKEN_END,   ST_TOKEN_END,   ST_TOKEN_END,   ST_LEX_ERROR,   ST_LEX_ERROR    },
    { ST_STR_LIT,   ST_STR_LIT,     ST_STR_LIT,     ST_STR_LIT,     ST_STR_LIT,     ST_STR_LIT,     ST_TOKEN_END    },
    { ST_APLHANUM,  ST_APLHANUM,    ST_TOKEN_END,   ST_TOKEN_END,   ST_TOKEN_END,   ST_LEX_ERROR,   ST_LEX_ERROR    },
    { ST_TOKEN_END, ST_TOKEN_END,   ST_OPERATOR,    ST_TOKEN_END,   ST_TOKEN_END,   ST_TOKEN_END,   ST_TOKEN_END    },
    { ST_TOKEN_END, ST_TOKEN_END,   ST_TOKEN_END,   ST_TOKEN_END,   ST_TOKEN_END,   ST_TOKEN_END,   ST_TOKEN_END    },
    { ST_LEX_ERROR, ST_LEX_ERROR,   ST_LEX_ERROR,   ST_LEX_ERROR,   ST_LEX_ERROR,   ST_LEX_ERROR,   ST_LEX_ERROR    }
};


void addToBuffer(LexStateData* lsd){
    lsd->buffer[lsd->bufferIndex] = lsd->currentInput;
    lsd->bufferIndex++;
    lsd->buffer[lsd->bufferIndex] = '\0';
}

void changeCurrentState(LexStateData* lsd, LexState nState){
    lsd->lastState = lsd->currentState;
    lsd->currentState = nState;
}

void appendInput(LexStateData* lsd) {
    addToBuffer(lsd);
    changeCurrentState(lsd, nextState[lsd->currentState][lsd->inputClass]);
}

void handleDelimiterInit(LexStateData* lsd) {
    if((lsd->currentInput == ' ' || lsd->currentInput == '\t' || lsd->currentInput == '\n')) {
        changeCurrentState(lsd, ST_INIT);
    } else{
        appendInput(lsd);
    }
}

void ignoreInput(LexStateData* lsd) {
    (void) lsd; /* unused */
}

void handleCommentInit(LexStateData* lsd) {
    changeCurrentState(lsd, ST_COMMENT);
}

void handleCommentDelimiter(LexStateData* lsd) {
    if(lsd->currentInput == '\n')
        changeCurrentState(lsd, ST_INIT);
}

void handleTokenEnd(LexStateData* lsd) {
    ungetc(lsd->currentInput, lsd->filePtr);
    changeCurrentState(lsd, ST_TOKEN_END);
}

void handleStrLit(LexStateData* lsd) {
    if(lsd->currentInput == '"' && lsd->currentState == ST_INIT) {
        changeCurrentState(lsd, ST_STR_LIT);
    } else if(lsd->currentInput == '"' && lsd->currentState == ST_STR_LIT){
        changeCurrentState(lsd, ST_TOKEN_END);
    } else if(lsd->currentInput != '"') {
        addToBuffer(lsd);
    }
}

void handleError(LexStateData* lsd) {
    (void) lsd;
    printf(">>ERROR: BUILD FAILED!\n");
    exit(1);
}

/*
 *			digit	alpha	operator	DELIMITER	COMM_INIT	DOT	STRING_QUOTE
 *	INIT
 *	COMMENT
 *	N_INT
 *	FLOAT
 *	STR_LIT
 *	ALPHAN
 *	OP
 *	DELIM
 *	LEX_ERROR
 */
void (* stateFunction[STATES_SIZE][IN_CLASS_SIZE]) (LexStateData* lsd) = {
{ appendInput,      appendInput,    appendInput,    handleDelimiterInit,    handleCommentInit,  handleError,    handleStrLit },
{ ignoreInput,      ignoreInput,    ignoreInput,    handleCommentDelimiter, ignoreInput,        ignoreInput,    ignoreInput },
{ appendInput,      handleTokenEnd, handleTokenEnd, handleTokenEnd,         handleTokenEnd,     appendInput,    handleError },
{ appendInput,      handleTokenEnd, handleTokenEnd, handleTokenEnd,         handleTokenEnd,     handleError,    handleError },
{ handleStrLit,     handleStrLit,   handleStrLit,   handleStrLit,           handleStrLit,       handleStrLit,   handleStrLit},
{ appendInput,      appendInput,    handleTokenEnd, handleTokenEnd,         handleTokenEnd,     handleError,    handleError },
{ handleTokenEnd,   handleTokenEnd, appendInput,    handleTokenEnd,         handleTokenEnd,     handleTokenEnd, handleTokenEnd},
{ handleTokenEnd,   handleTokenEnd, handleTokenEnd, handleTokenEnd,         handleTokenEnd,     handleTokenEnd, handleTokenEnd},
{ handleError,      handleError,    handleError,    handleError,            handleError,        handleError,    handleError}
};

InputClass classifyInputClass(char c) {
    if(isDigit(c))
        return IN_DIGIT;
    else if(isAlpha(c))
        return IN_APLHA;
    else if(isOperator(c))
        return IN_OPERATOR;
    else if(isDelimiter(c))
        return IN_DELIMITER;
    else if(isCommentBegin(c))
        return IN_COMMENT_BEGIN;
    else if(isDot(c))
        return IN_DOT;
    else if(isStringQuote(c))
        return IN_STRING_QUOTE;
    else
        return -1;
}

Token* buildToken(LexStateData* lsd) {
    switch(lsd->lastState) {
    case ST_APLHANUM:
        if(isReserverdWord(lsd->buffer)){
            return newToken(CLASS_RESERVED_WORD, lsd->buffer);
        }else{
            return newToken(CLASS_IDENTIFIER, lsd->buffer);
        }
    case ST_DELIMITER:
        return newToken(CLASS_DELIMITER, lsd->buffer);
    case ST_NUM_FLOAT:
        return newToken(CLASS_FLOAT, lsd->buffer);
    case ST_NUM_INT:
        return newToken(CLASS_INT, lsd->buffer);
    case ST_STR_LIT:
        return newToken(CLASS_STRING_LIT, lsd->buffer);
    case ST_OPERATOR:
        if(lsd->bufferIndex == 1){
            return newToken(CLASS_SINGLE_OPERATOR, lsd->buffer);
        } else {
            if(!isComparisonOperator(lsd->buffer))
                return NULL;
            return newToken(CLASS_DOUBLE_OPERATOR, lsd->buffer);
        }
    default:
        return NULL;
    }
}

int nextToken(FILE* file_ptr, Token** token){
    memset(&lexStateData, 0, sizeof(LexStateData));
    lexStateData.filePtr = file_ptr;
    
    do {
        lexStateData.currentInput = getc(file_ptr);
        if(lexStateData.currentInput == EOF)
            break;
		lexStateData.inputClass = classifyInputClass(lexStateData.currentInput);
		stateFunction[lexStateData.currentState][lexStateData.inputClass](&lexStateData);
	} while(lexStateData.currentState != ST_TOKEN_END);

	if(lexStateData.bufferIndex == 0){
        *token = NULL;
        return 0;
    }
    
    *token = buildToken(&lexStateData);
    if(*token == NULL){
        fprintf(stderr, "[ERROR] Could not create Token - Unknown error\n");
        return -1;
    }
    
    return 1;
}
