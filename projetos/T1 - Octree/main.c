/*	Programa que identifica colisões de objetos 3d, analisando caixas em torno do objeto
	Autor: Matheus Sanchez;
	NºUsp: 9081453;	
	ICMC-USP 2017;
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef int bool;
#define true 1;		// definições para melhor entendimento
#define false 0;	

typedef struct _ponto{
	double x,y,z;		// estrutura que define um ponto com coordenadas x,y,z
}ponto;

typedef struct _triangulo{
	ponto *cord;		// estrutura que define um triângulo com um ponteiro para os pontos;
}triangulo;


typedef struct _node{	// estrutura que define um node, usado para guardar				
	int levels;			// as 8 coordenadas da box/octante e o level máximo dos octantes;
	ponto *cord; 
	ponto *min;
	ponto *max;		
}node;					

node* gera_no(){ 	//função para alocação de um nó

	node* no = (node*) malloc(sizeof(node)); 
	no->cord = malloc(sizeof(ponto)*8);		//como nosso nó representa uma caixa, são 8 pontos;
	no->min = malloc(sizeof(ponto));
	no->max = malloc(sizeof(ponto));

	return no;
}
void desaloca_no(node* no){ 	// função que desaloca o nó

	free(no->cord);	
	free(no->min);
	free(no->max);
	free(no);
}

void desaloca_malha(triangulo* malha, int n){ // função que desaloca a malha

	for (int i = 0; i < n; ++i){	// para cada triângulo
		free(malha[i].cord);		// desaloque as coordenadas
	}

	free(malha);

}

triangulo* gera_malha(int n){		// função para alocação de triângulos -> n é a quantidade deles;

	triangulo* malha = malloc(sizeof(triangulo) * n);

	for (int i = 0; i < n; ++i){
		malha[i].cord = malloc(sizeof(ponto) * 3);	// para cada triângulo, alocamos 3 pontos;
	}

	return malha;

}

void le_ponto(ponto *p){ 	
	scanf("%lf %lf %lf",&p->x,&p->y,&p->z);	// função que lê um ponto;
}

void calcula_min_max(node* caixa){ //função que cálculo ponto mínimo e máximo do octante atual(nó)

	*caixa->min = caixa->cord[0];
	*caixa->max = caixa->cord[0];    	// setamos as coordenadas de mínimo e máximo para podermos comparar

	for (int i = 1; i < 5; ++i){ 				 // podemos percorrer apenas 5 pontos do cubo pois
		if(caixa->cord[i].x < caixa->min->x){	// estaremos necessariamente passando por pelos max/min	
			caixa->min->x = caixa->cord[i].x;
		}
		if(caixa->cord[i].y < caixa->min->y){		
			caixa->min->y = caixa->cord[i].y;		//ponto minimo
		}
		if(caixa->cord[i].z < caixa->min->z){
			caixa->min->z = caixa->cord[i].z;
		}

		if(caixa->cord[i].x > caixa->max->x){		//ponto maximo
			caixa->max->x = caixa->cord[i].x;
		}
		if(caixa->cord[i].y > caixa->max->y){
			caixa->max->y = caixa->cord[i].y;		
		}
		if(caixa->cord[i].z > caixa->max->z){
			caixa->max->z = caixa->cord[i].z;
		}
	}
}
void calcula_mproximo(ponto* colisor,node* pai,ponto* p_proximo){ // função que calcula o ponto mais próximo do ponto colisor

	double dist = sqrt(pow((pai->cord[0].x - colisor->x),2) + pow((pai->cord[0].y - colisor->y),2) + pow((pai->cord[0].z - colisor->z),2));
	double aux = 0;  // calculamos a distância entre os ponto colisor e o primeiro ponto do octante

	int pm = 0;
	for (int i = 1; i < 8; ++i){
		aux = sqrt(pow((pai->cord[i].x - colisor->x),2) + pow((pai->cord[i].y - colisor->y),2) + pow((pai->cord[i].z - colisor->z),2));

		if(dist > aux){
			dist = aux;
			pm = i;
		}
	}	

	*p_proximo = pai->cord[pm]; 	

	// ATENÇÃO: repare que o octante vai ser construído a partir da menor distância entre o colisor e um dos vértices,
	// existem casos em que a distância do colisor aos vértices podem ser as mesmas, neste caso avalie cada octante
	// separadamente, isto é, verifique se há triângulos dentro deles antes de selecionar o octante. Está implementação
	// não contém isso entretanto as funções estão prontas. 
}

void exibe_caixa(node * raiz){	// função de debug usada para exibir as coordenadas da caixa

	for (int i = 0; i < 8; ++i){
		printf("Ponto %d\n", i+1);
		printf("%f - %f - %f\n",raiz->cord[i].x,raiz->cord[i].y,raiz->cord[i].z);
	}
	printf("Level atual -> %d\n",raiz->levels);

}

void exibe_ponto(ponto* p){		// função de debug que exibe as coordenadas do ponto
	printf("X - %f Y - %f Z - %f\n",p->x,p->y,p->z);
}

void gera_octante(ponto* p,ponto *min, ponto* max,node* atual){ // função que gera um novo octante 

	atual->cord[0] = *p;	// recebe o ponto que pertence ao octante que vai ser gerado
							// que é necessariamente o ponto mais próximo do ponto colisor pertencente
							// ao octante "pai"	

	for (int i = 1; i < 8; ++i){

		if(i>=4){
			atual->cord[i].x = (min->x + max->x)/2.0;		//permuta as coordenadas entre o ponto 
		}else{												// que ja pertence ao octante e o ponto medio entre
			atual->cord[i].x = p->x;						// o minimo e o máximo do octante "pai"
		}

		if(i==2||i==3||i==6||i==7){
			atual->cord[i].y = (min->y + max->y)/2.0;
		}else{
			atual->cord[i].y = p->y;
		}

		if(i%2 != 0){
			atual->cord[i].z = (min->z + max->z)/2.0;
		}else{
			atual->cord[i].z = p->z;
		}
	}

	calcula_min_max(atual); // calcula novamente os pontos mínimos e máximos agora do novo octante
	atual->levels--;		// diminui o level 

}

bool colide(ponto* min,ponto *max, triangulo *malha, int num_triangulos){  // função que verifica se a malha esta dentro do octante atual
 	
 	ponto aux;	// ponto auxiliar

	for (int i = 0; i < num_triangulos; ++i){	// para cada triangulo

		for (int j = 0; j < 3; ++j){
			if(malha[i].cord[j].x <= max->x && malha[i].cord[j].y <= max->y && malha[i].cord[j].z <= max->z &&
				malha[i].cord[j].x >= min->x && malha[i].cord[j].y >= min->y && malha[i].cord[j].z >= min->z){
				return true;		// verifica se um dos três pontos está no octante atual
			}
		}
		
		aux.x = (malha[i].cord[0].x + malha[i].cord[1].x)/2.0;		// aresta 01
		aux.y = (malha[i].cord[0].y + malha[i].cord[1].y)/2.0;		// caso os vértices não estejam contidos no octante
		aux.z = (malha[i].cord[0].z + malha[i].cord[1].z)/2.0;		// faremos a verificação do ponto médio de cada aresta
				
		if(aux.x <= max->x && aux.y <= max->y && aux.z <= max->z &&
			aux.x >= min->x && aux.y >= min->y && aux.z >= min->z){	
			return true;
		}

		aux.x = (malha[i].cord[1].x + malha[i].cord[2].x)/2.0;
		aux.y = (malha[i].cord[1].y + malha[i].cord[2].y)/2.0;		//aresta 02
		aux.z = (malha[i].cord[1].z + malha[i].cord[2].z)/2.0;

		if(aux.x <= max->x && aux.y <= max->y && aux.z <= max->z &&
			aux.x >= min->x && aux.y >= min->y && aux.z >= min->z){
			return true;
		}

		aux.x = (malha[i].cord[2].x + malha[i].cord[0].x)/2.0;
		aux.y = (malha[i].cord[2].y + malha[i].cord[0].y)/2.0;		//aresta 03
		aux.z = (malha[i].cord[2].z + malha[i].cord[0].z)/2.0;

		if(aux.x <= max->x && aux.y <= max->y && aux.z <= max->z &&
			aux.x >= min->x && aux.y >= min->y && aux.z >= min->z){
			return true;
		}

		// repare que essa função é uma aproximação do problema ideal, uma vez que para haver uma colisão podemos ter
		// um ponto que não seja aresta nem ponto médio dentro do octante ;
	}

	return false;
}

bool ponto_caixa(node* raiz,ponto *colisor){	// funçaõ que avalia se o ponto colisor esta dentro da caixa inicial

	if(colisor->x <= raiz->max->x && colisor->y <= raiz->max->y && colisor->z <= raiz->max->z &&
		colisor->x >= raiz->min->x && colisor->y >= raiz->min->y && colisor->z >= raiz->min->z){ 
		return true;	// se as coordenadas do ponto colisor esta entre os mínimos e máximos da caixa, retorne true

	}else{
		return false;
	}		

	// repare que esta função não precisa ser chamada quando se diminui os octantes, pois se o ponto colisor esta
	// dentro da caixa inicial, necessariamente vai estar dentro de um dos octantes menores (filhos).
}	

void contrutor_box(node* raiz, triangulo* malha, int num_triangulos){ // função que gera a caixa inicial, caso isso não seja 
																	// dado na entrada	
	ponto min_malha,max_malha;

	min_malha = malha[0].cord[0];						// setamos mínimo e máximo para o ponto inicial
	max_malha = malha[0].cord[0];						// para tornar possível a comparação

	for (int i = 0; i < num_triangulos; ++i){			// para cara um dos triângulos
		for (int j = 0; j < 3; ++j){					// avaliamos os três pontos
			if(malha[i].cord[j].x > max_malha.x){
				max_malha.x = malha[i].cord[j].x;
			}
			if(malha[i].cord[j].y > max_malha.y){		//e selecionamos o ponto máximo	
				max_malha.y = malha[i].cord[j].y;
			}
			if(malha[i].cord[j].z > max_malha.z){		
				max_malha.z = malha[i].cord[j].z;
			}
			if(malha[i].cord[j].x < min_malha.x){		// e mínimo
				min_malha.x = malha[i].cord[j].x;
			}
			if(malha[i].cord[j].y < min_malha.y){
				min_malha.y = malha[i].cord[j].y;
			}
			if(malha[i].cord[j].z < min_malha.z){
				min_malha.z = malha[i].cord[j].z;
			}
		}
	}

	for (int i = 0; i < 8; ++i){					// permutação dos pontos máximos e mínimos para criar uma caixa
		if(i>=4){									// em volta da malha
			raiz->cord[i].x = max_malha.x;
			
		}else{
			raiz->cord[i].x = min_malha.x;
		}

		if(i==2||i==3||i==6||i==7){
			raiz->cord[i].y = max_malha.y;
		}else{
			raiz->cord[i].y = min_malha.y;
		}

		if(i%2 != 0){
			raiz->cord[i].z = min_malha.z;
		}else{
			raiz->cord[i].z = max_malha.z;
		}
	}
}

void le_triangulos(triangulo* malha, int num_triangulos){	 // função que faz a leitura dos triângulos

	for (int i = 0; i < num_triangulos; ++i){	//leitura dos triangulos
		for (int j = 0; j < 3; ++j){
			le_ponto(&(malha[i].cord[j]));		//para cada triangulo lemos 3 pontos
		}
	}
}


int main(int argc, char const *argv[]){
	int caixa;
	scanf("%d",&caixa);	// leitura se a entrada contém a caixa delimitadora

	node *raiz = gera_no();		// função que aloca um nó 
	scanf("%d",&raiz->levels);	

	int num_triangulos;	
	scanf("%d",&num_triangulos);	// leitura do número de triângulos da entrada

	triangulo * malha =  gera_malha(num_triangulos); // função que gera um array de triângulos;

	ponto *colisor = malloc(sizeof(ponto));	//alocamos um ponto para ser o colisor
	le_ponto(colisor); //leitura do ponto;

	le_triangulos(malha,num_triangulos); // leitura dos triangulos

	if(caixa == 0){												
		for (int i = 0; i < 8; ++i){
			le_ponto(&(raiz->cord[i]));	// caso a entrada contenha a caixa delimitadora, lemos 
		}								//	os oito pontos respectivos da caixa					
	}else{
			contrutor_box(raiz,malha,num_triangulos);	// caso a entrada não contenha a caixa, construimos uma			
		}

	calcula_min_max(raiz);					// função para cálculo do ponto mínimo/máximo
	
	
	if(ponto_caixa(raiz,colisor)){		// se o ponto estiver na caixa inicial

		ponto *p_proximo = malloc(sizeof(ponto)); // alocamos um ponto para indicar qual o ponto mais

		while(raiz->levels>=0){							//vamos diminuir o octante até o level pedido

			calcula_mproximo(colisor,raiz,p_proximo); 		// função que cálcula o ponto mais próximo,
															//pertencente ao octante, do ponto colisor	
			
			gera_octante(p_proximo,raiz->min,raiz->max,raiz);	// gera o novo octante
		}

		if(colide(raiz->min,raiz->max,malha,num_triangulos)){	// função que verifica se algum triangulo
			printf("%d\n", 1);									// esta dentro da caixa atual		

		}else{											// repare que este if pode ser colocado dentro do while
			printf("%d\n", 0);							// uma vez que no while diminui os octantes até o level 
		}												// pedido -> podemos diminuir um level e verificar se a malha esta dentro	
														// nesta aplicação optamos por outra interpretação 

		free(p_proximo);

	}else{
		printf("%d\n", 0);			//caso o ponto não esteja na caixa inicial, retorne 0;
	}

	desaloca_malha(malha, num_triangulos);	// função que desaloca a malha
	desaloca_no(raiz);						// função que desaloca o nó
	free(colisor);
	return 0;
}