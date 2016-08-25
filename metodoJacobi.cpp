//
//  metodoJacobi.cpp
//  
//
//  Created by Jean Mario Moreira de Lima on 2016-05-22.
//
//

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

void geraVetorErros(double xAtual[], double xAnterior[], double xTemporario[], int n);
void atribuiVetor(double vSaida[], double vEntrada[], int n);
double maxAbsolutoVetor(double vetor[], int n);
void printVetor(double vetor[],int n);
void printMatriz(double matriz[][3],int n);

int main(){
    
    double A[3][3] = {{8,1,-1},{1,-7,2},{2,1,9}};
    double b[3] = {8,-4,12};
    
    double x[3] = {0,0,0};
    double x_prev[3] = {0,0,0};
    double x_temp[3] = {0,0,0};
    
    int ordem = sizeof(b)/sizeof(b[0]);
    
    //Variaveis dos For's
    double soma,erro = 0;
    
    //Parametros
    int numIteracoes = 10;
    double precisao = 0.0001;
    
    
    printVetor(x,ordem);
    printMatriz(A,ordem);
    for(int k = 0; k < numIteracoes; k++){
        for(int i = 0; i < ordem; i++){
            soma = 0;
            for(int j = 0; j < ordem; j++){
                if(j != i){
                    soma = soma + A[i][j]*x_prev[j];

                }
            }
            printf("Soma: %f\n",soma);
            x[i] = (b[i] - soma)/(A[i][i]);
        }
        printf("x da iteração: %f %f %f \n",x[0],x[1],x[2]);
        geraVetorErros(x,x_prev,x_temp,ordem);
        atribuiVetor(x_prev,x,ordem);
        erro = maxAbsolutoVetor(x_temp,ordem);
        printf("Tolerancia: %f\n",erro);
        if(erro < precisao){
            printf("Precisao requerida foi atingida!\n");
            k = numIteracoes;
            break;
        }
    }
    
    
    
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

void printMatriz(double matriz[][3],int n){
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            printf("%f ",matriz[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}



