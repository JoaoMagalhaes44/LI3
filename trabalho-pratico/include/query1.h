#ifndef _QUERY1_H_
#define _QUERY1_H_

#include "drivers.h"
#include "users.h"

void query1_driver(char* driver_id, char* filename, struct drivers* drivers, int x);

void query1_user(char* user_username, char* filename, struct users* users, int x);

void query1(char* arg1, struct drivers* drivers, struct users* users, int x);

#endif