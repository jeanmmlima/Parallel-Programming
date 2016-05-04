#include <stdio.h>
#include <string.h> 
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

int verificaPosicao(double x, double y, int r){
	
	double d = sqrt(x*x + y*y);
	if (d <= r){
		return 1;
	}
	else{
		return 0;
	}
}


int main(){

	int comm_sz;               /* Number of processes    */
    int my_rank;               /* My process rank        */
	
	int r = 4, resto, my_first_i, my_last_i, precisao = 100000;
	long int nAmostras = 10000000, nAmostras_local;
	double x_local, y_local, h_local = 0, h_rec = 0;
	double pi = 0,local_start, local_finish, local_elapsed, elapsed;
	

	MPI_Init(NULL,NULL);
	MPI_Barrier(MPI_COMM_WORLD);
	local_start = MPI_Wtime();

	//Número de processos dentro do comunicador (tamanho do comm)
   	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz); 

   	 //index de um processo que está no comunicador
   	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); 
	
 	resto = nAmostras%comm_sz;
	nAmostras_local = nAmostras/comm_sz;
	

	//mesmo que o numero de amostras nao seja divisivel por comm_sz, funciona
	if(my_rank < resto){
		my_first_i = my_rank * (nAmostras_local + 1);
		my_last_i = (nAmostras_local + 1) * my_rank + nAmostras_local;	
	}
	else{
		my_first_i = my_rank * nAmostras_local + resto;
		my_last_i = nAmostras_local * my_rank + resto + (nAmostras_local - 1);
	}

	unsigned int seed = time(NULL);
	for(int i = my_first_i; i < my_last_i; i++){
		x_local = ((double)(rand_r(&seed) % (2*r*precisao+1))) - r*precisao;
		y_local = ((double)(rand_r(&seed) % (2*r*precisao+1))) - r*precisao;
		if(verificaPosicao(x_local/precisao,y_local/precisao,r) == 1){
			h_local++;
		}
	}

	int core_difference = 1;
    int divisor = 2;
	int sender;
	int receiver;

	while(divisor <= comm_sz){
		if(my_rank%divisor == 0){
			sender = my_rank + core_difference;
			if(sender < comm_sz){
				MPI_Recv(&h_rec, 1, MPI_DOUBLE, sender, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				h_local += h_rec;
			}		
		}
		else{
			receiver = my_rank - core_difference;
			MPI_Send(&h_local, 1, MPI_DOUBLE, receiver, 0, MPI_COMM_WORLD);
			break;
		}
		divisor *= 2;
		core_difference *= 2;
	}

	local_finish = MPI_Wtime();
	local_elapsed = local_finish - local_start;
	MPI_Reduce(&local_elapsed,&elapsed,1,MPI_DOUBLE,MPI_MAX,0,MPI_COMM_WORLD);
	
	if(my_rank == 0){
		pi = 4*h_local/nAmostras;
		printf("Pi estimado e: %f\n", pi);
		printf("O tempo total foi de: %e segundos\n", elapsed);
	}

	MPI_Finalize();

	return 0;

}