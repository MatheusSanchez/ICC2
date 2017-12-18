# Compilador gcc
CP = gcc
# Parâmetros de compilação
P_CO = -c -lm -Wall -pedantic 

sort.o: sort.c vet.o
	$(CP) $(P_CO) sort.c

vet.o: vet.c 
	$(CP) $(P_CO) vet.c	

main.o: main.c vet.o sort.o
	$(CP) $(P_CO) main.c

main: sort.o main.o vet.o
	$(CP) main.o sort.o vet.o -o main -lm

run: main
	./main 

clean: 
	rm -rf *o main