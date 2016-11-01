C=gcc
CFLAGS=-I.
DEPS = lex.h token.h util.h syntatic.h
OBJ = lex.o token.o util.o syntatic.o main.o

%.o: %.c $(DEPS)
		$(CC) -c -o $@ $< $(CFLAGS)

syntatic: $(OBJ)
		gcc -o $@ $^ $(CFLAGS)

test: syntatic
		./syntatic ../ENTRADA.txt

.PHONY: clean
clean:
		rm -f *.o syntatic 
