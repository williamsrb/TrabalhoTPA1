#ifndef _THREADUTILS_H
#define _THREADUTILS_H

#include <stdint.h>

typedef struct st_sortingInfo *SortingInfo;

typedef struct st_workerInfo *WorkerInfo;

typedef struct st_reporterInfo *ReporterInfo;

typedef struct st_interceptorInfo *InterceptorInfo;

SortingInfo createStructSortingInfo(int **dataArray, uint32_t size, uint32_t *time, uint32_t *clock);

WorkerInfo createStructWorkerInfo(int **dataArrays, uint32_t *times, uint32_t *clock, int algoIdentifier, uint32_t size);

ReporterInfo createReporterInfo(pthread_t **threads, WorkerInfo workersData);

InterceptorInfo createInterceptorInfo(pthread_t **threads, uint32_t *times, uint32_t waiting);

void msleep(uint32_t msec);

void *clockThread(void *arg);

//Uma thread que dorme(sleep) e depois mata(cancel) todas as outras threads que não pararam, exceto a MAE. Então finaliza.
void* interceptorThread(void *arg);

//Uma thread que espera(join) todas as demais para terminar, mesmo se elas foram eternas, para então finalizar.
void *reportThread(void *arg);
#endif
