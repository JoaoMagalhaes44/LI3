#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "rides.h"
#include "query4.h"
#include "query_aux.h"

//Invoca a função que imprime o resultado da query 4, fornecendo-lhe todos os argumentos necessários.
void query4(char* arg1, struct rides* rides, int x)
{
    char* city = strdup(arg1);

    char* filename = write_filename(x);

    print_query4(city, rides, filename, x);

    free(filename);
    free(city);
}