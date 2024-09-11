#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "drivers.h"
#include "query2.h"
#include "query_aux.h"

//Invoca a função que imprime o resultado da query 2, fornecendo-lhe todos os argumentos necessários.
int query2(char* arg1, struct drivers* drivers, int x, int max, int page_size, int interativo)
{
    int aux;
    int driver_number = atoi(arg1);

    char* filename = write_filename(x);

    aux = print_query2(driver_number, drivers, filename, x, max, page_size, interativo);

    free(filename);
    return aux;
}