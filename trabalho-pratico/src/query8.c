#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "rides.h"
#include "users.h"
#include "drivers.h"
#include "query8.h"
#include "query_aux.h"

//Invoca a função que imprime o resultado da query 8, fornecendo-lhe todos os argumentos necessários.
int query8(char* arg1, char* arg2, struct drivers* drivers, struct users* users, struct rides* rides, int x, int max, int page_size, int interativo)
{
    int aux;
    char* gender = strdup(arg1);
    int acc_age = atoi (arg2);

    char* filename = write_filename(x);

    aux = print_query8(gender, acc_age, drivers, users, rides, filename, x, max, page_size, interativo);

    free(filename);
    free(gender);
    return aux;
}