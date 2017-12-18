# Compilador gcc
CP = gcc
# Parâmetros de compilação
P_CO = -c -lm -Wall -pedantic -g3

main: analisadores.o main.o
	$(CP) analisadores.o main.o -o main -lm

all: analisadores.o main.o 
	$(CP) analisadores.o main.o -o main -lm

analisadores.o: analisadores.c
	$(CP) $(P_CO) analisadores.c	

main.o: main.c 
	$(CP) $(P_CO) main.c

run: main
	./main

clean: 
	rm -rf *o main