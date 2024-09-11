#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "drivers.h"
#include "rides.h"
#include "query7.h"
#include "query_aux.h"

//Invoca a função que imprime o resultado da query 7, fornecendo-lhe todos os argumentos necessários.
int query7(char* arg1, char* arg2, struct drivers* drivers, struct rides* rides, int x, int max, int page_size, int interativo)
{
    int aux;
    int driver_number = atoi(arg1);
    char* city = strdup(arg2);
    int print = 0;

    if(real_city(city, rides))
    {
        is_ordered(city, drivers);
        print = 1;
    }
    
    char* filename = write_filename(x);

    aux = print_query7(driver_number, city,drivers, filename, print, x, max, page_size, interativo);

    free(filename);
    free(city);
    return aux;
}