/*
 * Copyright(C) 2016
 * 	Amanda M. P. Amorim (amandamp.aorim@gmail.com)
 * 	Matheus A. Souza (matheusalcantarasouza@gmail.com)
 */

#include <stdio.h>
#include <papi.h>
#include <stdlib.h>

#define NUM_EVENTS 4

int main(int argc, char **argv)
{
    int event[NUM_EVENTS] = {PAPI_L2_TCA, PAPI_L2_TCM, PAPI_L3_TCA, PAPI_L3_TCM};
    long long values[NUM_EVENTS];
    pid_t pid;
    char* newargv[10]; // FIXME: Improve args wrapper
    int pidStatus;
    
    /* Start counting events */
    if (PAPI_start_counters(event, NUM_EVENTS) != PAPI_OK) {
        fprintf(stderr, "PAPI_start_counters - FAILED\n");
        exit(1);
    }

    /* Wrap your file */
    if(!(pid = fork()))
	execv(argv[1], argv);
    waitpid(pid, pidStatus, 0);

    /* Read the counters */
    if (PAPI_read_counters(values, NUM_EVENTS) != PAPI_OK) {
        fprintf(stderr, "PAPI_read_counters - FAILED\n");
        exit(1);
    }

    /* Show readings */
    /* FIXME: The case when L3 does not exist... */
    printf("# of L2 cache access	= %lld\n", values[0]);
    printf("# of L2 cache misses	= %lld\n", values[1]);
    printf("	%% of L2 cache misses	= %2.3f\n\n",(double) values[1] / (double) values[0]);
    printf("# of L3 cache access	= %lld\n", values[2]);
    printf("# of L3 cache misses	= %lld\n", values[3]);
    printf("	%% of L3 cache misses	= %2.3f\n\n", (double) values[3] / (double) values[2]);

	
    /* Stop counting events */
    if (PAPI_stop_counters(values, NUM_EVENTS) != PAPI_OK) {
        fprintf(stderr, "PAPI_stoped_counters - FAILED\n");
        exit(1);
    }

    return 0;
}
