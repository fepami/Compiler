#ifndef UTIL_H
#define UTIL_H

int isDigit(char c);
int isAlpha(char c);
int isCharOperator(char c);
int isStrictDelimiter(char c);
int isSpaceDelimiter(char c);
int isNewLineDelimiter(char c);
int isDelimiter(char c);
int isComment(char c);
int isDot(char c);
int isStringQuote(char c);

int isStrOperator(const char* c);
int isReservedWord(const char* c);


#endif // UTIL_H
