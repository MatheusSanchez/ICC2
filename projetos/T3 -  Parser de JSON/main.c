#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "analisadores.h"

void ini_cont(cont **contador){
	*contador = calloc(1, sizeof(cont)); 	// inicializa o contador
}

void ini_json(json **objeto){
	*objeto = malloc(sizeof(json)); // inicializa o json
}
	

int main (){

	json* objeto;
	cont *contador;

	int *pont = malloc(sizeof(int));	// contador passado para as funções
	int i = 1;	// contador das linhas
	
	ini_cont(&contador);
	ini_json(&objeto);

	while(fgets(objeto->exp,300,stdin)){		//enquanto tiver linhas para ler

		objeto->exp[strlen(objeto->exp)] = '\0';
		objeto->n_caracteres = strlen(objeto->exp); // numero de caracteres do json

		*pont = 0;	// contador da string em 0
		if(!(analisador_value(contador,objeto,pont))){ 	// caso o analisar de value retorne 0
			printf("Error line %d\n", i);				// printamos a linha 
			break;
		}
		i++;
	}

	printf("Number of Objects: %d\n",contador->objeto);
	printf("Number of Arrays: %d\n",contador->array);
	printf("Number of Pairs: %d\n",contador->pair);
	printf("Number of Strings: %d\n",contador->string); // print da resposta
	printf("Number of Numbers: %d\n",contador->numeros);
	printf("Number of Trues: %d\n",contador->n_true);
	printf("Number of Falses: %d\n",contador->n_false);
	printf("Number of Nulls: %d\n",contador->nulls);


	free(contador);
	free(objeto);
	free(pont);

	return 0;
}