#include <stdio.h>
#include <string.h> 
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

void alocaVetores(double** local_x, double** local_y, int local_n, MPI_Comm comm); 
void somaPrefixa(double vetor[], double vetorSomas[], int local_n);
void ler_n(int* n_p, int* local_n_p, int my_rank, int comm_sz, MPI_Comm comm);
void lerVetores(double local_a[],int local_n, int n, int my_rank, MPI_Comm comm);
void imprimiVetor( double local_b[],  int local_n,  int n,  int my_rank, MPI_Comm  comm);

int main(){

	int comm_sz;        
    int my_rank;
	double *vetor, *vetorSomas;
	int n, n_local;


	MPI_Init(NULL,NULL);

	//Número de processos dentro do comunicador (tamanho do comm)
   	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz); 

   	 //index de um processo que está dentro do comunicador
   	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); 

   	//Ler a ordem dos vetores
   	ler_n(&n,&n_local,my_rank,comm_sz,MPI_COMM_WORLD);

   	//Aloca os vetores
   	alocaVetores(&vetor,&vetorSomas,n_local,MPI_COMM_WORLD);

   	//ler o vetor
   	lerVetores(vetor,n_local,n,my_rank,MPI_COMM_WORLD);

   	//Soma
   	somaPrefixa(vetor,vetorSomas,n_local);

   	imprimiVetor(vetorSomas, n_local, n,my_rank,MPI_COMM_WORLD);

   	free(vetor);
   	free(vetorSomas);

   	MPI_Finalize();

	

    return 0;
    
}

void ler_n(
      int*      n_p        /* out */, 
      int*      local_n_p  /* out */, 
      int       my_rank    /* in  */, 
      int       comm_sz    /* in  */,
      MPI_Comm  comm       /* in  */) {
   
   if (my_rank == 0) {
      printf("Informe a ordem dos vetores: \n");
      scanf("%d", n_p);
   }
   MPI_Bcast(n_p, 1, MPI_INT, 0, comm);
   *local_n_p = *n_p/comm_sz;
} 

void alocaVetores(
      double** local_x, 
	  double** local_y,
      int local_n,
      MPI_Comm comm) {

   *local_x = malloc(local_n*sizeof(double));
   *local_y = malloc(local_n*sizeof(double));
} 

void lerVetores(
      double    local_a[]   /* out */,
      int       local_n     /* in  */, 
      int       n           /* in  */,
      int       my_rank     /* in  */, 
      MPI_Comm  comm        /* in  */) {

   double* a = NULL;
   int i;

   if (my_rank == 0) {
      a = malloc(n*sizeof(double));
      printf("Entre com o vetor: \n");
      for (i = 0; i < n; i++)
         scanf("%lf", &a[i]);
      MPI_Scatter(a, local_n, MPI_DOUBLE, local_a, local_n, MPI_DOUBLE, 0,
         comm);
      free(a);
   } else {
      MPI_Scatter(a, local_n, MPI_DOUBLE, local_a, local_n, MPI_DOUBLE, 0,
         comm);
   }
} 

void somaPrefixa(
	double vetor[], 
	double vetorSomas[], 
	int local_n){

	int i_local;
	vetorSomas[0] = vetor[0];
	for (i_local = 1; i_local < local_n; i_local++){
		vetorSomas[i_local] = vetor[i_local] + vetorSomas[i_local-1];
	}

}

void imprimiVetor(
      double    local_b[]  /* in */, 
      int       local_n    /* in */, 
      int       n          /* in */, 
      int       my_rank    /* in */,
      MPI_Comm  comm       /* in */) {

   double* b = NULL;
   int i;
   int j;

   if (my_rank == 0) {
      b = malloc(n*sizeof(double));
      MPI_Gather(local_b, local_n, MPI_DOUBLE, b, local_n, MPI_DOUBLE,
            0, comm);
      printf("O resutaldo da soma prefixada e: \n");
      for (i = 0; i < n; i++){
      	 if(i < local_n){
         	printf("%f ", b[i]);
         } else {
         	for(j = i; j < (i*local_n)-(i-local_n); j++){
         		b[j] = b[j]+b[i-1];
         		printf("%f ",b[j]);
         	}
         	i = i + 1;
         }
     }
      printf("\n");
      free(b);
   } else {
      MPI_Gather(local_b, local_n, MPI_DOUBLE, b, local_n, MPI_DOUBLE, 0,
         comm);
   }
}