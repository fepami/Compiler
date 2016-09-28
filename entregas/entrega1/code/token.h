#ifndef TOKEN_H
#define TOKEN_H

typedef enum {
    CLASS_INT,
    CLASS_FLOAT,
    CLASS_STRING_LIT,
    CLASS_IDENTIFIER,
    CLASS_RESERVED_WORD,
    CLASS_OPERATOR,
    CLASS_DELIMITER, 
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

