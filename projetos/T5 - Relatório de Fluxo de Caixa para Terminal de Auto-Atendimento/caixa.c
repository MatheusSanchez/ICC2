#include <stdio.h>
#include <stdlib.h>
#include "caixa.h"

void cria_caixa(t_caixa **caixa){        // criamos todos dos terminais
	(*caixa) = malloc(sizeof(t_caixa)*6);
	for (int i = 0; i < 6; ++i){
		(*caixa)[i].operacoes = NULL;
	}
}

void cria_operacoes(t_fila **op){		// criamos o array de hash com 761 posições

	(*op) = malloc(sizeof(t_fila)*761);	// a hash é de estruturas do tipo t_fila

	for (int i = 0; i < 761; ++i){
		(*op)[i].first = NULL;
		(*op)[i].last = NULL;
	}
}

void cria_transacao(t_caixa* caixa, t_op *op, int key, int erro){ // criando uma transação
	if(erro == 0){
		if(caixa->operacoes == NULL){ // caso seja a primeira 
			cria_operacoes(&(caixa->operacoes));	// alocaremos a hash
		}
		insere_fila(&(caixa->operacoes[key]),op);	// inserimos na hash na key passada como parâmetro
	}else if(erro == 1){
		if(caixa->erros == NULL){
			cria_operacoes(&(caixa->erros));	// a mesma coisa entretando para o array de erro
		}
		insere_fila(&(caixa->erros[key]),op);	
	}
}


void insere_fila(t_fila *f,t_op *op){ // inserção na fila

	if(f->first == NULL){	// caso a fila não exista, 
		f->first = malloc(sizeof(t_no)); // alocamos ela
		f->last = f->first; // ultimo aponta para o primeiro 
		f->first->operacao = *op; // copiando a struct operação
		f->first->prox = NULL; // o próximo não existe
		
	}else{
		f->last->prox = malloc(sizeof(t_no)); // inserção depois do ultimo nó da fila
		f->last->prox->operacao = *op;
		f->last = f->last->prox; // atualizando o ultimo
	}
}

int hash_function(int n_terminal, int key){ // seletor de função de hash 

	if(n_terminal == 1){	// teminal 1
		return hash1(key);
	}else if(n_terminal == 2){ // teminal 2
		return hash2(key);
	}else if(n_terminal == 3){ // teminal 3
		return hash3(key);
	}else if(n_terminal == 4){ // teminal 4
		return hash4(key);
	}

	return hash1(key); // terminais auxiliares
}

int hash1(int key){
	int id = (((int)((key)*1.61803398875)) % 761);
	return id;	
}
int hash2(int key){
	int id = (((int)((key)*0.61803398875)) % 761);
	return id;	
}													// funções de hash
int hash3(int key){
	int id = (((int)((key)*1.57079632679)) % 761);
	return id;	
}
int hash4(int key){
	int id = (((int)((key)*0.78539816339)) % 761);
	return id;	
}



void exibe_caixa(t_caixa* caixa){ // função de debug que procura uma fila e exibe ela;
	if(caixa->operacoes != NULL){	
		for (int i = 0; i < 761; ++i){
			if(caixa->operacoes[i].first != NULL){
				printf("FILA NA POSIÇÃO %d\n",i);
				t_no *aux = caixa->operacoes[i].first;
				while(aux != NULL){
					printf("FILA NA POSIÇÃO %d\n",i);
					printf("Terminal %d\n", aux->operacao.terminal);
					printf("Banco Origem %d\n", aux->operacao.banco_o);
					printf("Banco Destino %d\n", aux->operacao.banco_d);
					printf("Operacao %c\n", aux->operacao.operacao);
					printf("Valor %lf\n", aux->operacao.valor);
					aux = aux->prox;
					printf("\n");
				}
			}
		}
	}
	if(caixa->erros != NULL){
		printf("ERROS\n");
		for (int i = 0; i < 761; ++i){
			if(caixa->erros[i].first != NULL){
				printf("FILA NA POSIÇÃO %d\n",i);
				t_no *aux = caixa->erros[i].first;
				while(aux != NULL){
					printf("FILA NA POSIÇÃO %d\n",i);
					printf("Terminal %d\n", aux->operacao.terminal);
					printf("Banco Origem %d\n", aux->operacao.banco_o);
					printf("Banco Destino %d\n", aux->operacao.banco_d);
					printf("Operacao %c\n", aux->operacao.operacao);
					printf("Valor %lf\n", aux->operacao.valor);
					aux = aux->prox;
					printf("\n");
				}
			}
		}
	}
}

void relatorio(t_caixa *caixa){ //função que gera o primeiro relatório ao fim do dia

	double moeda_e=0, moeda_s=0, transf_e=0,transf_s=0;
	int count = 0;

	if(caixa->operacoes != NULL){ // se existem operações 
		for (int i = 0; i < 761; i++){ // percorremos a fila
			if(caixa->operacoes[i].first != NULL){
				//printf("ACHEI POSIÇÃO %d\n", i);
				t_no *aux = caixa->operacoes[i].first; // primeiro no
				t_op *op;

				op = &(aux->operacao);
				int banco = op->banco_o;
				moeda_e=0;
				moeda_s=0;
				transf_e=0;
				transf_s=0;


				while(aux != NULL){ //enquanto não acharmos o final da fila

					op = &(aux->operacao);
					if(op->operacao == 'D'){  // operação de deposito
						moeda_e += op->valor; //incremento na variavel	
					}else if(op->operacao == 'S'){ // operação de saque
						moeda_s += op->valor;
					}else if(op->operacao == 'T' && op->banco_d != 0){ // transferencia
						transf_s += op->valor;
					}else if(op->operacao == 'E'){ // e transferencia externa (caso o banco em questão seja o banco de destino)
						transf_e += op->valor;
						count--;
					}

					count++;	// contador para saber o lucro
					aux = aux->prox;	//próximo nó
				}
				if(moeda_e != 0 ||  moeda_s != 0 || transf_e!= 0 || transf_s != 0){
					printf("Banco %d: ", banco);
					printf("Moeda +%.2lf -%.2lf Transferencia +%.2lf -%.2lf\n",moeda_e,moeda_s,transf_e,transf_s);	
				} 

				free_fila(&(caixa->operacoes[i])); // depois liberamos a memória alocada para uma fila
			}

		}
		printf("Lucro obtido: %d.00\n",count*3);
	}
	
	if(caixa->erros != NULL){ // a mesma analise é feita para os erros

		int saque=0;
		int deposito=0;
		int transf=0;
		int consulta=0;

		for (int i = 0; i < 761; i++){
			if(caixa->erros[i].first != NULL){

				t_no *aux = caixa->erros[i].first; // primeiro nó de erro
				t_op *op;
				op = &(aux->operacao);

				while(aux != NULL){

					op = &(aux->operacao);
					if(op->operacao == 'D'){ // entretando precisamos apenas saber quantos erros foram contabilizados;
						deposito++;
					}else if(op->operacao == 'S'){
						saque++;
					}else if(op->operacao == 'T'){
						transf++;
					}else if(op->operacao == 'C'){
						consulta++;
					}

					aux = aux->prox;	

				}
				free_fila(&(caixa->erros[i]));
			}
		}

		if(saque != 0){
			printf("Erros de saque: %d\n", saque);	
		}
		if(deposito != 0){
			printf("Erros de deposito: %d\n", deposito);	
		}
		if(consulta != 0){
			printf("Erros de consulta: %d\n", consulta);	
		}
		if(transf != 0){
			printf("Erros de transferencia: %d\n", transf);	
		}


		if (saque != 0 || deposito != 0 || transf != 0 || consulta != 0){
			printf("Total de erros: %d\n", saque+deposito+transf+consulta);
		}

	}
	
}// note que essa função pode ser substituida, se tivermos os ids de todos os bancos, para uma função O(1),
// entretanto deixaremos assim implementado pois podem ser adicionados novos bancos;

void audutoria(t_caixa *caixa, char op,int n,int terminal){ // função de auditoria ;

	int i = hash1((op)+(terminal*3)); // com o terminal e a operação achamos a fila de operações desejada para consulta
	t_no *aux = caixa->operacoes[i].first;
	int l = 1;// contador de operações 
	printf("Mostrando primeiros %d resultados\n", n);

	while(aux != NULL && n > 0){ // com o indice na hash apenas percorremos uma fila e printamos quantas operações foram pedidas
		if(op == 'T' && aux->operacao.banco_d != 0){
			printf("%d- Banco origem %d Banco destino %d %.2lf\n",l,aux->operacao.banco_o,aux->operacao.banco_d,aux->operacao.valor);
			n--;
			l++;
		}else if(op == 'T' && aux->operacao.banco_d == 0){	
			printf("%d- Banco origem %d Banco destino %d %.2lf\n",l,aux->operacao.banco_o,aux->operacao.banco_o,aux->operacao.valor);	
			n--;
			l++;		
		}else{
			printf("%d- Banco %d %.2lf\n",l,aux->operacao.banco_o,aux->operacao.valor);
			n--;
			l++;
		}			
		
		aux = aux->prox;
	}
	
	if(l == 1){ // se o contador continuar como 1, nenhuma operação foi realizada
		printf("Sem resultados\n");
	}
}

void free_fila(t_fila *f){

	t_no *no = f->first;
	t_no *aux = f->first;

	while(no != NULL){
		aux = no;
		no = no->prox;
		free(aux);
	}
		
	f->first = NULL;
	f->last = NULL;	
}

void free_terminal(t_caixa *terminal){
	for (int i = 0; i < 5; ++i){
		free(terminal[i].operacoes); // desaloca operação dos 6 terminais
		free(terminal[i].erros);	// desaloca os erros dos 6 terminais 
	}

	// as operações de relatorio e auditoria nao necessariamente passam por todas as filas do terminal 6
	// entao desalocaremos elas aqui

	for (int i = 0; i < 761; ++i){ // percorrendo toda hash
		if(terminal[5].operacoes[i].first != NULL){
			free_fila(&(terminal[5].operacoes[i]));
		}
	}

	free(terminal[5].operacoes); // desalocando as duas hash de ponteiros
	free(terminal[5].erros);

	free(terminal); // por fim da um free nos terminais
}