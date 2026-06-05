#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 512

int main()
{
    int *Mat1 = malloc(N*N * sizeof(int)); //Primeira Matriz.
    int *Mat2 = malloc(N*N * sizeof(int)); //Segunda Matriz. 
    int *MatR = malloc(N*N * sizeof(int)); // Resultado.

    if (!Mat1 || !Mat2 || !MatR)
        printf("Erro ao alocar\n");

    //Inicializacao das matrizes.
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            Mat1[i*N+j] = 1; //Matrizes de 1s.
            Mat2[i*N+j] = 1; 
            MatR[i*N+j] = 0;
        }
    }

    clock_t inicio = clock();

    //Mult das matrizes.(ijk)
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            for (int k = 0; k <  N; k++) //Percorre elementos internos.
            {
                MatR[i*N+j] += Mat1[i*N+k] * Mat2[k*N+j];
            }
         }
    }

    //Mat1[i*N+k] acessa a memoria sequencialmente.
    //Mat2[i*N+k] pula a memoria continuamente, prejudica a cache.

    clock_t fim = clock();

    double tempo = (double)(fim-inicio) /CLOCKS_PER_SEC;
    printf("O tempo eh: %f segundos\n", tempo);

    printf("Primeiro elemento = %d\n", MatR[0]);

    free(Mat1);
    free(Mat2);
    free(MatR);

    return (0);
}
