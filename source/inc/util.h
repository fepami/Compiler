#ifndef UTIL_H
#define UTIL_H

int isDigit(char c);
int isAlpha(char c);
int isOperator(char c);
int isDelimiter(char c);
int isCommentBegin(char c);
int isDot(char c);
int isStringQuote(char c);

int isReserverdWord(const char* c);
int isComparisonOperator(const char* c);


#endif // UTIL_H
