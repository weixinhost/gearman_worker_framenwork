#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>

#include "../framework/framework.h"

void printf_command_help(){

    printf(
            "\n"
                    "======================================================================\n"
                    "++++++++++++++++++   Wxhost-Gearman-Worker-Framework Helper   +++++++++++++++++++\n"
                    "======================================================================\n"
                    "\n"
                    "-h            show this help page\n"
                    "-c            config file path"
                    "\n"
    );
}

void* test_function(gearman_job_st *job, void *result, size_t *result_len, gearman_return_t *ret_ptr);

int main(int argc, char **argv){

    int opt = 0, ret = 0;

    char *ini_file = NULL;

    while((opt = getopt(argc,argv,"c:")) != -1){

        switch(opt){

            case 'c':{ ini_file = optarg; }break;

            default:{

                printf_command_help ();

            }

        }

    }


   ret =  mgearmand_parse_config(ini_file,"product");

   if(!ret){

       fprintf(stderr,"parse init_file error.");

       return 0;
   }

    mgearmand_add_function ("test",test_function);

    mgearmand_worker_init ();

    mgearmand_worker_run ();

    return 1;
}

void* test_function(gearman_job_st *job, void *result, size_t *result_len, gearman_return_t *ret_ptr){

    /**
     * get the job data
     */
    size_t job_data_size = 0;
    void *job_data = gearman_job_take_workload(job,&job_data_size);

    /**
     * init job result
     */

    result = (void*)malloc(job_data_size);
    memcpy(result,job_data,job_data_size);
    *result_len = job_data_size;

    *ret_ptr = GEARMAN_SUCCESS;

    return result;
}










