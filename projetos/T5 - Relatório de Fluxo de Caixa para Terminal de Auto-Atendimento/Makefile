# Compilador gcc
CP = gcc
# Parâmetros de compilação
P_CO = -c -lm -Wall -pedantic -g 

all: main.o caixa.o
	$(CP) main.o caixa.o -o main -lm

main: main.o caixa.o
	$(CP) main.o caixa.o -o main -lm

caixa.o: caixa.c 
	$(CP) $(P_CO) caixa.c	

main.o: main.c 
	$(CP) $(P_CO) main.c

run_teste: main 
	./main < entrada.txt 
	make clean

zip:
	zip fluxo_caixa.zip caixa.h caixa.c main.c Makefile nome

run: main
	./main

clean: 
	rm -rf *o main

valgrind: main
	valgrind --leak-check=full --track-origins=yes ./main < entrada.txt