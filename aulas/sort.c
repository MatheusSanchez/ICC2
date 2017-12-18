#include "sort.h"
#include "vet.h"

void swap(int *vet,int i, int j){ // troca elementos das posições i/j
	int aux = vet[i];
	vet[i] = vet[j];
	vet[j] = aux;
}

void bubble(int *vet,int n){
	for (int i = n; i > 0; i--){

		for (int j = 1; j < i; j++){
			if(vet[j-1] > vet[j]){
				//print_vet(vet,n);
				swap(vet,j-1,j);
			}else{
				//print_vet(vet,n);	
			}
		}

	}
}

void selection(int *vet,int n){

	int menor;

	
	for (int j = 0; j < n; j++){
		menor = j;

		for (int i = j+1; i < n; i++){
			if(vet[i] < vet[menor]){
				menor = i;
			}
		}
		//print_vet(vet,n);
		swap(vet,j,menor);
	}
}

void insertion(int *vet,int n){

	int key,j;

	for (int i = 1; i < n; i++){
		j=i;
		key = vet[j];
		
		while(j > 0 && vet[j-1] > key){
			vet[j] = vet[j-1];
			j--;
			
		}
		vet[j] = key;
	}

}

void mergesort(int *vet,int ini, int fim, int n){

	int meio = (fim+ini)/2;

	if(ini < fim){
		mergesort(vet,ini,meio,n);
		mergesort(vet,meio+1,fim,n);
		merge(vet,ini,meio,fim);
		print_vet(vet,n);
	}

}

void merge(int *vet,int ini, int meio, int fim){

	int *vet_aux = (int *) malloc(sizeof(int)*(fim - ini));

	int i,j,k;
	i = ini;
	j = meio+1;
	k = 0;

	while(i <= meio && j <= fim){
		if(vet[i] < vet[j]){
			vet_aux[k] = vet[i];
			i++;
		}else{
			vet_aux[k] = vet[j];
			j++;
		}
		k++;
	}	

	while(i <= meio){
		vet_aux[k] = vet[i];
		k++;i++;
	}

	while(j <= fim){
		vet_aux[k] = vet[j];
		k++;j++;
	}

	for (k=0,i = ini ; i<=fim; i++,k++){
		vet[i] = vet_aux[k];
	}


}


void heapsort(int *vet,int n){

	build_maxHeap(vet,n);

	for (int i = n-1; i > 0; i--){
		swap(vet,0,i);
		build_maxHeap(vet,i);
	}


}


void build_maxHeap(int *vet,int n){
	int i;
	if(n%2 == 0){
		i = (n/2);
	}else{
		i = (n/2) - 1;
	}

	for (; i >= 0 ; i--){
		swap(vet,0,i);
		maxHeap(vet,n,i);
	}

}


void maxHeap(int *vet,int n,int i){
	int pos_maior = i;
	int f_esq = (i*2)+1;
	int f_dir = (i*2)+2;

	if(f_esq < n && vet[pos_maior] < vet[f_esq]){
		pos_maior = f_esq;
	}

	if(f_dir < n && vet[pos_maior] < vet[f_dir]){
		pos_maior = f_dir;
	}

	if(pos_maior != i){
		swap(vet,i,pos_maior);
		maxHeap(vet,n,pos_maior);
	}
}


void quicksort(int *vet,int ini, int fim,int n){

	if(fim <= ini)return;

	int i,j,pivo;

	i = ini;
	j = fim - 1;
	pivo = fim;


	while(i < j){

		while(vet[i] < vet[pivo]) i++;
		while(j >= ini && vet[j] > vet[pivo]) j--;


		if(j > i){
			swap(vet,i,j);
		}

	}

	swap(vet,pivo,i);

	pivo = i;

	quicksort(vet,pivo+1,fim,n);
	quicksort(vet,ini,pivo-1,n);

}

void countingsort (int *vet,int n){
	int *vet_aux = NULL;
 	vet_aux = calloc(RANGE+1,sizeof(int));

	for (int i = 0; i < n; i++){
		vet_aux[vet[i]]++;
	}
	for (int i = 1; i <= RANGE; i++){
		vet_aux[i]+=vet_aux[i-1];
	}

	int *resp = malloc(sizeof(int)*(n));

	for (int i = 0; i < n; i++){
		resp[vet_aux[vet[i]]-1] = vet[i];
		vet_aux[vet[i]]--;
	}
	for (int i = 0; i < n; i++){
		vet[i] = resp[i];
	}


}