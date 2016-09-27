#include <string.h>

#include "util.h"

const char CHAR_OPERATORS[] = {
    '=', '>', '<', '!', '+', '-', '*', '/', '^', '&', '|' 
};
const char DELIMITERS[] = {
    '{', '}', '[', ']', '(', ')', ',', ';', ' ', '\n', '\t'
};
const char COMENTARY = '#';
const char DOT = '.';
const char STRING_QUOTE = '"';
const char* RESERVERD_WORDS[] = {
    "if", "else", "while", "int", "float", "string",
    "return", "const", "break", "continue", "read", "write"
};
const char* OPERATORS[] = {
    "=", ">", "<", "!", "+", "-", "*", "/", "^", "&", "|",
    "==", ">=", "<=", "!=", "&&", "||"
};

int isInCharArray(const char array[], const unsigned arraySize, const char c) {
    unsigned i;
    for(i = 0; i < arraySize; i++) {
        if (c == array[i])
            return 1;
    }
    return 0;
}

int isInCharPtrArray(const char* array[], const unsigned arraySize, const char* c) {
    unsigned i;
    for(i = 0; i < arraySize; i++) {
        if (strcmp(c, array[i]) == 0)
            return 1;
    }
    return 0;
}

int isDigit(char c) {
    return c >= '0' && c <= '9';
}

int isAlpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

int isCharOperator(char c) {
    return isInCharArray(CHAR_OPERATORS, sizeof(CHAR_OPERATORS)/sizeof(char), c);
}

int isDelimiter(char c) {
    return isInCharArray(DELIMITERS, sizeof(DELIMITERS)/sizeof(char), c);
}

int isCommentBegin(char c) {
    return c == COMENTARY;
}

int isDot(char c) {
    return c == DOT;
}

int isStringQuote(char c) {
    return c == STRING_QUOTE;
}

int isReserverdWord(const char* c) {
    return isInCharPtrArray(RESERVERD_WORDS, sizeof(RESERVERD_WORDS)/sizeof(const char *), c);
}

int isStrOperator(const char* c) {
    return isInCharPtrArray(OPERATORS, sizeof(OPERATORS)/sizeof(const char *), c);
}

