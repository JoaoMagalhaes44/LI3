#ifndef _QUERY_AUX_H_
#define _QUERY_AUX_H_

int is_driver(char* line);

char* write_filename(int x);

char* write_tst_result(int x);

char* write_rtfilename(int x, int testes);

void write_argv(char* argv[], int x);

int compare(char* exp_file, char* res_file);

#endif