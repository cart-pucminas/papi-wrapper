/*
 * Copyright(C) 2016
 * 	Amanda M. P. Amorim (amandamp.aorim@gmail.com)
 * 	Matheus A. Souza (matheusalcantarasouza@gmail.com)
 */

#include <stdio.h>
#include <papi.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    int eventSet;
    eventSet = PAPI_NULL;
    pid_t pid;
    char* newargv[10]; // FIXME: Improve args wrapper
    int pidStatus, retval, numEvents, numEventSets;
/*   int event[NUM_EVENTS] = {PAPI_L2_TCA, PAPI_L2_TCM, PAPI_L3_TCA, PAPI_L3_TCM}; */
    long long values[4];


    retval = PAPI_library_init(PAPI_VER_CURRENT);
    if (retval != PAPI_VER_CURRENT) {
	fprintf(stderr,"PAPI_library_init - FAILED\n", retval);
    }

    /* Create EventSet */
    if(PAPI_create_eventset(&eventSet) != PAPI_OK) {
        fprintf(stderr, "PAPI_create_eventset - FAILED\n");
    }

    if((retval=PAPI_add_event(eventSet,PAPI_L2_TCA))!= PAPI_OK) fprintf(stderr, "PAPI_add_event - FAILED\n\t%s\n", retval);
    if((retval=PAPI_add_event(eventSet,PAPI_L2_TCM))!= PAPI_OK) fprintf(stderr, "PAPI_add_event - FAILED\n\t%s\n", retval);
    if((retval=PAPI_add_event(eventSet,PAPI_L3_TCA))!= PAPI_OK) fprintf(stderr, "PAPI_add_event - FAILED\n\t%s\n", retval);
    if((retval=PAPI_add_event(eventSet,PAPI_L3_TCM))!= PAPI_OK) fprintf(stderr, "PAPI_add_event - FAILED\n\t%s\n", retval);

    /* Wrap your file */
    if(!(pid = fork())) {
        /* Need to set inherit options */
        if( (retval = PAPI_attach(eventSet, pid)) != PAPI_OK) {
            fprintf(stderr, "PAPI_attach - FAILED\n\t%s\n", PAPI_strerror(retval));
            exit(1);
        }

	/* Start counting events */
	if ( (retval = PAPI_start(eventSet)) != PAPI_OK) {
            fprintf(stderr, "PAPI_start - FAILED\n\t%s\n", retval);
            exit(1);
    	}

        execv(argv[1], argv);

	if( (retval = PAPI_detach(eventSet)) != PAPI_OK) {
	    fprintf(stderr, "PAPI_detach - FAILED\n\t%s\n", PAPI_strerror(retval));
            exit(1);
	}

	/* Read the counters and stop */
	if ( (retval = PAPI_stop(eventSet,values)) != PAPI_OK) {
            fprintf(stderr, "PAPI_stop - FAILED\n\t%s\n", retval);
            exit(1);
	}

    }
    waitpid(pid, pidStatus, 0);

    /* Show readings */
    printf("# of L2 cache access	= %lld\n", values[0]);
    printf("# of L2 cache misses	= %lld\n", values[1]);
//  printf("	%% of L2 cache misses	= %2.3f\n\n",(double) *values[1] / (double) *values[0]);
    printf("# of L3 cache access	= %lld\n", values[2]);
    printf("# of L3 cache misses	= %lld\n", values[3]);
//    printf("	%% of L3 cache misses	= %2.3f\n\n", (double) *values[3] / (double) *values[2]);
	
    return 0;
}
