#ifndef TOKEN_H
#define TOKEN_H

typedef enum {
    CLASS_INT, // int number
    CLASS_FLOAT, // float number
    CLASS_STRING_LIT, // literal string
    CLASS_RESERVED_WORD, // if, while, int, ...
    CLASS_IDENTIFIER, // variable name
    CLASS_OPERATOR, // '=', '>', '<', '!', '+', '-', '*', '/', "==", ">=", "<=", "!="
    CLASS_DELIMITER, //  '{', '}', '[', ']', ',', ';', ' ', '\t'
} TokenClass;

typedef struct Token {
    union value {
        int intValue;
        float floatValue;
        char charValue;
        char* stringValue;
    } value;
    TokenClass class;
} Token;

void printToken(const Token* t);
Token* newToken(TokenClass class, void* data);
void freeToken(Token** t);

#endif // TOKEN_H

