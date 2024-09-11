#ifndef _M_STRUCTS_AUX_H_
#define _M_STRUCTS_AUX_H_

#include "drivers.h"
#include "users.h"
#include "rides.h"

double arredonda(char* num);

double ride_cost(char* car, int distance);

int both_active(char* driver_id, char* user_username, struct drivers* drivers, struct users* users);

int same_gender(char* driver_id, char* user_username, struct drivers* drivers, struct users* users);

void sort_arrays(struct drivers* drivers, struct users* users, struct rides* rides);

#endif