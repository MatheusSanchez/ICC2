#include "analisadores.h"
#include <string.h>
#include <stdio.h>

void tira_espaco(json *objeto, int *pont){
	while(objeto->exp[*pont] == 32){		// tira os espaços da string deixando o apontador no primeiro caracter
		(*pont)++;							// válido
	}
}

int analisador_objeto(cont*contador,json *objeto, int *pont){

	(*pont)++;					// neste ponto estamos no caracter "{", entao avançaremos para o proximo
	tira_espaco(objeto,pont);	// tirando possíveis espaços

	if(!(analisador_pair(contador,objeto,pont))){	//um objeto pode ser por vários "pairs" entao chamaremos a função que analisa este tipo de dado
		return 0;
	}else{
		tira_espaco(objeto,pont);		
		if(objeto->exp[*pont] == '}'){		// caracter que indica o fim do objeto;
			(*pont)++;
			contador->objeto++;
			return 1;
		}else{
			return 0;
		}

	}
}

int analisador_string(cont*contador,json *objeto, int *pont){
	(*pont)++;					//ignorando a primeira '"'
	tira_espaco(objeto,pont);	

	while(objeto->exp[*pont] != '"' && objeto->n_caracteres > *pont){ // enquanto nao chegar o final da string
		if(objeto->exp[*pont] == '\b' || objeto->exp[*pont] == '\f' || objeto->exp[*pont] == '\n'
			|| objeto->exp[*pont] == '\r' || objeto->exp[*pont] == '\t'
			){
			return 0;
		} // caracteres invalidos


		if(objeto->exp[*pont] == '\\'){ // caso achemos uma barra, deveremos vereficar novamente os caracteres de controle;
			(*pont)++;					// este if deveria ser substituido pelo de cima, mas por algum motivo 
										// quando o caracter especial esta no meio da string o programa conta como dois caracteres;
										// entao verificaremos de novo
			if((objeto->exp[(*pont)] == '\\' || objeto->exp[(*pont)] == 'f'
			|| objeto->exp[(*pont)] == 'b' || objeto->exp[(*pont)] == 'n' || objeto->exp[(*pont)] == 'r'
			|| objeto->exp[(*pont)] == 't' || objeto->exp[(*pont)] == '"')){
				return 0;
			}else{
				(*pont)--;
			}
		}
		(*pont)++;
	}

	if(objeto->exp[*pont] == '"'){	// fechando a string
		(*pont)++;
		tira_espaco(objeto,pont);
		if(objeto->exp[*pont] == ':' || objeto->exp[*pont] == ',' || objeto->exp[*pont] == ']' || objeto->exp[*pont] == '}'){
			contador->string++; // caracteres válidos após o fechamento da strig;
			return 1;
		}else{
			return 0;	
		}
	}else{
		return 0;
	}
}

int analisador_pair(cont*contador,json *objeto, int *pont){

	if(objeto->exp[*pont] == '"'){		// todo pair começa com uma string

		if(!(analisador_string(contador,objeto,pont))){	// entao analisaremos ela
			return 0;
		}

		tira_espaco(objeto,pont);
		if(!(objeto->exp[*pont] == ':')){	// depois da string temos um ":"
			return 0;	
		}

		(*pont)++;	
		tira_espaco(objeto,pont);

		

		if(!(analisador_value(contador,objeto,pont))){	// e depois um value, que pode ser qualquer um

			return 0;
		}else{
			
			if(objeto->exp[*pont] == ','){ // teremos ',' caso o pair seja mais de um
				contador->pair++;
				(*pont)++;
				tira_espaco(objeto,pont);
				return analisador_pair(contador,objeto,pont);
			}else if(objeto->exp[*pont] == '}'){ // fechamento do objeto (que contém o pair);
				contador->pair++;
				return 1;		
			}else{
				return 0;
			}
			
		}

	}else{
		return 0;
	}
}

int analisador_numerico(cont*contador,json *objeto, int *pont){
	if(objeto->exp[*pont] == '+' || objeto->exp[*pont] == '-'){ // ignoramos o sinal do numero caso ele exista
		(*pont)++;
	}

	if((objeto->exp[*pont] >= '0' && objeto->exp[*pont] <= '9')){ //caso a string possua pelo menos um numero

		while((objeto->exp[*pont] >= '0' && objeto->exp[*pont] <= '9')){ // pode conter outros
			(*pont)++;
		}
		if(objeto->exp[*pont] == 'e' || objeto->exp[*pont] == 'E' || objeto->exp[*pont] == '.'){ // e os caracters e/E/'.'
			(*pont)++;	
			return analisador_numerico(contador,objeto,pont);			// entretando o próximo caracter tem que ser um numero
		}	

	}else{
		return 0;
	}

	contador->numeros++;  // incrementando o contador
	tira_espaco(objeto,pont);

	if(objeto->exp[*pont] == ',' || objeto->exp[*pont] == ']' || objeto->exp[*pont] == '}'){ // numeros precisam ser seguidos desses caracteres;
		return 1;
	}else{
		return 0;
	}
}
int analisador_true(cont*contador,json *objeto, int *pont){		
	char aux[5];

	for (int i = 0; i < 4; ++i){
		aux[i] = objeto->exp[*pont];		// vamos copiar as proximas 4 letras para um vetor auxiliar
		(*pont)++;
	}

	aux[4] = '\0';

	if(strcmp(aux,"true") == 0){		// e comparar com a palavra true
		tira_espaco(objeto,pont);
		if(objeto->exp[*pont] == '}' || objeto->exp[*pont] == ']' || objeto->exp[*pont] == ','){ // os valores booleanos podem ser acompanhados por esses tipos
			contador->n_true++;																	// tipo de char
			return 1;
		}else{
			return 0;
		}
	}else{
		return 0;
	}

	

}



int analisador_false(cont*contador,json *objeto, int *pont){
	char aux[6];

	for (int i = 0; i < 5; ++i){
		aux[i] = objeto->exp[*pont];	//mesmo raciocínio do true porém para o false
		(*pont)++;
	}

	aux[5] = '\0';

	if(strcmp(aux,"false") == 0){
		tira_espaco(objeto,pont);
		if(objeto->exp[*pont] == '}' || objeto->exp[*pont] == ']' || objeto->exp[*pont] == ','){
			contador->n_false++;
			return 1;
		}else{
			return 0;
		}
	}else{
		return 0;
	}


}

int analisador_null(cont*contador,json *objeto, int *pont){

	char aux[5];

	for (int i = 0; i < 4; ++i){
		aux[i] = objeto->exp[*pont];		//mesmo raciocínio do true porém para o null 
		(*pont)++;
	}

	aux[4] = '\0';

	if(strcmp(aux,"null") == 0){
		tira_espaco(objeto,pont);
		if(objeto->exp[*pont] == '}' || objeto->exp[*pont] == ']' || objeto->exp[*pont] == ','){
			contador->nulls++;
			return 1;
		}else{
			return 0;
		}
	}else{
		return 0;
	}

	

}

int analisador_boolean(cont*contador,json *objeto, int *pont){ // o analisador de booleano poderia ser descartado
															// por esses mesmos ifs dentro do analisador de value, entretanto	
															// para seguir o padrão do código agruparemos eles
	if(objeto->exp[*pont] == 't'){								
		return analisador_true(contador,objeto,pont);		// true;
	}else if(objeto->exp[*pont] == 'f'){
		return analisador_false(contador,objeto,pont);    // false;
	}else if(objeto->exp[*pont] == 'n'){
		return analisador_null(contador,objeto,pont); 		// null;
	}

	return 0;
}



int analisador_value(cont*contador,json *objeto, int *pont){ // analisa todos os values chamando funções auxiliares
	tira_espaco(objeto,pont);	// tirando os espaços do começo do json
								// como cada value começa com um char diferente, trataremos um por um	
	

	if(objeto->exp[*pont] == '{'){					// o value é um objeto
				if(!(analisador_objeto(contador,objeto,pont))){
					return 0;
				}else{
					return 1;
				}

	}else if(objeto->exp[*pont] == '"'){		// o value é uma string
				if(!(analisador_string(contador,objeto,pont))){
					return 0;
				}else{
					return 1;
				}	
			
	}else if((objeto->exp[*pont] >= '0' && objeto->exp[*pont] <= '9') || (objeto->exp[*pont] == '+'
		|| objeto->exp[*pont] == '-')){	 // o value é um numero, neste caso o primeiro char pode ser um sinal também
				
		if(!(analisador_numerico(contador,objeto,pont))){
			return 0;
		}else{
			return 1;
		}

	}else if(objeto->exp[*pont] == 't' || objeto->exp[*pont] == 'f'
					||objeto->exp[*pont] == 'n'){        // o value é um valor booleano ou null					
		if(!(analisador_boolean(contador,objeto,pont))){
			return 0;
		}else{
			return 1;
		}
	}else if(objeto->exp[*pont] == '['){ // o value é um array;
		
		if(!(analisador_array(contador,objeto,pont))){
			return 0;
		}else{
			return 1;
		}

	}else{
		return 0;
	}

	// em todas as funções retornamos o 1/0 retornando para a função principal

	return 1;
}


int analisador_array(cont*contador,json *objeto, int *pont){ // analisador de array;
	
	(*pont)++;	// sabemos que neste ponto o caracter atual é '[', então pulamos para o próximo

	tira_espaco(objeto,pont); // tirando os possíveis espaços

	while(objeto->exp[*pont] != ']' && *pont < objeto->n_caracteres){ // enquanto não acharmos o final do array, respeitando o numero 
		tira_espaco(objeto,pont);										// de caracteres da string que representa o json;
	
		if(objeto->exp[*pont] != ','){ // os values do array são separados por ','
			
			if(!(analisador_value(contador,objeto,pont))){		//  caso nao seja uma virgula, chamamos o anasilador de value
				return 0;
			}else{
				tira_espaco(objeto,pont);			// caso o analisador de value retorne 1, tiramos os espaços
				if(objeto->exp[*pont] != ','){		// e vereficamos se há algum outro value	
					break;		// caso não haja, podemos parar o laço
				}
			}

		}
		(*pont)++;
	}

	if(objeto->exp[*pont] == ']'){		// o último caracter precisa ser necessariamente um ']'
		contador->array++;		// incrementamos o contador de array;
		(*pont)++;			// e caminhamos no objeto json, isto é, vamos para o próximo caracter;
		return 1;
	}else{
		return 0;
	}

}