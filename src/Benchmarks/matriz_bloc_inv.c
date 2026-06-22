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

    int N = atoi(teste_v[1]); // Converte argumento para inteiro.

    printf("Tamanho da matriz: %d X %d\n", N, N);
    printf("Tamanho do bloco: %d\n", BLOCO);

    int *Mat1 = malloc(N * N * sizeof(int)); // Primeira Matriz.
    int *Mat2 = malloc(N * N * sizeof(int)); // Segunda Matriz.
    int *MatR = malloc(N * N * sizeof(int)); // Resultado.

    if (!Mat1 || !Mat2 || !MatR)
    {
        printf("Erro ao alocar\n");
        free(Mat1);
        free(Mat2);
        free(MatR);

        return (1);
    }

    // Inicializacao das matrizes.
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            Mat1[i * N + j] = 2;
            Mat2[i * N + j] = 7;
            MatR[i * N + j] = 0;
        }
    }

    clock_t inicio = clock();

    // Inverte jj por kk
    for (int ii = 0; ii < N; ii += BLOCO)
    {
        for (int kk = 0; kk < N; kk += BLOCO)
        {
            for (int jj = 0; jj < N; jj += BLOCO)
            {
                // Evita ultrapassar o tamanho da matriz
                int i_max;
                if (ii + BLOCO < N)
                    i_max = ii + BLOCO;
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

                // Loops - removemos loop jj:
                for (int i = ii; i < i_max; i++)
                {
                    // Variáveis para cada elemento da linha de um bloco
                    // Poderia ser um vetor também
                    int s0 = 0, s1 = 0, s2 = 0, s3 = 0, s4 = 0, s5 = 0, s6 = 0, s7 = 0;
                    int s8 = 0, s9 = 0, s10 = 0, s11 = 0, s12 = 0, s13 = 0, s14 = 0, s15 = 0;
                    int s16 = 0, s17 = 0, s18 = 0, s19 = 0, s20 = 0, s21 = 0, s22 = 0, s23 = 0;
                    int s24 = 0, s25 = 0, s26 = 0, s27 = 0, s28 = 0, s29 = 0, s30 = 0, s31 = 0;

                    for (int k = kk; k < k_max; k++)
                    {
                        int temp = Mat1[i * N + k]; // Evita acessar desnecessáriamente o valor em memória

                        // Fazemos as contas somando nas variáveis de rascunho
                        // Garantimos que jj + n seja menor que j_max evitando ultrapassar as bordas
                        if (jj + 0 < j_max)
                            s0 += temp * Mat2[k * N + jj + 0];
                        if (jj + 1 < j_max)
                            s1 += temp * Mat2[k * N + jj + 1];
                        if (jj + 2 < j_max)
                            s2 += temp * Mat2[k * N + jj + 2];
                        if (jj + 3 < j_max)
                            s3 += temp * Mat2[k * N + jj + 3];
                        if (jj + 4 < j_max)
                            s4 += temp * Mat2[k * N + jj + 4];
                        if (jj + 5 < j_max)
                            s5 += temp * Mat2[k * N + jj + 5];
                        if (jj + 6 < j_max)
                            s6 += temp * Mat2[k * N + jj + 6];
                        if (jj + 7 < j_max)
                            s7 += temp * Mat2[k * N + jj + 7];
                        if (jj + 8 < j_max)
                            s8 += temp * Mat2[k * N + jj + 8];
                        if (jj + 9 < j_max)
                            s9 += temp * Mat2[k * N + jj + 9];
                        if (jj + 10 < j_max)
                            s10 += temp * Mat2[k * N + jj + 10];
                        if (jj + 11 < j_max)
                            s11 += temp * Mat2[k * N + jj + 11];
                        if (jj + 12 < j_max)
                            s12 += temp * Mat2[k * N + jj + 12];
                        if (jj + 13 < j_max)
                            s13 += temp * Mat2[k * N + jj + 13];
                        if (jj + 14 < j_max)
                            s14 += temp * Mat2[k * N + jj + 14];
                        if (jj + 15 < j_max)
                            s15 += temp * Mat2[k * N + jj + 15];
                        if (jj + 16 < j_max)
                            s16 += temp * Mat2[k * N + jj + 16];
                        if (jj + 17 < j_max)
                            s17 += temp * Mat2[k * N + jj + 17];
                        if (jj + 18 < j_max)
                            s18 += temp * Mat2[k * N + jj + 18];
                        if (jj + 19 < j_max)
                            s19 += temp * Mat2[k * N + jj + 19];
                        if (jj + 20 < j_max)
                            s20 += temp * Mat2[k * N + jj + 20];
                        if (jj + 21 < j_max)
                            s21 += temp * Mat2[k * N + jj + 21];
                        if (jj + 22 < j_max)
                            s22 += temp * Mat2[k * N + jj + 22];
                        if (jj + 23 < j_max)
                            s23 += temp * Mat2[k * N + jj + 23];
                        if (jj + 24 < j_max)
                            s24 += temp * Mat2[k * N + jj + 24];
                        if (jj + 25 < j_max)
                            s25 += temp * Mat2[k * N + jj + 25];
                        if (jj + 26 < j_max)
                            s26 += temp * Mat2[k * N + jj + 26];
                        if (jj + 27 < j_max)
                            s27 += temp * Mat2[k * N + jj + 27];
                        if (jj + 28 < j_max)
                            s28 += temp * Mat2[k * N + jj + 28];
                        if (jj + 29 < j_max)
                            s29 += temp * Mat2[k * N + jj + 29];
                        if (jj + 30 < j_max)
                            s30 += temp * Mat2[k * N + jj + 30];
                        if (jj + 31 < j_max)
                            s31 += temp * Mat2[k * N + jj + 31];
                    }

                    // Escrevemos os resultados finais na MatR só uma vez por bloco
                    if (jj + 0 < j_max)
                        MatR[i * N + jj + 0] += s0;
                    if (jj + 1 < j_max)
                        MatR[i * N + jj + 1] += s1;
                    if (jj + 2 < j_max)
                        MatR[i * N + jj + 2] += s2;
                    if (jj + 3 < j_max)
                        MatR[i * N + jj + 3] += s3;
                    if (jj + 4 < j_max)
                        MatR[i * N + jj + 4] += s4;
                    if (jj + 5 < j_max)
                        MatR[i * N + jj + 5] += s5;
                    if (jj + 6 < j_max)
                        MatR[i * N + jj + 6] += s6;
                    if (jj + 7 < j_max)
                        MatR[i * N + jj + 7] += s7;
                    if (jj + 8 < j_max)
                        MatR[i * N + jj + 8] += s8;
                    if (jj + 9 < j_max)
                        MatR[i * N + jj + 9] += s9;
                    if (jj + 10 < j_max)
                        MatR[i * N + jj + 10] += s10;
                    if (jj + 11 < j_max)
                        MatR[i * N + jj + 11] += s11;
                    if (jj + 12 < j_max)
                        MatR[i * N + jj + 12] += s12;
                    if (jj + 13 < j_max)
                        MatR[i * N + jj + 13] += s13;
                    if (jj + 14 < j_max)
                        MatR[i * N + jj + 14] += s14;
                    if (jj + 15 < j_max)
                        MatR[i * N + jj + 15] += s15;
                    if (jj + 16 < j_max)
                        MatR[i * N + jj + 16] += s16;
                    if (jj + 17 < j_max)
                        MatR[i * N + jj + 17] += s17;
                    if (jj + 18 < j_max)
                        MatR[i * N + jj + 18] += s18;
                    if (jj + 19 < j_max)
                        MatR[i * N + jj + 19] += s19;
                    if (jj + 20 < j_max)
                        MatR[i * N + jj + 20] += s20;
                    if (jj + 21 < j_max)
                        MatR[i * N + jj + 21] += s21;
                    if (jj + 22 < j_max)
                        MatR[i * N + jj + 22] += s22;
                    if (jj + 23 < j_max)
                        MatR[i * N + jj + 23] += s23;
                    if (jj + 24 < j_max)
                        MatR[i * N + jj + 24] += s24;
                    if (jj + 25 < j_max)
                        MatR[i * N + jj + 25] += s25;
                    if (jj + 26 < j_max)
                        MatR[i * N + jj + 26] += s26;
                    if (jj + 27 < j_max)
                        MatR[i * N + jj + 27] += s27;
                    if (jj + 28 < j_max)
                        MatR[i * N + jj + 28] += s28;
                    if (jj + 29 < j_max)
                        MatR[i * N + jj + 29] += s29;
                    if (jj + 30 < j_max)
                        MatR[i * N + jj + 30] += s30;
                    if (jj + 31 < j_max)
                        MatR[i * N + jj + 31] += s31;
                }
            }
        }
    }

    clock_t fim = clock();

    double tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;
    printf("O tempo eh: %.6f segundos\n", tempo);

    printf("Primeiro elemento = %d\n", MatR[0]);

    free(Mat1);
    free(Mat2);
    free(MatR);

    return (0);
}
