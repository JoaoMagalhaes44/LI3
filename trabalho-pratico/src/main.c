#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "sys/stat.h"
#include "unistd.h" 
#include "mode.h"
#include "testes.h"

//Função main; Face aos argumentos dados ao programa, chama o modo de execução correspondente (batch, interativo ou testes).
int main(int argc, char* argv[])
{
    if(argc==1)
    {
        if (!strcmp("./programa-testes", argv[0])) testing();
        else interativo();
    }
    else if(argc==3)
    {
        batch(argv);
    }

    return 0;
}