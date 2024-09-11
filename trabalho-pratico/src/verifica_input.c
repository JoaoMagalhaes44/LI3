#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "ctype.h"
#include "verifica_input.h"

///CAR_CLASS///

//Verifica se a car_class é alguma combinação de maiúsculas e minúsculas de 'premium'.
int verificaPremium(char* a)
{
    int is = 0;
    if((a[0]=='P' || a[0]=='p') && (a[1]=='R' || a[1]=='r') && (a[2]=='E' || a[2]=='e') && (a[3]=='M' || a[3]=='m') && (a[4]=='I' || a[4]=='i') && (a[5]=='U' || a[5]=='u') && (a[6]=='M' || a[6]=='m'))
    {
        is=1;
    }
    return is;
}

//Verifica se a car_class é alguma combinação de maiúsculas e minúsculas de 'green'.
int verificaGreen(char* a)
{
    int is = 0;
    if((a[0]=='G' || a[0]=='g') && (a[1]=='R' || a[1]=='r') && (a[2]=='E' || a[2]=='e') && (a[3]=='E' || a[3]=='e') && (a[4]=='N' || a[4]=='n'))
    {
        is=1;
    }
    return is;
}

//Verifica se a car_class é alguma combinação de maiúsculas e minúsculas de 'basic'.
int verificaBasic(char* a)
{
    int is = 0;
    if((a[0]=='B' || a[0]=='b') && (a[1]=='A' || a[1]=='a') && (a[2]=='S' || a[2]=='s') && (a[3]=='I' || a[3]=='i') && (a[4]=='C' || a[4]=='c'))
    {
        is=1;
    }
    return is;
}

//Verifica se a car_class corresponde a alguma das válidas.
int verificaCarClass(char* a)
{
    if(verificaPremium(a) || verificaGreen(a) || verificaBasic(a))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

///ACC_STATUS///

//Verifica se o acc_status é alguma combinação de maiúsculas  minúsculas de 'active'.
int verificaActive(char* a)
{
    int is = 0;
    if((a[0]=='A' || a[0]=='a') && (a[1]=='C' || a[1]=='c') && (a[2]=='T' || a[2]=='t') && (a[3]=='I' || a[3]=='i') && (a[4]=='V' || a[4]=='v') && (a[5]=='E' || a[5]=='e'))
    {
        is=1;
    }
    return is;
}

//Verifica se o acc_status é alguma combinação de maiúsculas  minúsculas de 'inactive'.
int verificaInactive(char* a)
{
    int is = 0;
    if((a[0]=='I' || a[0]=='i') && (a[1]=='N' || a[1]=='n') && (a[2]=='A' || a[2]=='a') && (a[3]=='C' || a[3]=='c') && (a[4]=='T' || a[4]=='t') && (a[5]=='I' || a[5]=='i') && (a[6]=='V' || a[6]=='v') && (a[7]=='E' || a[7]=='e'))
    {
        is=1;
    }
    return is;
}

//Verifica a validade do acc_status.
int verificaAccStatus(char* a)
{
    if(verificaActive(a) || verificaInactive(a))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

///SIZE///

//Verifica se o campo possui algum tamanho.
int verificaTamanho(char* a)
{
    if (a[0] != '\0')
    {
        return 1;
    }
    return 0;
}

///DOUBLE///

//Verifica se os doubles dados são válidos.
int verificaDouble(char* a)
{
    if (strlen(a) == 0)
    {
        return 1;
    }
    int i = 0;
    if(a[0] == '-')
    {
        return 1;
    }
    else
    {
        while(a[i] != '.')
        {
            if(isdigit(a[i])) 
            {
                i++;
            }
            else 
            {
                if (a[i]=='\0')
                {
                    return 0;
                }
                else 
                {
                    return 1;
                }
            }
        }
        i++;
        while(a[i]!='\0')
        {
            if(isdigit(a[i])) 
            {
                i++;
            }
            else 
            {
                return 1;
            }
        }
    }

    return 0;
}

///INT///

//Verifica se os int dados são válidos.
int verificaInt(char* a)
{
    if (strlen(a) == 0)
    {
        return 0;
    }
    int i = 0;
    while(a[i]!='\0')
    {
        if(isdigit(a[i]))
        { 
            i++;
        }
        else 
        {
            return 0;
        }
    }
    return 1;
}

///DATE///

//Verifica se os dias e os meses estão dentro dos seus limites.
int verificaD2(char *a)
{
    int day = (a[0]-'0')*10 + a[1]-'0';
    int month = (a[3]-'0')*10 + a[4]-'0';

    if(day >=1 && day <= 31 && month >=1 && month <=12)
    { 
        return 0;
    }
    else 
    {
        return 1;
    }
}

//Verifica se uma data é válida.
int verificaData(char* a)
{
    int i = 0;

    if (strlen(a) == 10)
    {
        while (i<10)
        {
            if (i != 2 && i != 5)
            {
                if (!isdigit(a[i]))
                {
                    return 1;
                }
            }
            else
            {
                if(a[i] != '/')
                {
                    return 1;
                }
            }
            i++;
        }
        return verificaD2(a);
    }
    return 1;
}