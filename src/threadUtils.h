#ifndef _THREADUTILS_H
#define _THREADUTILS_H

#include <stdint.h>
#include <stdbool.h>
#include "list.h"

typedef struct st_sortingInfo *SortingInfo;

typedef struct st_workerInfo *WorkerInfo;

typedef struct st_reporterInfo *ReporterInfo;

typedef struct st_interceptorInfo *InterceptorInfo;

SortingInfo createSortingInfo(int **dataArray, List *dataList, uint32_t size, uint64_t *time, uint64_t *clock, bool useArray);

WorkerInfo createWorkerInfo(int **dataArrays, List *dataLists, uint64_t *times, uint64_t *clock, int algoIdentifier, uint32_t size, bool useArray);

ReporterInfo createReporterInfo(pthread_t **threads, WorkerInfo workersData, bool isParallel);

InterceptorInfo createInterceptorInfo(pthread_t **threads, uint64_t *times, uint32_t waiting);

int** getSortingDataArray(SortingInfo sinfo);

uint32_t getSortingSize(SortingInfo sinfo);

uint64_t* getSortingTime(SortingInfo sinfo);

uint64_t* getSortingClock(SortingInfo sinfo);

void msleep(uint32_t msec);

void *clockThread(void *arg);

//Uma thread que dorme(sleep) e depois mata(cancel) todas as outras threads que não pararam, exceto a MAE. Então finaliza.
void* interceptorThread(void *arg);

//Uma thread que espera(join) todas as demais para terminar, mesmo se elas foram eternas, para então finalizar.
void *reportThread(void *arg);
#endif
