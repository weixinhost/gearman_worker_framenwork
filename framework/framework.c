#include "framework.h"
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>
#include <signal.h>

#include "../lib/iniparser/iniparser.h"

#define MGEARMAND_MAX_WORKER_NUM 128

typedef struct {

    char name[255];

    worker_func_ptr callback;

} _mgearmand_worker;


struct {

    _mgearmand_worker worker_list[MGEARMAND_MAX_WORKER_NUM];

    size_t len;

} global_worker_list;


dictionary *global_ini = NULL;

char global_ini_node_name[128];

gearman_worker_st *worker_obj = NULL;

int mgearmand_parse_config(const char *config_path,char *node_name){

    if( !config_path ) return 0;

    global_ini = iniparser_load(config_path);

    memcpy(global_ini_node_name,node_name,strlen(node_name));

    if(global_ini) return 1;

    return 0;

}

long mgearmand_config_get_long(const char *name){

    if(!global_ini || !global_ini_node_name) return 0;

    char command[255] = {0};

    sprintf(command,"%s:%s",global_ini_node_name,name);

    return iniparser_getint(global_ini,command,0);

}

char* mgearmand_config_get_string(const char *name){

    if(!global_ini || !global_ini_node_name) return 0;

    char command[255] = {0};

    sprintf(command,"%s:%s",global_ini_node_name,name);

   return  iniparser_getstring(global_ini,command,NULL);

}


int mgearmand_add_function(const char *name,worker_func_ptr callback){

    if(global_worker_list.len > MGEARMAND_MAX_WORKER_NUM) return 0;

    memcpy (global_worker_list.worker_list[global_worker_list.len].name,name,strlen(name));

    global_worker_list.worker_list[global_worker_list.len].callback = callback;

    global_worker_list.len++;

    return 1;

}

int mgearmand_delete_function(const char *name){

    return 0;

}



void _mgearnad_signal_handle(int sig){

    fprintf(stderr,"sig exit:%d",sig);

    if(worker_obj){

        gearman_worker_free(worker_obj);

    }

    exit(sig);

}

void mgearmand_signal_init(){

    signal(SIGBUS,  _mgearnad_signal_handle);
    signal(SIGSEGV, _mgearnad_signal_handle);
    signal(SIGILL,  _mgearnad_signal_handle);
    signal(SIGHUP,  _mgearnad_signal_handle);
    signal(SIGQUIT, _mgearnad_signal_handle);
    signal(SIGABRT, _mgearnad_signal_handle);
    signal(SIGKILL, _mgearnad_signal_handle);
    signal(SIGTERM, _mgearnad_signal_handle);
    signal(SIGTSTP, _mgearnad_signal_handle);
    signal(SIGSTOP, _mgearnad_signal_handle);

}





int mgearmand_worker_init(){

    char *server_host = mgearmand_config_get_string ("gearman_host");

    if(! server_host) {

        fprintf (stderr, "config item `gearman_host` not found.\n");

        return 0;

    }

    long server_port = mgearmand_config_get_long ("gearman_port");

    if(! server_port) {

        fprintf (stderr, "config item `gearman_port` not found.\n");

        return 0;

    }

    long timeout = mgearmand_config_get_long ("gearman_timeout");


    worker_obj = gearman_worker_create(NULL);

    if(worker_obj){

        gearman_worker_add_server(worker_obj,server_host,server_port);

        int worker_current = 0;

        for(;worker_current < global_worker_list.len;worker_current++){

            _mgearmand_worker *current = &(global_worker_list.worker_list[worker_current]);

            gearman_worker_add_function(worker_obj,current->name,15,current->callback,NULL);

        }

        gearman_worker_set_timeout(worker_obj,timeout);

        mgearmand_signal_init();

        return worker_obj;

    }else{

        fprintf (stderr, "create worker_obj error.\n");

        return 0;

    }

}

int mgearmand_worker_run(){

    if(! worker_obj) return 0;

    int ret = 0;

    while(1){

        ret =  gearman_worker_wait(worker_obj);

        if(ret == 7){

            ret = gearman_worker_work (worker_obj);

        }

        usleep(2000);
    }

}
