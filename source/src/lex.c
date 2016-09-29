#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"
#include "token.h"
#include "lex.h"

/*
 *              DIGIT           APLHA           OPERATOR        S_DELIMITER     SP_DELIMITER 	NL_DELIMITER	COMM_INIT       DOT             STRING_QUOTE
 *  INIT
 *  COMMENT
 *  INT
 *  FLOAT
 *  STRING_LIT
 *  ALPHAN
 *  OP
 *  DELIM
 *  LEX_ERROR
 *              DIGIT           APLHA           OPERATOR        S_DELIMITER	SP_DELIMITER 	NL_DELIMITER	COMM_INIT       DOT             STRING_QUOTE
 */
const LexState nextState[STATES_SIZE][IN_CLASS_SIZE] = {
        {       ST_NUM_INT,     ST_APLHANUM,    ST_OPERATOR,    ST_DELIMITER,	ST_INIT,	ST_INIT,   	ST_COMMENT,     ST_LEX_ERROR,   ST_STR_LIT      },
        {       ST_COMMENT,     ST_COMMENT,     ST_COMMENT,     ST_COMMENT,  	ST_COMMENT,  	ST_INIT,     	ST_COMMENT,     ST_COMMENT,     ST_COMMENT      },
        {       ST_NUM_INT,     ST_TOKEN_END,   ST_TOKEN_END,   ST_TOKEN_END,	ST_TOKEN_END,	ST_TOKEN_END,   ST_TOKEN_END,   ST_NUM_FLOAT,   ST_TOKEN_END    },
        {       ST_NUM_FLOAT,   ST_TOKEN_END,   ST_TOKEN_END,   ST_TOKEN_END,	ST_TOKEN_END,	ST_TOKEN_END,   ST_TOKEN_END,   ST_LEX_ERROR,   ST_LEX_ERROR    },
        {       ST_STR_LIT,     ST_STR_LIT,     ST_STR_LIT,     ST_STR_LIT,  	ST_STR_LIT,  	ST_STR_LIT,     ST_STR_LIT,     ST_STR_LIT,     ST_TOKEN_END    },
        {       ST_APLHANUM,    ST_APLHANUM,    ST_TOKEN_END,   ST_TOKEN_END,	ST_TOKEN_END,	ST_TOKEN_END,   ST_TOKEN_END,   ST_LEX_ERROR,   ST_LEX_ERROR    },
        {       ST_TOKEN_END,   ST_TOKEN_END,   ST_OPERATOR,    ST_TOKEN_END,	ST_TOKEN_END,	ST_TOKEN_END,   ST_TOKEN_END,   ST_TOKEN_END,   ST_TOKEN_END    },
        {       ST_TOKEN_END,   ST_TOKEN_END,   ST_TOKEN_END,   ST_TOKEN_END,	ST_TOKEN_END,	ST_TOKEN_END,   ST_TOKEN_END,   ST_TOKEN_END,   ST_TOKEN_END    },
        {       ST_LEX_ERROR,   ST_LEX_ERROR,   ST_LEX_ERROR,   ST_LEX_ERROR,	ST_LEX_ERROR,	ST_LEX_ERROR,   ST_LEX_ERROR,   ST_LEX_ERROR,   ST_LEX_ERROR    }
};

void changeCurrentState(LexStateData* lsd, LexState nState){
    lsd->lastState = lsd->currentState;
    lsd->currentState = nState;
}

int addToBuffer(LexStateData* lsd){
    lsd->buffer[lsd->bufferIndex] = lsd->currentInput;
    lsd->bufferIndex++;
    lsd->buffer[lsd->bufferIndex] = '\0';
    return 1;
}

int endToken(LexStateData* lsd) {
    if(lsd->lastState != ST_STR_LIT) // Última  '"' deve ser ignorada 
	ungetc(lsd->currentInput, lsd->filePtr);
    return 1;
}

int ignoreInput(){
    return 1;
}

int error() {
    printf("[ERROR] Could not create Token - Lexical error\n");
    return 0;
}

int doStateChange(LexStateData* lsd) {
    changeCurrentState(lsd, nextState[lsd->currentState][lsd->inputClass]);
    
    if(lsd->currentState == ST_INIT || lsd->currentState == ST_COMMENT ||
	    (lsd->currentState == ST_STR_LIT && lsd->lastState != ST_STR_LIT)){ //Primeira '"' deve ser ignorada
	return ignoreInput();
    }else if(lsd->currentState == ST_TOKEN_END){
	return endToken(lsd);
    }else if(lsd->currentState == ST_LEX_ERROR){
	return error();
    }else{
	return addToBuffer(lsd);
    }
}

InputClass classifyInputClass(char c) {
    if(isDigit(c))
        return IN_DIGIT;
    else if(isAlpha(c))
        return IN_APLHA;
    else if(isCharOperator(c))
        return IN_OPERATOR;
    else if(isStrictDelimiter(c))
        return IN_STRICT_DELIMITER;
    else if(isSpaceDelimiter(c))
        return IN_SPACE_DELIMITER;
    else if(isNewLineDelimiter(c))
        return IN_NEW_LINE_DELIMITER;   
    else if(isComment(c))
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
        if(isReservedWord(lsd->buffer)){
            return newToken(CLASS_RESERVED_WORD, lsd->buffer);
        }else{
            // TODO: Symbols Table
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
        if(!isStrOperator(lsd->buffer))
            return NULL;
        return newToken(CLASS_OPERATOR, lsd->buffer);
    default:
        return NULL;
    }
}

int nextToken(FILE* file_ptr, Token** token){
    LexStateData lexStateData;
    memset(&lexStateData, 0, sizeof(LexStateData));
    lexStateData.filePtr = file_ptr;
    
    do {
        lexStateData.currentInput = getc(file_ptr);
        if(lexStateData.currentInput == EOF)
            break;
	
	lexStateData.inputClass = classifyInputClass(lexStateData.currentInput);
	if(!doStateChange(&lexStateData)){
	    *token = NULL;
	    return -1;
	}
    } while(lexStateData.currentState != ST_TOKEN_END);

    if(lexStateData.bufferIndex == 0){
        *token = NULL;
        return 0;
    }
    
    *token = buildToken(&lexStateData);
    if(*token == NULL){
        printf("[ERROR] Could not create Token - Unknown error\n");
        return -1;
    }
    
    return 1;
}
