#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "rides.h"
#include "query6.h"
#include "query_aux.h"

//Invoca a função que imprime o resultado da query 6, fornecendo-lhe todos os argumentos necessários.
void query6(char* arg1, char* arg2, char* arg3, struct rides* rides, int x)
{
    char* city = strdup(arg1);
    char* date_1 = strdup(arg2);
    char* date_2 = strdup(arg3);

    char* filename = write_filename(x);

    print_query6(city, date_1, date_2, rides, filename, x);

    free(filename);
}
