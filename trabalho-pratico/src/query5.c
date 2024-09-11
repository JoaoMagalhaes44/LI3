#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "rides.h"
#include "query5.h"
#include "query_aux.h"

//Invoca a função que imprime o resultado da query 5, fornecendo-lhe todos os argumentos necessários.
void query5(char* arg1, char* arg2, struct rides* rides, int x)
{
    char* date_1 = strdup(arg1);
    char* date_2 = strdup(arg2);
    
    char* filename = write_filename(x);

    print_query5(date_1, date_2, rides, filename, x);

    free(filename);
}