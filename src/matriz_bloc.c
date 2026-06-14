#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BLOCO 32

int main(int teste_arg, char *teste_v[])
{
    if (teste_arg != 2)
    {
        printf("Escolha outro tamanho\n");
        return (1);
    }

    int N = atoi(teste_v[1]); //Converte argumento para inteiro.

    printf("Tamanho da matriz: %d X %d\n", N, N);

    printf("Tamanho do bloco: %d\n", BLOCO);

    int *Mat1 = malloc(N*N * sizeof(int)); //Primeira Matriz.
    int *Mat2 = malloc(N*N * sizeof(int)); //Segunda Matriz. 
    int *MatR = malloc(N*N * sizeof(int)); // Resultado.

    if (!Mat1 || !Mat2 || !MatR)
    {
        printf("Erro ao alocar\n");
        free(Mat1);
        free(Mat2);
        free(MatR);

        return(1);
    }
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

    //ii, jj e kk selecinam qual bloco sera processado
    for (int ii = 0; ii < N; ii += BLOCO)
    {
        for (int jj = 0; jj < N; jj += BLOCO)
        {
            for (int kk = 0; kk <  N; kk += BLOCO)
            {
               /*Evita ultrapassar o tamanho da matriz.*/
               int i_max; 
    
                if (ii + BLOCO < N)
                    i_max = ii+ BLOCO;
                else
                    i_max = N;
             
                int j_max;

                if (jj + BLOCO < N)
                    j_max = jj + BLOCO;
                else
                    j_max = N;

                int k_max;

                if (kk + BLOCO < N)
                    k_max = kk + BLOCO;
                else
                    k_max = N;
                 

               for (int i = ii ; i < i_max; i++) //Percorre elementos do bloco atual.
               {
                   for (int j = jj; j < j_max; j++)
                   {
                       int soma = MatR[i*N+j];

                       for (int k = kk; k < k_max; k++)
                       {
                           soma += Mat1[i*N+k] * Mat2[k*N+j];
                       }

                       MatR[i*N+j] = soma;
                   }
               }
            }
         }
    }

    //Mat1[i*N+k] acessa a memoria sequencialmente.
    //Mat2[i*N+k] pula a memoria continuamente, prejudica a cache.

    clock_t fim = clock();

    double tempo = (double)(fim-inicio) /CLOCKS_PER_SEC;
    printf("O tempo eh: %.6f segundos\n", tempo);

    printf("Primeiro elemento = %d\n", MatR[0]);

    free(Mat1);
    free(Mat2);
    free(MatR);

    return (0);
}
