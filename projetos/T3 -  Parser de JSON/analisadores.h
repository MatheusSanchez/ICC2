#ifndef LISTA_H
#define LISTA_H

typedef struct cont{	// estrutura de contador para os diferentes tipos a serem analisados
	int objeto;
	int array;
	int pair;
	int string;
	int numeros;
	int n_true;
	int n_false;
	int nulls;
}cont;

typedef struct _json{
	char exp[200];
	int n_caracteres;	//expressao json e numero de caracteres da mesma
}json;


//funções que analisam a gramatica e os tokens, retorna 1 em caso de sucesso e 0 em caso de falha
//todas as funções tem como parâmetro um ponteiro para inteiro que é o indice atual, ou seja,
// o caracter corrente do json

int analisador_array(cont*contador,json *objeto, int *pont);  //analisa os arrays
int analisador_string(cont*contador,json *objeto, int *pont);	//analisa as strings
int analisador_numero(cont*contador,json *objeto, int *pont); //analisa os numeros
int analisador_boolean(cont*contador,json *objeto, int *pont); //analisa os valores true,false e null

int analisador_objeto(cont*contador,json *objeto, int *pont); // analisa um objeto
int analisador_pair(cont*contador,json *objeto, int *pont);	 // analisa um pair
int analisador_value(cont*contador,json *objeto, int *pont);  // analisa todos os values com auxílio das outras funções

int analisador_true(cont*contador,json *objeto, int *pont); // analisa um true
int analisador_null(cont*contador,json *objeto, int *pont);	// analisa um false
int analisador_false(cont*contador,json *objeto, int *pont); // // analisa um null

void tira_espaco(json *objeto, int *pont); // função auxiliar para tirar espaços do objeto json

#endif