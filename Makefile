all: cript_p cript

cript_p: cript_p.o criptfunc.o
	gcc cript_p.o criptfunc.o -o cript_p

cript: cript.o criptfunc.o
	gcc cript.o criptfunc.o -o cript

cript_p.o:  cript_p.c criptfunc.h
	gcc -c cript_p.c

cript.o:  cript.c criptfunc.h
	gcc -c cript.c

criptfunc.o: criptfunc.c criptfunc.h
	gcc -c criptfunc.c 