#ifndef _THREADUTILS_H
#define _THREADUTILS_H

#include <stdint.h>
#include <stdbool.h>
#include "list.h"

typedef struct st_sortingInfo_int *SortingInfo_int;

typedef struct st_workerInfo_int *WorkerInfo_int;

typedef struct st_reporterInfo_int *ReporterInfo_int;

typedef struct st_interceptorInfo_int *InterceptorInfo_int;

SortingInfo_int createSortingInfo_int(int **dataArray, List *dataList, uint32_t size, uint64_t *time, uint64_t *clock, bool useArray);

WorkerInfo_int createWorkerInfo_int(int **dataArrays, List *dataLists, uint64_t *times, uint64_t *clock, int algoIdentifier, uint32_t size, bool useArray);

ReporterInfo_int createReporterInfo_int(pthread_t **threads, WorkerInfo_int workersData, bool isParallel);

InterceptorInfo_int createInterceptorInfo_int(pthread_t **threads, uint64_t *times, uint32_t waiting);

int** getSorting_intDataArray(SortingInfo_int sinfo);

uint32_t getSorting_intSize(SortingInfo_int sinfo);

uint64_t* getSorting_intTime(SortingInfo_int sinfo);

uint64_t* getSorting_intClock(SortingInfo_int sinfo);

void msleep_int(uint32_t msec);

void *clockThread_int(void *arg);

//Uma thread que dorme(sleep) e depois mata(cancel) todas as outras threads que não pararam, exceto a MAE. Então finaliza.
void* interceptorThread_int(void *arg);

//Uma thread que espera(join) todas as demais para terminar, mesmo se elas foram eternas, para então finalizar.
void *reportThread_int(void *arg);
#endif
