typedef struct _operacoes{ // struct capaz de armazenar todas as operações
	int terminal;	
	int banco_o;		//banco origem, caso a operação seja transferencia
	int banco_d;		//banco destino, caso a operação seja transferencia
	char operacao;
	double valor;
}t_op;

typedef struct _no{		// no para a fila de operações da tabela hash
	t_op operacao;		// operação
	 struct _no* prox;	// próxima operação	
}t_no;

typedef struct _fila{		// fila com ponteiro para o primeiro e para o ultimo
	t_no* first;			// com isso faremos a inserção em O(1)
	t_no* last;	
}t_fila;

typedef struct caixa{	// cada caixa ou terminal contém duas tabelas hashing
	t_fila* operacoes;	// tabela de operaçẽos	
	t_fila* erros;		// tabela de erros
}t_caixa;

void cria_caixa(t_caixa** caixa); // cria os terminais que usaremos (4 existentes e 2 auxiliares);
void cria_transacao(t_caixa* caixa, t_op *op, int key, int erro); // função que coloca a operação na tabela hash
void cria_operacoes(t_fila **op); // função que aloca as tabelas dos terminais (erro e operacoes)
int hash_function(int n_terminal, int key); // seletor de qual função de hashing usar
void insere_fila(t_fila *f,t_op *op); // função que insere a operação na fila que esta na tabela
void relatorio(t_caixa *caixa);// gera os relatorios por banco
void audutoria(t_caixa *caixa, char op,int n,int terminal); // gera os relatorios de auditoria no fim do dia

int hash1(int key); // função de hash do terminal 1
int hash2(int key);	// função de hash do terminal 2
int hash3(int key);	// função de hash do terminal 3
int hash4(int key);	// função de hash do terminal 4

void exibe_caixa(t_caixa* caixa); // função de debug que exibe uma fila de operações
void free_fila(t_fila *f); // libera a memória de uma fila;
void free_terminal(t_caixa *terminal);