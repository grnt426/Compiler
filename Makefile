CFLAGS=-std=c99 

all: hartz

hartz:
	gcc $(CFLAGS) translator.c symbols.c idents.c strlib.c generrors.c -o translator 

ccode:
	gcc $(CLFAGS) compiler.c symbols.c idents.c strlib.c generrors.c -o compiler

clean:
	rm -rf *.o hartz ccode
