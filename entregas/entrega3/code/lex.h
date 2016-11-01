#ifndef LEX_H
#define LEX_H

#include <stdio.h>

#include "token.h"

typedef enum {
    ST_INIT,
    ST_COMMENT,
    ST_NUM_INT,
    ST_NUM_FLOAT,
    ST_STR_LIT,
    ST_APLHANUM,
    ST_OPERATOR,
    ST_DELIMITER,
    ST_TOKEN_END,
    ST_LEX_ERROR,
    STATES_SIZE
} LexState;

typedef enum {
    IN_DIGIT,
    IN_APLHA,
    IN_OPERATOR, 
    IN_STRICT_DELIMITER,
    IN_SPACE_DELIMITER,
    IN_NEW_LINE_DELIMITER,
    IN_COMMENT_BEGIN,
    IN_DOT,
    IN_STRING_QUOTE,
    IN_CLASS_SIZE
} InputClass;

typedef struct LexStateData {
    char currentInput;
    char buffer[256];
    unsigned bufferIndex;
    FILE* filePtr;
    LexState lastState;
    LexState currentState;
    InputClass inputClass;
} LexStateData;

int nextToken(FILE* file_ptr, Token** token);

#endif // LEX_H
