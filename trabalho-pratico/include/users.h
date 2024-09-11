#ifndef _USERS_H_
#define _USERS_H_

#include "glib.h"

typedef struct user *USER;

typedef struct suarray *SUARRAY;

typedef struct uh *UH;

typedef struct users *USERS;

struct user* init_user(char* line_m);

struct uh* init_uh();

struct suarray* init_suarray();

struct users* init_users(struct uh* uh, struct suarray* suarray);

int resize_suarray(struct suarray* suarray);

int insert_user_su(struct user* user, struct suarray* suarray);

void insert_user(struct uh* uh, struct suarray* suarray, char* line);

struct users* load_users(char* arg);

void update_u(char* date_user, char* user_username, double ride_cost, int distance, double score_user, double tip, struct users* users);

void swap_user(struct user** uarray, int i, int j);

void qSort_uarray(struct user** uarray, int left, int right);

void qSort_users(struct users* users);

///PRINTERS///

char* print_query1_u(char* user_username, struct users* users);

int print_query3(int user_number, struct users* users, char* filename, int x ,int max, int page_size, int interativo);

///MEMORY FREES///

void free_uhash(gpointer usr);

void free_mem_u(struct users* users);

///GETTERS///

int U_UNcheck_acc_status(char* user_username, struct users* users);

int U_UNcheck_gender(char* user_username, struct users* users);

char* U_UNget_acc_creation(char* user_username, struct users* users);

char* U_UNget_name(char* user_username, struct users* users);

char* Uget_name(struct user* user);

char* Uget_gender(struct user* user);

char* Uget_username(struct user* user);

#endif