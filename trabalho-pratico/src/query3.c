#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "users.h"
#include "query3.h"
#include "query_aux.h"

//Invoca a função que imprime o resultado da query 3, fornecendo-lhe todos os argumentos necessários.
int query3(char* arg1, struct users* users, int x, int max, int page_size, int interativo)
{
    int aux;
    int user_number = atoi(arg1);

    char* filename = write_filename(x);

    aux = print_query3(user_number, users, filename, x, max, page_size, interativo);

    free(filename);
    return aux;
}