#include "stdio.h"
#include "stdlib.h"
#include "unistd.h" 
#include "drivers.h"
#include "users.h"
#include "query1.h"
#include "query_aux.h"

//Invoca a função que retorna uma string com as informações-resultado da query 1 para drivers e imprime o resultado.
void query1_driver(char* driver_id, char* filename, struct drivers* drivers, int x)
{
    FILE *arq = fopen(filename, "wt"); 
    int acc_status = D_IDcheck_acc_status (driver_id, drivers);
    if (acc_status == 1)
    {
        char* print_2_file = print_query1_d (driver_id, drivers);

        if (x != 0)
        {
            fprintf(arq, "%s\n", print_2_file);
        }
        else
        {
            printf("%s\n", print_2_file);
        }
        free(print_2_file);
    }
    fclose(arq);
    if (x == 0)
    {
        remove(filename);
    }
}

//Invoca a função que retorna uma string com as informações-resultado da query 1 para users e imprime o resultado.
void query1_user(char* user_username, char* filename, struct users* users, int x)
{
    FILE *arq = fopen(filename, "wt"); 
    int acc_status = U_UNcheck_acc_status(user_username, users);
    if (acc_status == 1)
    {
        char* print_2_file = print_query1_u(user_username, users);

        if (x != 0)
        {
            fprintf(arq, "%s\n", print_2_file);
        }
        else
        {
            printf("%s\n", print_2_file);
        }
        free(print_2_file);
    }
    fclose(arq);
    if (x == 0)
    {
        remove(filename);
    }
}

//Verifica se é pedida informação de um user ou um driver e redireciona para a função correspondente.
void query1(char* arg1, struct drivers* drivers, struct users* users, int x)
{

        char* id = strdup(arg1);
        char* filename = write_filename(x);

        if (is_driver(id))
        {
            query1_driver(id, filename, drivers, x);
        }
        else
        {
            query1_user(id, filename, users, x);
        }

        free(id);
        free(filename);
}