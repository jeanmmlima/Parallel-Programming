#include <stdio.h>
#include <string.h> 
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

int verificaPosicao(double x, double y, int r);
void ler_n(int* n_p, int* local_n_p, int my_rank, int comm_sz, MPI_Comm comm);
void alocaVetores(double** local_x, double** local_y, double** local_c, int local_n, MPI_Comm comm);
void lerVetores(double local_a[],double local_b[],int local_n, int n, int my_rank, MPI_Comm comm);
void lerEscalar(int* escalar, int my_rank, MPI_Comm comm);
void produtoEscalar(double  local_x[], double local_z[],int escalar, int local_n);
void produtoVetorial(double  local_x[], double  local_y[],double* produtoVet_local, int local_n);
void imprimiVetor( double    local_b[],  int local_n,  int n,  int my_rank, MPI_Comm  comm);



int main(){

	int comm_sz;               /* Number of processes    */
    int my_rank;               /* My process rank        */
    int n, n_local, escalar;
    double *local_x, *local_y, *local_z, produtoVet_local, produtoVetorialTotal;
	

	MPI_Init(NULL,NULL);

	//Número de processos dentro do comunicador (tamanho do comm)
   	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz); 

   	 //index de um processo que está dentro do comunicador
   	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); 

   	//Ler a ordem dos vetores
   	ler_n(&n,&n_local,my_rank,comm_sz,MPI_COMM_WORLD);
   	//Aloca os vetores locais de cada processo
   	alocaVetores(&local_x,&local_y,&local_z,n_local,MPI_COMM_WORLD);
   	//Ler Vetores
   	lerVetores(local_x,local_y,n_local,n,my_rank,MPI_COMM_WORLD);
   	//Ler escalar
   	lerEscalar(&escalar, my_rank, MPI_COMM_WORLD);
      //Realizar produto escalar
      produtoEscalar(local_x,local_z,escalar,n_local);
      //Realiza o produto vetorial
      produtoVetorial(local_x,local_y,&produtoVet_local,n_local);

      //Imprimi o resultado do produto escalar:
      imprimiVetor(local_z, n_local,n,my_rank,MPI_COMM_WORLD);

      MPI_Reduce(&produtoVet_local,&produtoVetorialTotal,1,MPI_DOUBLE,MPI_SUM,0,MPI_COMM_WORLD);

      if(my_rank == 0){
         printf("O resultado do produto vetorial e: %f\n", produtoVetorialTotal);
      }

      free(local_x);
      free(local_y);
      free(local_z);

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
      double**   local_x  /* out */, 
      double**   local_y  /* out */,
      double**   local_z /* out */, 
      int        local_n     /* in  */,
      MPI_Comm   comm        /* in  */) {

   *local_x = malloc(local_n*sizeof(double));
   *local_y = malloc(local_n*sizeof(double));
   *local_z = malloc(local_n*sizeof(double));
} 

void lerVetores(
      double    local_a[]   /* out */,
      double 	local_b[] /* out */ ,
      int       local_n     /* in  */, 
      int       n           /* in  */,
      int       my_rank     /* in  */, 
      MPI_Comm  comm        /* in  */) {

   double* a = NULL;
   double* b = NULL;
   int i;

   if (my_rank == 0) {
      a = malloc(n*sizeof(double));
      printf("Entre com o vetor A: \n");
      for (i = 0; i < n; i++)
         scanf("%lf", &a[i]);
      MPI_Scatter(a, local_n, MPI_DOUBLE, local_a, local_n, MPI_DOUBLE, 0,
         comm);
      free(a);
   } else {
      MPI_Scatter(a, local_n, MPI_DOUBLE, local_a, local_n, MPI_DOUBLE, 0,
         comm);
   }

   if (my_rank == 0) {
      b = malloc(n*sizeof(double));
      printf("Entre com o vetor B: \n");
      for (i = 0; i < n; i++)
         scanf("%lf", &b[i]);
      MPI_Scatter(b, local_n, MPI_DOUBLE, local_b, local_n, MPI_DOUBLE, 0,
         comm);
      free(b);
   } else {
      MPI_Scatter(b, local_n, MPI_DOUBLE, local_b, local_n, MPI_DOUBLE, 0,
         comm);
   }
} 

void lerEscalar(int* escalar, int my_rank, MPI_Comm comm){

	if (my_rank == 0){
		printf("Entre com o escalar: \n");
		scanf("%i", escalar);
	}
	MPI_Bcast(escalar, 1, MPI_INT, 0, comm);

}

void produtoVetorial(
      double  local_x[]  /* in  */, 
      double  local_y[]  /* in  */,
      double* produtoVet_local, 
      int     local_n    /* in  */) {
   int local_i;
   *produtoVet_local = 0;
   for (local_i = 0; local_i < local_n; local_i++)
      *produtoVet_local = (local_x[local_i] * local_y[local_i]) + *produtoVet_local;
} 

void produtoEscalar(
      double  local_x[]  /* in  */, 
      double local_z[],
      int escalar, 
      int     local_n    /* in  */) {
   int local_i;
   for (local_i = 0; local_i < local_n; local_i++)
      local_z[local_i] = local_x[local_i] * escalar;
} 

void imprimiVetor(
      double    local_b[]  /* in */, 
      int       local_n    /* in */, 
      int       n          /* in */, 
      int       my_rank    /* in */,
      MPI_Comm  comm       /* in */) {

   double* b = NULL;
   int i;

   if (my_rank == 0) {
      b = malloc(n*sizeof(double));
      MPI_Gather(local_b, local_n, MPI_DOUBLE, b, local_n, MPI_DOUBLE,
            0, comm);
      printf("O resutaldo do produto escalar e: \n");
      for (i = 0; i < n; i++)
         printf("%f ", b[i]);
      printf("\n");
      free(b);
   } else {
      MPI_Gather(local_b, local_n, MPI_DOUBLE, b, local_n, MPI_DOUBLE, 0,
         comm);
   }
}