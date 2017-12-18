#include "vet.h"

int* vet_aleatorio(int n){ // gera um vetor aleatório entre 0 e 1000 (RANGE)

	int *v = malloc(sizeof(int)*n);
	for(int i = 0; i < n; i++){
		v[i] = rand() % RANGE;
	}

	return v;
}

void print_vet(int *vet,int n){ // printa o vetor
	for (int i = 0; i < n; ++i){
		printf("%d-", vet[i]);
	}
	printf("\n");
}

void reset_vet(int *vet,int n){ // torna o vetor aleatório novamente;
	for(int i = 0; i < n; i++){
		vet[i] = rand() % RANGE;
	}
}