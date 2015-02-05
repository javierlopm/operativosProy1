all: cript_p cript cript_t

cript_t: cript_t.o criptfunc.o
	gcc -g-Wall-Wextra cript_t.o criptfunc.o -o cript_t -lpthread

cript_p: cript_p.o criptfunc.o
	gcc -g-Wall-Wextra cript_p.o criptfunc.o -o cript_p

cript: cript.o criptfunc.o
	gcc -g-Wall-Wextra cript.o criptfunc.o -o cript



cript_t.o:  cript_t.c criptfunc.h
	gcc -g -c cript_t.c -lpthread

cript_p.o:  cript_p.c criptfunc.h
	gcc -g -c cript_p.c

cript.o:  cript.c criptfunc.h
	gcc -g -c cript.c

criptfunc.o: criptfunc.c criptfunc.h
	gcc -c criptfunc.c 

clean:
	rm *.o