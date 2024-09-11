#include "stdio.h"
#include "stdlib.h"
#include "unistd.h" 
#include "rides.h"
#include "query9.h"
#include "query_aux.h"


//Invoca a função que imprime o resultado da query 9, fornecendo-lhe todos os argumentos necessários.
int query9(char* arg1, char* arg2, struct rides* rides, int x, int max, int page_size, int interativo)
{
    int aux ;
    char* date_1 = strdup(arg1);
    char* date_2 = strdup(arg2);

    char* filename = write_filename(x);

    aux = print_query9(date_1, date_2, rides, filename, x, max, page_size, interativo);


    free(filename);
    free(date_1);
    free(date_2);
    return aux;
}