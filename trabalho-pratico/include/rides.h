#ifndef _RIDES_H_
#define _RIDES_H_

#include "glib.h"
#include "users.h"
#include "drivers.h"

typedef struct ride *RIDE;

typedef struct srarray *SRARRAY;

typedef struct mpreco *MPRECO;

typedef struct rh *RH;

typedef struct rides *RIDES;

typedef struct foreach *FOREACH;

typedef struct foreach_5_6* FOREACH_5_6;

struct ride* init_ride(char* line_m);

struct mpreco* init_mpreco();

struct rh* init_rh();

struct rh* init_ch();

struct srarray* init_srarray();

struct foreach* init_foreach(struct drivers* drivers, struct users* users, struct rh*);

struct foreach_5_6* init_foreach_5_6(char* date_1, char* date_2, char* city);

struct rides* init_rides(struct rh* rh, struct rh* ch, struct srarray* sr_male, struct srarray* sr_female, struct srarray* sr_tip);

int resize_srarray(struct srarray* srarray);

int insert_ride_sr(struct ride* ride,struct srarray* srarray);

void insert_ride(struct rh* rh, struct srarray* sr_male, struct srarray* sr_female, struct srarray* sr_tip, char* line, struct drivers* drivers, struct users* users);

void update_c(char* city, double ride_cost, struct rh* ch);

void foreach_updt(gpointer key, gpointer value, gpointer userdata);

void foreach_q5(gpointer key, gpointer value, gpointer userdata);

void foreach_q6(gpointer key, gpointer value, gpointer userdata);

int real_city (char* city,struct rides* rides);

///SORTING///

void swap_ride(struct ride** rarray, int i, int j);

int partition_rarray_t(struct ride** rarray, int left, int right, struct drivers* drivers, struct users* users);

int partition_rarray_fm(struct ride** rarray, int left, int right, struct drivers* drivers, struct users* users);

void qSort_rarray_t(struct ride** rarray, int left, int right, struct drivers* drivers, struct users* users);

void qSort_rarray_fm(struct ride** rarray, int left, int right, struct drivers* drivers, struct users* users);

void qSort_rides(struct rides* rides, struct drivers* drivers, struct users* users);

///LOAD///

struct rides* load_rides(char* arg, struct drivers* drivers, struct users* users);

///PRINTERS///

void print_query4(char* city, struct rides* rides, char* filename, int x);

void print_query5(char* date_1, char* date_2, struct rides* rides, char* filename, int x);

void print_query6(char* city, char* date_1, char* date_2, struct rides* rides, char* filename, int x);

int print_query8(char* gender, int acc_age, struct drivers* drivers, struct users* users, struct rides* rides, char* filename, int x, int max, int page_size, int interativo);

int print_query9(char* date_1, char* date_2, struct rides* rides, char* filename, int x, int max, int page_size, int interativo);

///MEMORY FREES///

void free_rhash(gpointer rd);

void free_chash_k(gpointer ct);

void free_chash_v(gpointer mp);

void free_mem_r(struct rides* rides);

///GETTERS///

char* Rget_date(struct ride* ride);

char* Rget_driver_id(struct ride* ride);

char* Rget_user_username(struct ride* ride);

char* Rget_city (struct ride* ride);

int Rget_distance(struct ride* ride);

double Rget_score_user(struct ride* ride);

double Rget_score_driver(struct ride* ride);

double Rget_tip(struct ride* ride);

char* Rget_id(struct ride* ride);

#endif