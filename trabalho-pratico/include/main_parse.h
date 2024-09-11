#ifndef _MAIN_PARSE_H_
#define _MAIN_PARSE_H_

#include "users.h"
#include "rides.h"
#include "drivers.h"
#include "query_aux.h"
#include "testes_aux.h"

typedef struct args* ARGS;

int get_query(struct args* args);

char* get_arg1(struct args* args);

struct args* init_args(char* line_m);

int dist_query(struct args* args, struct drivers* drivers, struct users* users, struct rides* rides, int x, int max, int page_size, int interativo);

void main_parse(char* input_path, struct drivers* drivers, struct users* users, struct rides* rides);

double* main_parse_teste(char* input_path, struct drivers* drivers, struct users* users, struct rides* rides, float load_time, int testes, int pf);

#endif