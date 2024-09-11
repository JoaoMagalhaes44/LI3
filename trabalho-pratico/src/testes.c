#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"
#include "ctype.h"
#include "time.h"
#include "query_aux.h"
#include "testes.h"
#include "testes_aux.h"
#include "mode.h"

//Função principal do modo de testes, que executa de forma automática todos os testes desenhados pelo grupo, imprimindo para cada teste as estatísticas de cada tarefa neles executada num ficheiro.
int testing()
{   
    int n = 1;
    int pf;
    double* time[REPS];
    char* argv[4];

    for (int i = 0; i < REPS; i++) time[i] = NULL; // Inicialização de todas a posições de "time"

    while(n<=4)
    {
        write_argv(argv, n);
        if(n<=2)
        {
            pf=1;
        }
        else
        {
            pf=0;
        }
        
        batch_teste(time, argv, n, pf);
        calcula_estatisticas(time, n, pf);
        n++;
    }
    for (int i = 0; i < REPS; i++) free(time[i]);

    return 0;
}