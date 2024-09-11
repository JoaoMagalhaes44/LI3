#include "stdio.h"
#include "stdlib.h"
#include "dates.h"

#define base_date "09/10/2022"

//Transforma a data com formato dd/mm/aaaa em um int, correspondente ao número de dias.
int create_Date(char* date)
{
    return 10*(date[0]-'0') + (date[1]-'0') + 
           31*( 10*(date[3]-'0')+ (date[4]-'0')) +
           365*( 1000*(date[6]-'0') + 100*(date[7]-'0') + 10*(date[8]-'0') + (date[9]-'0'));
}

//Compara duas datas através da diferença entre os dias das mesmas.
int compara_datas(char* A, char* B)
{ 
    int x = 0;
    if(A[9] == B[9])
    {
        return create_Date(A)- create_Date(B);
    }
    else 
    {
        if(( (10*(A[8]-'0') + A[9]-'0') - (10*(B[8]-'0') + B[9]-'0') ) > 0) 
        {
            x = 1;
        }
        else 
        {
            x = (-1);
        }
        return x;
    }
}

//Calcula a idade de um indivíduo através da sua data de nascimento.
int calcula_idade(int birth_date)
{
    int D_atual = create_Date(base_date);

    if((D_atual-birth_date)<=0) 
    {
        return (birth_date-D_atual)/365;
    }
    else 
    {
        return (D_atual-birth_date)/365;
    }
}

//Verifica se uma data pertence aos limites dados como argumento.
int belongs(char* date_1, char* date_2, char* ride_date)
{
    if(compara_datas(date_1,ride_date)<=0 && compara_datas(date_2,ride_date)>=0)
    {
        return 1;
    }
    return 0;
}