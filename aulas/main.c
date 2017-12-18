	#include <stdio.h>
#include "vet.h"
#include "sort.h"

int main (){
	int n;

	printf("Digite o Tamanho do vetor: ");
	scanf("%d",&n);

	int *vet = vet_aleatorio(n);

	printf("Vetor Gerado Aleatorio\n");
	print_vet(vet,n);		
	printf("\n");
	
	printf("Vetor Ordenado Bubble\n");
	bubble(vet,n);
	print_vet(vet,n);
	printf("\n");

	printf("Novo vetor\n");
	reset_vet(vet,n);
	print_vet(vet,n);
	printf("Vetor Ordenado selection\n");
	selection(vet,n);
	print_vet(vet,n);
	printf("\n");

	printf("Novo vetor\n");
	reset_vet(vet,n);
	print_vet(vet,n);
	printf("Vetor Ordenado Insertion\n");
	insertion(vet,n);
	print_vet(vet,n);
	printf("oiii\n");
	printf("\n");

	printf("Novo vetor\n");
	reset_vet(vet,n);
	print_vet(vet,n);
	printf("Vetor Ordenado Merge\n");
	mergesort(vet,0,n-1,n);
	print_vet(vet,n);
	printf("\n");

	printf("Novo vetor\n");
	reset_vet(vet,n);
	print_vet(vet,n);
	printf("Vetor Ordenado Heap\n");
	heapsort(vet,n);
	print_vet(vet,n);
	printf("\n");

	printf("Novo vetor\n");
	reset_vet(vet,n);
	print_vet(vet,n);
	printf("Vetor Ordenado Quick\n");
	quicksort(vet,0,n-1,n);
	print_vet(vet,n);
	printf("\n");

	printf("Novo vetor\n");
	reset_vet(vet,n);
	print_vet(vet,n);
	printf("Vetor Ordenado Counting\n");
	countingsort(vet,n);
	print_vet(vet,n);
	printf("\n");
}