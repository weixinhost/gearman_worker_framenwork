#ifndef __framework_H_
#define __framework_H_
#include <stdio.h>
#include <libgearman/gearman.h>

// the gearmand worker function prototype
typedef void(*worker_func_ptr)(gearman_job_st *job, void *result, size_t *result_len, gearman_return_t *ret_ptr);


// return 1 when config_path is a file and parse ok.

int mgearmand_parse_config(const char *config_path,char *node_name);

long mgearmand_config_get_long(const char *name);

char* mgearmand_config_get_string(const char *name);

int mgearmand_add_function(const char *name,worker_func_ptr callback);

int mgearmand_delete_function(const char *name);

int mgearmand_worker_init();

int mgearmand_worker_run();


#endif //framework.h_H_
