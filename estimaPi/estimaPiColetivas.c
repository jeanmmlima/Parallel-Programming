#include <stdio.h>
#include <string.h> 
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

void Get_input(int my_rank, int comm_sz, long int* nAmostras);
int verificaPosicao(double x, double y, int r);


int main(){

	int comm_sz;               /* Number of processes    */
    int my_rank;               /* My process rank        */
	
	int r = 4, resto, my_first_i, my_last_i, precisao = 1000000;
	long int nAmostras, nAmostras_local;
	double h = 0, x_local, y_local, h_local = 0;
	double pi = 0, local_start, local_finish, local_elapsed, elapsed;
	

	MPI_Init(NULL,NULL);
	MPI_Barrier(MPI_COMM_WORLD);
	local_start = MPI_Wtime();

	//Número de processos dentro do comunicador (tamanho do comm)
   	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz); 

   	 //index de um processo que está dentro do comunicador
   	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); 

   	Get_input(my_rank,comm_sz,&nAmostras);
	
 	resto = nAmostras%comm_sz;
	nAmostras_local = nAmostras/comm_sz;
	
	if(my_rank < resto){
		my_first_i = my_rank * (nAmostras_local + 1);
		my_last_i = (nAmostras_local + 1) * my_rank + nAmostras_local;	
	}
	else{
		my_first_i = my_rank * nAmostras_local + resto;
		my_last_i = nAmostras_local * my_rank + resto + (nAmostras_local - 1);
	}

	//srand((unsigned)time(NULL));
	unsigned int seed = time(NULL);
	for(int i = my_first_i; i < my_last_i; i++){
		x_local = ((double)(rand_r(&seed) % (2*r*precisao+1))) - r*precisao;
		y_local = ((double)(rand_r(&seed) % (2*r*precisao+1))) - r*precisao;
		if(verificaPosicao(x_local/precisao,y_local/precisao,r) == 1){
			h_local++;
		}
	}

	MPI_Reduce(&h_local,&h,1,MPI_DOUBLE,MPI_SUM,0,MPI_COMM_WORLD);
	local_finish = MPI_Wtime();
	local_elapsed = local_finish - local_start;
	MPI_Reduce(&local_elapsed,&elapsed,1,MPI_DOUBLE,MPI_MAX,0,MPI_COMM_WORLD);

	if(my_rank == 0){
		pi = 4*h/nAmostras;
		printf("Pi estimado e: %f\n", pi);
		printf("O tempo total foi de: %e segundos\n", elapsed);
	}

	MPI_Finalize();

	return 0;

}

void Get_input(int my_rank, int comm_sz, long int* nAmostras) {

   nAmostras = 100000000; 
   MPI_Bcast(nAmostras, 1, MPI_LONG, 0, MPI_COMM_WORLD);
}  /* Get_input */

int verificaPosicao(double x, double y, int r){
	
	double d = sqrt(x*x + y*y);
	if (d <= r){
		return 1;
	}
	else{
		return 0;
	}
}
