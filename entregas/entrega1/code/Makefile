C=gcc
CFLAGS=-I.
DEPS = lex.h token.h util.h
OBJ = lex.o token.o util.o main.o

%.o: %.c $(DEPS)
		$(CC) -c -o $@ $< $(CFLAGS)

lex: $(OBJ)
		gcc -o $@ $^ $(CFLAGS)

test: lex
		./lex ../ENTRADA.txt

.PHONY: clean
clean:
		rm -f *.o lex 
