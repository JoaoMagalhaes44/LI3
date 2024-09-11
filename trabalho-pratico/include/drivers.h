#ifndef _DRIVERS_H_
#define _DRIVERS_H_

#include "glib.h"

typedef struct city_score *CITY_SCORE;

typedef struct med_score *MED_SCORE;

typedef struct driver *DRIVER;

typedef struct sdarray *SDARRAY;

typedef struct dh *DH;

typedef struct drivers *DRIVERS;

struct med_score* init_med_score();

struct driver* init_driver(char* line_m);

struct dh* init_dh();

struct sdarray* init_sdarray();

struct drivers* init_drivers(struct dh* dh, struct sdarray* sdarray, struct sdarray* ctarray);

int resize_city_score_arr(struct med_score* med_score);

int resize_sdarray(struct sdarray* sdarray);

int insert_driver_sd(struct driver* driver, struct sdarray* sdarray);

void insert_driver(struct dh* dh, struct sdarray* sdarray, struct sdarray* ctarray, char* line);

struct drivers* load_drivers(char* arg);

int update_city_score(struct driver* driver, double score_driver, char* city);

double update_d(char* date_driver, char* driver_id, int distance, double score_driver, char* city, double tip, struct drivers* drivers);

void is_ordered(char* city,struct drivers* drivers);

///SORTING///

void qSort_drivers(struct drivers* drivers);

void qSort_darray_ct(struct driver** darray, int left, int right, char* city);

void qSort_darray(struct driver** darray, int left, int right);

void swap_driver(struct driver** darray, int i, int j);

int partition_darray_ct(struct driver** darray, int left, int right, char* city);

int partition_darray(struct driver** darray, int left, int right);

///PRINTERS///

char* print_query1_d(char* driver_id, struct drivers* drivers);

int print_query2(int driver_number, struct drivers* drivers, char* filename, int x, int max, int page_size, int interativo);

int print_query7(int driver_number, char* city, struct drivers* drivers, char* filename, int print, int x, int max, int page_size, int interativo);

///MEMORY FREES///

void free_med_score(struct driver* driver);

void free_dhash(gpointer drv);

void free_mem_d(struct drivers* drivers);

///GETTERS///

char* D_IDget_car_class(char* driver_id, struct drivers* drivers);

int D_IDcheck_acc_status(char* driver_id, struct drivers* drivers);

int D_IDcheck_gender(char* driver_id, struct drivers* drivers);

char* D_IDget_acc_creation(char* driver_id, struct drivers* drivers);

char* D_IDget_name(char* driver_id, struct drivers* drivers);

char* Dget_name(struct driver* driver);

char* Dget_gender(struct driver* driver);

char* Dget_id(struct driver* driver);

#endif