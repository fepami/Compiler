#include <string.h>

#include "util.h"

const char CHAR_OPERATORS[] = {
    '=', '>', '<', '!', '+', '-', '*', '/', '^', '&', '|'
};
const char STRICT_DELIMITERS[] = {
    '{', '}', '[', ']', '(', ')', ',', ';', ':'
};
const char SPACE_DELIMITERS[] = {
    ' ', '\t'
};
const char NEW_LINE_DELIMITERS[] = {
    '\n', '\r'
};
const char COMENTARY = '#';
const char DOT = '.';
const char STRING_QUOTE = '"';
const char* OPERATORS[] = {
    "=", ">", "<", "!", "+", "-", "*", "/", "^", "&", "|",
    "==", ">=", "<=", "!=", "&&", "||"
};

const char* RESERVED_WORDS[] = {
    "int", "float", "string", "bool",
    "function", "program", "deftype",
    "if", "else", "while", "return", 
    "var", "persistent", "const",
    "true", "false" 
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

int isStrictDelimiter(char c) {
    return isInCharArray(STRICT_DELIMITERS, sizeof(STRICT_DELIMITERS)/sizeof(char), c);
}

int isSpaceDelimiter(char c) {
    return isInCharArray(SPACE_DELIMITERS, sizeof(SPACE_DELIMITERS)/sizeof(char), c);
}

int isNewLineDelimiter(char c) {
    return isInCharArray(NEW_LINE_DELIMITERS, sizeof(NEW_LINE_DELIMITERS)/sizeof(char), c);
}

int isDelimiter(char c) {
    return isStrictDelimiter(c) || isSpaceDelimiter(c) || isNewLineDelimiter(c);
}

int isComment(char c) {
    return c == COMENTARY;
}

int isDot(char c) {
    return c == DOT;
}

int isStringQuote(char c) {
    return c == STRING_QUOTE;
}

int isStrOperator(const char* c) {
    return isInCharPtrArray(OPERATORS, sizeof(OPERATORS)/sizeof(const char *), c);
}

int isReservedWord(const char* c) {
    return isInCharPtrArray(RESERVED_WORDS, sizeof(RESERVED_WORDS)/sizeof(const char *), c);
}
