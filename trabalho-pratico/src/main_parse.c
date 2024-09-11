#include "stdlib.h"
#include "stdio.h"
#include "time.h"
#include "users.h"
#include "drivers.h"
#include "rides.h"
#include "main_parse.h"
#include "query_aux.h"
#include "testes_aux.h"
#include "query1.h"
#include "query2.h"
#include "query3.h"
#include "query4.h"
#include "query5.h"
#include "query6.h"
#include "query7.h"
#include "query8.h"
#include "query9.h"

//Estrutura que trata os comandos a realizar.
struct args
{
    int query;
    char* arg1;
    char* arg2;
    char* arg3;
};

//Retorna o primeiro argumento do comando.
char* get_arg1(struct args* args)
{
    char* arg1 = strdup(args->arg1);
    return arg1;
}

//Retorna a query pedida pelo comando.
int get_query(struct args* args)
{
    int query = args->query;
    return query;
}

//Inicializa uma struct args.
struct args* init_args(char* line_m)
{
    struct args* args = malloc (sizeof(struct args));
    char* line = strdup(line_m);
    char* linef = line;
    char* ptr;
    int index=0;

    while((ptr=strsep(&line, " \n")) != NULL) 
    {
        switch (index)
        {
            case 0:
            { 
                    args->query = atoi(ptr);
                    break;
            }
            case 1: 
            {
                    char* arg1_ = strdup(ptr); 
                    args->arg1 = arg1_; 
                    break;
            }
            case 2: 
            { 
                    char* arg2_ = strdup(ptr); 
                    args->arg2 = arg2_; 
                    break;
            }
            case 3: 
            {
                    char* arg3_ = strdup(ptr);
                    args->arg3 = arg3_; 
                    break;
            }       
        }
        index++;
    }

    free(linef);

    return args;
}

//Face a um args, redireciona o programa para a função principal que executa a query em questão.
int dist_query(struct args* args, struct drivers* drivers, struct users* users, struct rides* rides, int x, int max, int page_size, int interativo)
{
    int query = get_query(args);
    int posiI = 0;

    switch (query)
    {
        case 1:
        { 
            query1(args->arg1, drivers, users,x);
            break;
        }
        case 2:
        {
            posiI = query2(args->arg1, drivers, x, max, page_size, interativo);
            break;            
        }
        case 3:
        {
            posiI = query3(args->arg1, users, x, max, page_size, interativo);
            break;
        }
        case 4:
        {
            query4(args->arg1, rides, x);
            break;
        }
        case 5:
        {
            query5(args->arg1, args->arg2, rides, x);
            if (x != 0)
            {
                free(args->arg3);
            }
            break;
        }
        case 6:
        {
            query6(args->arg1, args->arg2, args->arg3, rides, x);
            if (x != 0)
            {
                free(args->arg3);
            }
            break;
        }
        case 7:
        {
            posiI = query7(args->arg1, args->arg2, drivers, rides, x, max, page_size, interativo);
            if (x != 0)
            {
                free(args->arg3);
            }
            break;
        }
        case 8:
        {
            posiI = query8(args->arg1, args->arg2, drivers, users, rides, x, max, page_size, interativo);
            if (x != 0)
            {
                free(args->arg3);
            }
            break;
        }
        case 9:
        {
            posiI = query9(args->arg1, args->arg2, rides, x, max, page_size, interativo);
            if (x != 0)
            {
                free(args->arg3);
            }
            break;
        }
    }
    return posiI;
}

//Lê o ficheiro com os comandos, linha a linha, e realiza a query em questão.
void main_parse(char* input_path, struct drivers* drivers, struct users* users, struct rides* rides)
{
    FILE* fptrI = fopen (input_path,"r");
    int x = 1;

    if (fptrI == NULL) {
        printf("Couldn't open the file!\n");
        exit(1);
    }

    char* line=NULL;
    ssize_t read;
    size_t len;

    while ((read = getline(&line, &len, fptrI)) != -1)
    {   
        struct args* args = init_args(line);
        dist_query(args, drivers, users, rides, x, 0, 0, 0);
        x++;

        free(args->arg1);
        free(args->arg2);
        free(args);
    }
    free(line);
}

//Lê o ficheiro com os comandos, linha a linha, e realiza a query em questão, registando o tempo de execução de cada uma.
double* main_parse_teste(char* input_path, struct drivers* drivers, struct users* users, struct rides* rides, float load_time, int testes, int pf)
{
    int aux=0;
    if(testes==1)
    {
        aux=52;
    }
    else if(testes==2)
    {
        aux=502;
    }
    else if(testes==3)
    {
        aux=122;
    }
    else if(testes==4)
    {
        aux=144;
    }
    double* t = calloc(aux,sizeof(double)); 
    t[0] = load_time;
    FILE* fptrI = fopen (input_path,"r");
    int x = 1;

    if (fptrI == NULL) {
        printf("Couldn't open the file!\n");
        exit(1);
    }

    char* line=NULL;
    ssize_t read;
    size_t len;

    while ((read = getline(&line, &len, fptrI)) != -1)
    {   
        struct args* args = init_args(line);
        float time_passed;
        clock_t tStart = clock();
        dist_query(args, drivers, users, rides, x, 0, 0, 0);
        time_passed = (double)(clock() - tStart)/CLOCKS_PER_SEC;
        t [x] = time_passed;
        if(pf==1)
        {
            char* filename = write_filename(x);
            char* rfilename = write_rtfilename(x, testes);
            if(compare(filename, rfilename))
            {
                if(time_passed<10)
                {
                    printf("O programa executou com sucesso o comando %d num tempo aceitável de %.7fs.\n", x, time_passed);
                }
                else
                {
                    printf("O programa executou com sucesso o comando %d num tempo inaceitável de %.7fs.\n", x, time_passed);
                }
            }
            else
            {
                printf("O programa não executou com sucesso o comando %d.\n", x);
            }
            free(filename);
            free(rfilename);
        }
        else if(pf==0)
        {
            if(time_passed<10)
            {
                printf("O programa executou o comando %d num tempo aceitável de %.7fs.\n", x, time_passed);
            }
            else
            {
                printf("O programa executou o comando %d num tempo inaceitável de %.7fs.\n", x, time_passed);
            }
        }
        x++;

        free(args->arg1);
        free(args->arg2);
        free(args);
    }
    free(line);
    fclose(fptrI);
    return t;
}