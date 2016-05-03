#include <stdio.h>
#include <string.h> 
#include <math.h>
#include <stdlib.h>
#include <time.h>

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
	
	int r = 4, nAmostras = 100000000, precisao = 10000;
	double x, y, h = 0;
	double pi = 0;
	unsigned int seed = time(NULL);
	time_t t1, t2;
	
	time(&t1);

	for (int i = 0; i < nAmostras; i++){
		x = ((double)(rand_r(&seed) % (2*r*precisao+1))) - r*precisao;
		y = ((double)(rand_r(&seed) % (2*r*precisao+1))) - r*precisao;
		if(verificaPosicao(x/precisao,y/precisao,r) == 1){
			h++;
		}
	}

	time(&t2);
	
	pi = 4*h/nAmostras;
	double tempo_gasto = difftime(t2,t1);
	printf("Valor estimado de pi eh: %f\n", pi);
	printf("Tempo gasto: %.f\n",tempo_gasto);

	return 0;

}     
