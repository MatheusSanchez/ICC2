#include <stdio.h>
#include "caixa.h"
#include <stdlib.h>

int main(){

	char vet[20],aux[10];	// vetor para guardar a linha e um auxiliar
	t_op op; // variavel do tipo operação 
	t_caixa *terminal; // array de caixas, 6 no total (4 existentes e 2 auxiliares);
	
	cria_caixa(&terminal); // criando array
	
	int i;int k;int flag = 0;

	while(fgets(vet,150,stdin)){// enquanto houver linhas para processamento
		
		i = 0;k = 0;
		op.banco_d = 0;
		op.valor = 0;

		for (i = 0; vet[i] != 32; i++){ // pegando o numero do terminal
			aux[i] = vet[i];	// o laço é necessário caso haja inserção de um terminal com mais de um digito
			aux[i+1] = '\0';		
		}

		i++;
		op.terminal = atoi(aux); // transformando uma string em inteiro

		if(vet[i] == 'S' || vet[i] == 'C' || vet[i] == 'T' || vet[i] == 'D'){ // caso a posição no vetor seja uma dessas letras, encontramos uma auditoria
			flag = 1;										// e paramos a computação de operações
			break;
		}else{
			
			for (k = 0; vet[i] != 32; i++,k++){		// pegando o banco de origem da operação
				aux[k] = vet[i];
				aux[k+1] = '\0';
			}
			i++;
			op.banco_o = atoi(aux); // transformando uma string em inteiro

			op.operacao = vet[i]; // pegando a operação, não precisamos do laço pois a operação é caracterizada por apenas um caracter
			i+=2;

			if(op.operacao == 'S' || op.operacao == 'D'){ // caso a operação seja de saque ou deposito
				for (k = 0; vet[i] != '\0'; i++,k++){
					aux[k] = vet[i];
					aux[k+1] = '\0';	//pegaremos os próximos numeros que indicaram o valor da operação
				}
				op.valor = atof(aux); // transformando uma string em float
			}

			if(op.operacao == 'T'){ // caso a operação seja transferencia

				for (k = 0; vet[i] != '\0' && vet[i] != 32; i++,k++){
					aux[k] = vet[i];	// vamos pegar os próximos numeros
					aux[k+1] = '\0'; 	// isto pode indicar o código do banco de destino ou o valor da transferencia
				}

				if(vet[i] == 32){ // se encontramos um espaço, estamos em uma transferencia externa
					i++;
					op.banco_d = atoi(aux); // ja temos o banco de destino

					for (k = 0; vet[i] != '\0'; i++,k++){
						aux[k] = vet[i];		// vamos ler o valor da transferencia aqui
						aux[k+1] = '\0';	
					}
					op.valor = atof(aux);
				
				}else{ // caso não seja um espaço, estamos no fim da linha
					op.valor = atof(aux);	// o que indica que a transferencia é interna;
				}

			}
			// neste ponto lemos todas as informações da operação, precisamos verificar se esta é uma operação com erro
			
			char erro = getchar();// vamos ler o próximo caracter
			
			if(erro != 'E'){ // caso não seja "E", a operação segue
				
				ungetc (erro,stdin); //devolvemos o caracter lido para o stdin
				
				cria_transacao(&(terminal[op.terminal-1]), &op,hash_function(op.terminal, op.banco_o),0); // crie uma transação, na hash do terminal pedido, passando como key o id do banco 
				cria_transacao(&(terminal[4]), &op, hash_function(5, op.banco_o),0); // crie uma transação, na hash do terminal 5, passando como key o id do banco. Isto faz com que tenhamos acesso em O(1)
																						// ao total se tivermos os ids dos bancos;
				if(op.operacao == 'T' && op.banco_d != 0){	 // caso a operação seja uma transferencia externa									
					op.operacao = 'E';	// vamos salvar nos dois terminais do mesmo jeito, entretando salvaremos no banco destino para facilitar as contas;
					cria_transacao(&(terminal[op.terminal-1]), &op,hash_function(op.terminal, op.banco_d),0);
					cria_transacao(&(terminal[4]), &op, hash_function(5, op.banco_d),0);
					op.operacao = 'T';	
				}

				// desde modo a inserção fica em O(1) e com os ids dos bancos temos o acesso a todas operações de um banco x em O(1) também,
				// alem de possibilitar a inserção de outros bancos e outras operações

				cria_transacao(&(terminal[5]), &op, hash_function(5, ((op.operacao)+(op.terminal*3))),0); // salvamos a operação no terminal 6 passando como key, a operação e o terminal. Assim salvamos
																											// a ordem cronológica dos dados separados por operação e terminal.
			}else{	//caso contrario trataremos o erro

				cria_transacao(&(terminal[op.terminal-1]), &op,hash_function(op.terminal, op.banco_o),1); // no caso de erro trataremos a inserção igualmente
				cria_transacao(&(terminal[4]), &op, hash_function(5, op.banco_o),1);	// mas na hash de erro de cada terminal, isso é caracterizado pela flag um no final
				fgets(vet,150,stdin);	// tira a linha de erro
			}	

		}	
	}

	printf("===TERMINAL 1===\n");
	relatorio(&(terminal[0])); // relatorio terminal 1
	printf("===TERMINAL 2===\n");
	relatorio(&(terminal[1]));	// relatorio terminal 2
	printf("===TERMINAL 3===\n");
	relatorio(&(terminal[2])); // relatorio terminal 3
	printf("===TERMINAL 4===\n");
	relatorio(&(terminal[3])); // relatorio terminal 4
	printf("===TOTAL===\n"); 
	relatorio(&(terminal[4])); // relatorio terminal 5 -> total
	 
	if(flag == 1){ // se a flag for um temos uma auditoria
		printf("===AUDITORIA===\n");
	
		do{	
			i=0;
			op.terminal = vet[i]-48; // terminal da auditoria
			i+=2;
			char c = vet[i]; // char da auditoria
			i+=2;
			int n = vet[i] - 48; // numero de operaçẽos
			if(c == 'S'){
				printf("===SAQUE TERMINAL %d===\n", op.terminal);	
			}else if(c == 'C'){
				printf("===CONSULTA TERMINAL %d===\n", op.terminal);	
			}else if(c == 'D'){
				printf("===DEPOSITO TERMINAL %d===\n", op.terminal);	
			}else if(c == 'T'){
				printf("===TRANSFERENCIA TERMINAL %d===\n", op.terminal);	
			}
	
			audutoria(&(terminal[5]),c,n,op.terminal); // terminal 06 com as operações separadas por operação
														// com o terminal e a operação caimos direto na fila que queremos, ou seja, O(1)

		}while(fgets(vet,150,stdin)); // enquanto tiver linhas para ler

	}
	
	free_terminal(terminal);	// free terminais 

	return 0;
}


