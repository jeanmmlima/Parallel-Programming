#include <stdio.h>
#include <string.h> 
#include <math.h>
#include <stdlib.h>
#include <time.h>

void alocaVetores(double** local_x, double** local_y, int local_n); 
void somaPrefixa(double vetor[], double vetorSomas[], int n);

int main(){

	double *vetor, *vetorSomas;
	int n;

	printf("Informe a ordem do vetor: \n");
	scanf("%d",&n);
	double vetor1[n],vetor2[n];

	alocaVetores(&vetor,&vetorSomas,n);
	int i;
	printf("Entre com o vetor: \n");
	for (i = 0; i < n; i++)
         scanf("%lf", &vetor1[i]);
     vetor2[0] = vetor1[0];
    for (i=1; i< n; i++){
		vetor2[i] = vetor1[i] + vetor2[i-1];
		printf("Soma Prefixada: \n");
		printf("%f\n", &vetor2[i]);
	}

    return 0;
    
}

void alocaVetores(
      double** local_x, 
	  double** local_y,
      int local_n) {

   *local_x = malloc(local_n*sizeof(double));
   *local_y = malloc(local_n*sizeof(double));
} 

void somaPrefixa(double vetor[], double vetorSomas[], int n){

	int i_local;
	vetorSomas[0] = vetor[0];
	for (i_local = 1; i_local < n; i_local++){
		vetorSomas[i_local] = vetor[i_local] + vetorSomas[i_local-1];
		printf("Soma Prefixada: \n");
		printf("%d\n", vetorSomas[i_local]);
	}

}