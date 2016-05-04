#Estama Pi

Esse diretório contém 4 códigos para estimar o valor de Pi utilizando o método de monte carlo. O primeiro deles, a forma serial convencional.
Os outros três, estimam o valor de pi utilizando a API MPI (Message-Passing Interface) habilitando o processamento
paralelo do código. 

This directory keeps 4 code files to estimate Pi's value using the Monte Carlo method. The first of them, it is the serial way. The others
three estimate Pi's value by using the API MPI (Message-Passing Interface) allowing a parallel processment of the code.

#Compilation

To compilate the codes which use the API MPI:

mpicc -g -Wall -o -std=c99 nameexec namefile.c -lm

#Running

mpiexec -n <numberOfProcessYouWant> ./nameexec

or

mpirun -n <numberOfProcessYouWant> ./nameexec
