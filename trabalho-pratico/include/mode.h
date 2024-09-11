#ifndef _MODE_H_
#define _MODE_H_

#include "drivers.h"
#include "users.h"
#include "rides.h"

typedef struct data *DATA;

struct data* init_data(struct drivers* drivers, struct users* users, struct rides* rides);

struct data* load_data(char* file_path);

int menu_paginacao();

void realiza_query(struct data* data);

int MainMenu();

int submenu_1();

int submenu_1_1();

int submenu_2();

void free_mem(struct data* data);

struct data* i_parse();

void print_instructions();

int interativo();

int batch(char* argv[]);

void batch_teste(double** time, char* argv[], int testes, int pf);

#endif