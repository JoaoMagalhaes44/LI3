#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "math.h"
#include "query_aux.h"
#include "testes_aux.h"


//Função que recebe um array de doubles, que correspondem aos valores do tempo registado para todas as repetições da execução de uma dada tarefa, e retorna o menor valor desse array.
double minimo(double tempos[])
{
    int i = 1;
    double minimo = tempos[0];
    while(i<REPS)
    {
        if(tempos[i]<minimo)
        {
            minimo=tempos[i];
        }
        i++;
    }
    return minimo;
}

//Função que recebe um array de doubles, que correspondem aos valores do tempo registado para todas as repetições da execução de uma dada tarefa, e retorna o maior valor desse array.
double maximo(double tempos[])
{
    int i = 1;
    double maximo = tempos[0];
    while(i<REPS)
    {
        if(tempos[i]>maximo)
        {
            maximo=tempos[i];
        }
        i++;
    }
    return maximo;
}

//Função que recebe um array de doubles, que correspondem aos valores do tempo registado para todas as repetições da execução de uma dada tarefa, e retorna a média dos valores desse array.
double calcula_media(double tempos[])
{
    int i = 0;
    double sum = 0.0;
    while(i<REPS)
    {
        sum += tempos[i];
        i++;
    }
    sum /= REPS;
    return sum;
}

//Função que recebe um array de doubles, que correspondem aos valores do tempo registado para todas as repetições da execução de uma dada tarefa, e a média dos valores desse array, retornando o correspondente desvio padrão.
double calcula_desvio_padrao(double media, double tempos[])
{
    int i = 0;
    double aux = 0;
    while(i<REPS)
    {
        aux += ((tempos[i]-media)*(tempos[i]-media));
        i++;
    }
    aux /= (REPS-1);
    double desvio_padrao = sqrt(aux);
    return desvio_padrao;
}

//Função que dada todas as estatísticas relativas ao tempo de execução de uma dada tarefa, para o modo de testes de performance, imprime essas estatísticas num ficheiro, fazendo distinção entre o load e as queries.
void print_estatisticas_p(int j, double media, double desvio_padrao, double max, double min, int testes, FILE* arq)
{
    if(j==0)
    {
        fprintf(arq, "L   | %.7f |   %.7f   | %.7f | %.7f\n", media, desvio_padrao, min, max);
    }
    else if(j<10)
    {
        fprintf(arq, "%d   | %.7f |   %.7f   | %.7f | %.7f\n", j, media, desvio_padrao, min, max);
    }
    else if(j<100)
    {
        fprintf(arq, "%d  | %.7f |   %.7f   | %.7f | %.7f\n", j, media, desvio_padrao, min, max);
    }
    else
    {
        fprintf(arq, "%d | %.7f |   %.7f   | %.7f | %.7f\n", j, media, desvio_padrao, min, max);
    }
}

//Função que dada todas as estatísticas relativas ao tempo de execução de uma dada tarefa, para o modo de testes funcionais, verifica se a tarefa foi ou não corretamente executada, e imprime essas informações num ficheiro, fazendo distinção entre o load e as queries.
void print_estatisticas_f(int j, double media, double desvio_padrao, double max, double min, int testes, FILE* arq)
{
    int success=1;
    if(j!=0)
    {
        char* filename = write_filename(j);
        char* rfilename = write_rtfilename(j, testes);

        if(compare(filename, rfilename))//função que compara linha a linha os dois ficheiros e retorna 1 se forem iguais, não sei se já existe.
        {
            if(media<10)
            {
                success=1;
            }
            else
            {
                success=-1;
            }
        }
        else
        {
            success=0;
        }
        free(filename);
        free(rfilename);
    }
    if(j==0)
    {
        fprintf(arq, "L   |    %d    | %.7f |   %.7f   | %.7f | %.7f\n", success, media, desvio_padrao, min, max);
    }
    else if(j<10)
    {
        fprintf(arq, "%d   |    %d    | %.7f |   %.7f   | %.7f | %.7f\n", j, success, media, desvio_padrao, min, max);
    }
    else if(j<100)
    {
        fprintf(arq, "%d  |    %d    | %.7f |   %.7f   | %.7f | %.7f\n", j, success, media, desvio_padrao, min, max);
    }
    else
    {
        fprintf(arq, "%d |    %d    | %.7f |   %.7f   | %.7f | %.7f\n", j, success, media, desvio_padrao, min, max);
    }
}

//Função que recebe todos os tempos registados na execução de um teste, e efetua o cálculo das respetivas estatísticas sobre esses dados, imprimindo de seguida esses dados num ficheiro.
void calcula_estatisticas(double** time, int testes, int pf)
{
    char* result = write_tst_result(testes);

    FILE* arq = fopen(result, "wt");

    int aux=0;
    if(testes==1)
    {
        aux=50;
    }
    else if(testes==2)
    {
        aux=500;
    }
    else if(testes==3)
    {
        aux=120;
    }
    else if(testes==4)
    {
        aux=142;
    }
    
    fprintf(arq, "Para o Dataset %d e respetivo input, numa amostra de %d repetições foram registados os seguintes dados:\n", testes, REPS);
    if(pf==1)
    {
        fprintf(arq, " Nº | SUCESSO |   MEDIA   | DESVIO PADRÃO |  MÍNIMO   |  MÁXIMO\n");
    }
    else if(pf==0)
    {
        fprintf(arq, " Nº |   MEDIA   | DESVIO PADRÃO |  MÍNIMO   |  MÁXIMO\n");
    }

    for(int j=0; j<=aux ;j++)
    {
        double aux[REPS];
        for (int i = 0; i < REPS; i++)
        {
        aux[i] = time[i][j];
        }

        double media = calcula_media(aux);
        double desvio_padrao = calcula_desvio_padrao(media, aux);
        double max = maximo(aux);
        double min = minimo(aux);
        if(pf==1)
        {
            print_estatisticas_f(j, media, desvio_padrao, max, min, testes, arq);
        }
        else if(pf==0)
        {
            print_estatisticas_p(j, media, desvio_padrao, max, min, testes, arq);
        }
    }
    fclose(arq);
}