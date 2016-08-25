//
//  metodoJacobi.cpp
//  
//
//  Created by Jean Mario Moreira de Lima on 2016-05-22.
// gcc -g -Wall -fopenmp -o metodoJacobi metodoJacobi.c
//

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <omp.h>

void geraVetorErros(double xAtual[], double xAnterior[], double xTemporario[], int n);
void atribuiVetor(double vSaida[], double vEntrada[], int n);
double maxAbsolutoVetor(double vetor[], int n);
void printVetor(double vetor[],int n);
void printMatrix( char* title, double A[], int n);
void geraMatrix(double A[], int n);
double somaLinhaMatriz(double A[], int linha, int n);
void geraVetor(double A[], double b[], int n);

void Get_args(int argc, char* argv[], int* thread_count_p, int* n_p);
void Usage(char* prog_name);

void Omp_mat_vect(double A[], double xAtual[], double xAnterior[], double b[], int n, int thread_count);


int main(int argc, char* argv[]){

    int thread_count;
    int n, numIteracoes = 1, precisao = 0.0001;
    double erro = 0;
    double* A;
    double* x;
    double* b;
    double* x_prev;
    double* x_temp;

    Get_args(argc, argv, &thread_count, &n);

    A = malloc(n*n*sizeof(double));
    x = malloc(n*sizeof(double));
    b = malloc(n*sizeof(double));
    x_prev = malloc(n*sizeof(double));
    x_temp = malloc(n*sizeof(double));
    
    geraMatrix(A,n);
    geraVetor(A,b,n);

    for(int l = 0; l < n; l++){
        x[l] = 0;
        x_prev[l] = 1;
        x_temp[l] = 0;
    }

    for(int k = 0; k < numIteracoes; k++){
        Omp_mat_vect(A, x, x_prev, b, n, thread_count); 
        geraVetorErros(x,x_prev,x_temp,n);
        atribuiVetor(x_prev,x,n);
        erro = maxAbsolutoVetor(x_temp,n);
        if(erro < precisao){
            k = numIteracoes;
            break;
        }
    }
    
    
    free(A);
    free(b);
    free(x);
    free(x_prev);
    free(x_temp);
    
    return 0;
}

void geraVetorErros(double xAtual[], double xAnterior[], double xTemporario[], int n){
    for(int i = 0; i <  n; i++){
        xTemporario[i] = xAtual[i] -  xAnterior[i];
    }
}
void atribuiVetor(double vSaida[], double vEntrada[], int n){
    for(int i = 0; i <  n; i++){
        vSaida[i] = vEntrada[i];
    }
}

double maxAbsolutoVetor(double vetor[], int n){
    double max = vetor[0];
    for(int i = 0; i < n; i++){
        if(vetor[i] < 0){
            vetor[i] = vetor[i]*(-1);
        }
        if(vetor[i] > max){
            max = vetor[i];
        }
    }
    return max;
}

void printVetor(double vetor[],int n){
    for(int i = 0; i < n; i++){
        printf("%f ",vetor[i]);
    }
    printf("\n");
}

void printMatrix( char* title, double A[], int n) {
   int   i, j;

   printf("%s\n", title);
   for (i = 0; i < n; i++) {
      for (j = 0; j < n; j++)
         printf("%4.1f ", A[i*n + j]);
      printf("\n");
   }
} 

void Get_args(int argc, char* argv[], int* thread_count_p, int* n_p)  {

   if (argc != 3) Usage(argv[0]);
   *thread_count_p = strtol(argv[1], NULL, 10);
   *n_p = strtol(argv[2], NULL, 10);
   if (*thread_count_p <= 0 || *n_p <= 0) Usage(argv[0]);

} 

void Usage (char* prog_name) {
   fprintf(stderr, "usage: %s <thread_count> <m> <n>\n", prog_name);
   exit(0);
}

void geraMatrix(double A[], int n) {
   int i, j;
   for (i = 0; i < n; i++)
      for (j = 0; j < n; j++)
         A[i*n+j] = (random()*100)/((double) RAND_MAX);
}

double somaLinhaMatriz(double A[], int linha, int n){
    double soma = 0;
    for(int j = 0; j < n; j++){
        soma += A[linha*n+j];
    }
    return soma;
}

void geraVetor(double A[], double b[], int n){
    for(int i = 0; i < n; i++){
        b[i] = somaLinhaMatriz(A,i,n);
    }
}

void Omp_mat_vect(double A[], double xAtual[], double xAnterior[], double b[], int n, int thread_count) {
   int i, j;
   double start, finish, elapsed,soma;

   start = omp_get_wtime();
#  pragma omp parallel for num_threads(thread_count)  \
      default(none) private(i, j, soma)  shared(A, xAtual, xAnterior, n, b)
   for(i = 0; i < n; i++){
            soma = 0;
            for(j = 0; j < n; j++){
                if(j != i){
                    soma += A[i*n+j]*xAnterior[j];

                }
            }
            xAtual[i] = (b[i] - soma)/(A[i*n+i]);
        }
   finish = omp_get_wtime();
   elapsed = finish - start;
   printf("Elapsed time = %e seconds\n", elapsed);

}



