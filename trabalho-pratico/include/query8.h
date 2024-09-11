#ifndef _QUERY8_H_
#define _QUERY8_H_

#include "rides.h"
#include "users.h"
#include "drivers.h"

int query8(char* arg1, char* arg2, struct drivers* drivers, struct users* users, struct rides* rides, int x, int max, int page_size, int interativo);

#endif