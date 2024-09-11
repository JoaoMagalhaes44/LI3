#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "m_structs_aux.h"
#include "drivers.h"
#include "users.h"
#include "rides.h"
#include "verifica_input.h"

#define TPAGE 50

//Função auxiliar de leitura do valor de tip.
double arredonda(char* num)
{
    if (strlen (num) == 3)
    {
        char* first = &num[0];
        int num1 = atoi(first);

        char* second = &num[2];
        int num2 = atoi(second);
        double num3 = num1 + (double)num2/10;  
        return num3;
    }
    else 
    {
        return 0;
    }
}

//Calcula o custo de uma viagem, recebendo como argumento a distância e a car_class.
double ride_cost (char* car_class, int distance)
{
    double profit=0;
    if (verificaBasic(car_class))
    {
        profit = 3.25 + distance*0.62;
    }
    else if(verificaGreen(car_class))
    {
        profit = 4 + distance*0.79;
    }
    else if(verificaPremium(car_class))
    {
        profit = 5.20 + distance*0.94;
    }

    return profit;
}

//Verifica se tanto um user como um driver possuem perfis ativos.
int both_active(char* driver_id, char* user_username, struct drivers* drivers, struct users* users)
{
    int driver_status = D_IDcheck_acc_status(driver_id, drivers);
    int user_status = U_UNcheck_acc_status(user_username, users); 

    return driver_status && user_status;
}

//Verifica se tanto um user como um driver possuem o mesmo género.
int same_gender(char* driver_id, char* user_username, struct drivers* drivers, struct users* users)
{
    int driver_gender = D_IDcheck_gender(driver_id, drivers);
    int user_gender = U_UNcheck_gender(user_username, users);

    if (driver_gender == 1 && user_gender == 1)
    {
        return 1;
    }

    if (driver_gender == 0 && user_gender == 0)
    {
        return 2;
    }
    return 0;
}

//Invoca funções de ordenação de arrays em cada módulo principal, drivers.c, users.c e rides.c.
void sort_arrays(struct drivers* drivers, struct users* users, struct rides* rides)
{
    qSort_drivers(drivers);
    qSort_users(users);
    qSort_rides(rides, drivers, users);
}