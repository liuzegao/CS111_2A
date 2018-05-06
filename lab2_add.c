
//
//  lab2_add.c
//  Project_2A
//
//  Created by Zegao on 5/5/18.
//  Copyright © 2018 pro. All rights reserved.
//

#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

char isThereThread = 'N';
char isThereIterations = 'N';
char isThereYield = 'N';
int nThreads = 1;
int nIterations = 1;
long long counter = 0;
char testName[25] = "add";


void add(long long *pointer, long long value) {
    long long sum = *pointer + value;
    *pointer = sum;
}

void *mythread(){
    long long addOne = 1;
    long long minusOne = -1;
    for(int i = 0; i < nIterations; i++){
        add(&counter, addOne);
    }
    for(int i = 0; i < nIterations; i++){
        add(&counter, minusOne);
    }
    return NULL;
}

int main(int argc, char **argv){
    
    static struct option long_options[] = {
        {"thread", required_argument, 0, 'T'},
        {"iterations", required_argument, 0, 'I'},
        {"yield", no_argument, 0, 'Y'},
        {"sync", required_argument, 0, 'S'},
    };
    
    int ch;
    while((ch = getopt_long(argc, argv,"",long_options,NULL))!=-1){
        switch(ch){
            case 'T': isThereThread = 'Y';
                nThreads = atoi(optarg);
                break;
            case 'I':
                isThereIterations = 'Y';
                nIterations = atoi(optarg);
                break;
            case 'Y':
                isThereYield = 'Y';
                break;
            default:
                exit(1); break;
        }
    }
    
    pthread_t* p = (pthread_t*) malloc(nThreads * sizeof(pthread_t));
    if (p == NULL) {
        fprintf(stderr, "Failed to malloc space for pthread p\n");
        exit(1);
    }
    
    struct timespec startTime;
    int rs = clock_gettime(CLOCK_MONOTONIC, &startTime);
    if(rs == -1){
        fprintf(stderr, "Failed to recored start time\n");
    }
    
    for(int i = 0; i < nThreads; i++){
        if(pthread_create(&p[i], NULL, mythread, NULL) != 0){
            fprintf(stderr, "Failed to create thread\n");
            free(p);
            exit(1);
        }
    }
    
    for(int i = 0; i < nThreads; i++){
        if(pthread_join(p[i], NULL) != 0){
            fprintf(stderr, "Failed to join thread\n");
            exit(1);
        }
    }
    
    free(p);
    
    struct timespec endTime;
    rs = clock_gettime(CLOCK_MONOTONIC, &endTime);
    if(rs == -1){
        fprintf(stderr, "Failed to recored end time\n");
    }
    
    long long timeElapsed = (endTime.tv_sec-startTime.tv_sec)*1000000000;
    timeElapsed += (endTime.tv_nsec-startTime.tv_nsec);
    
    long long nOperations = nIterations*nThreads*2;
    
    long long timePerOperation = timeElapsed/nOperations;

    printf("%s,%d,%d,%lld,%lld,%lld,%lld\n",testName, nThreads,nIterations,nOperations, timeElapsed, timePerOperation, counter);
    
    exit(0);
}
