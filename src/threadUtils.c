#include <time.h>
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "list.h"
#include "typeUtils.h"
#include "threadUtils.h"
#include "algorithms.h"

//#define DEBUG printf("Arquivo: %s, Função: %s, Linha: %d\n", __FILE__, __func__, __LINE__)

//------------- Estruturas usadas pelas threads para troca de informação ----------------//
struct st_sortingInfo {
	int **dataArray; //Array de dados usado por 1 thread trabalhadora
	List *dataList;
	uint32_t size; //Tamanho do array.
	uint64_t *time; //Tempo de execução da thread
	uint64_t *clock; //Ponteiro para o relógio
	bool useArray; //Define se a thread usará vetor ou lista encadeada
};

struct st_workerInfo {
	int **dataArrays; //Arrays de dados usados pelas threads
	List *dataLists;
	uint64_t *times; //Usado pela mãe para contabilizar o tempo das treads filhas
	uint64_t *clock; //Ponteiro para o relógio
	int algoIdentifier; //Identificador dos tipos de algoritmos a serem executados. É possível também descobrir a quantidade de arrays
	uint32_t size; //Tamanho dos arrays. Tamanho único
	bool useArray; //Define se as threads usarão vetor ou lista encadeada
};

struct st_reporterInfo {
	pthread_t **threads; //Vetor de ponteiros p/ threads trabalhadoras
	WorkerInfo workersData; //Dados para passar para as threads trabalhadoras
	bool isParallel; //Define se as threads serão executadas em paralelo ou sequencialmente
};

struct st_interceptorInfo {
	pthread_t **threads; //Vetor de ponteiros p/ threads trabalhadoras
	uint64_t *times; //Tempo de execução das threads - Serve de flag para cancelar as threads
	uint32_t waiting; //Tempo que a thread espera antes de matar as trabalhadoras
};


//------------- Funções de uso geral -------------------------------//
SortingInfo createSortingInfo(int **dataArray, List *dataList, uint32_t size, uint64_t *time, uint64_t *clock, bool useArray) {
	SortingInfo newSorting = (SortingInfo) malloc(sizeof(struct st_sortingInfo));
	newSorting->dataArray = dataArray;
	newSorting->dataList = dataList;
	newSorting->size = size;
	newSorting->time = time;
	newSorting->clock = clock;
	newSorting->useArray = useArray;
	return newSorting;
}

WorkerInfo createWorkerInfo(int **dataArrays, List *dataLists, uint64_t *times, uint64_t *clock, int algoIdentifier, uint32_t size, bool useArray) {
	WorkerInfo newWorker = (WorkerInfo) malloc(sizeof(struct st_workerInfo));
	newWorker->dataArrays = dataArrays;
	newWorker->dataLists = dataLists;
	newWorker->times = times;
	newWorker->clock = clock;
	newWorker->algoIdentifier = algoIdentifier;
	newWorker->size = size;
	newWorker->useArray = useArray;
	return newWorker;
}

ReporterInfo createReporterInfo(pthread_t **threads, WorkerInfo workersData, bool isParallel) {
	ReporterInfo newReporter = (ReporterInfo) malloc(sizeof(struct st_reporterInfo));
	newReporter->threads = threads;
	newReporter->workersData = workersData;
	newReporter->isParallel = isParallel;
	return newReporter;
}

InterceptorInfo createInterceptorInfo(pthread_t **threads, uint64_t *times, uint32_t waiting) {
	InterceptorInfo newInterceptor = (InterceptorInfo) malloc(sizeof(struct st_interceptorInfo));
	newInterceptor->threads = threads;
	newInterceptor->times = times;
	newInterceptor->waiting = waiting;
	return newInterceptor;
}

int** getSortingDataArray(SortingInfo sinfo) {
	return sinfo->dataArray;
}

uint32_t getSortingSize(SortingInfo sinfo) {
	return sinfo->size;
}

uint64_t* getSortingTime(SortingInfo sinfo) {
	return sinfo->time;
}

uint64_t* getSortingClock(SortingInfo sinfo) {
	return sinfo->clock;
}

//---------------- Funções usadas pelas threads ------------------------//
//http://cc.byexamples.com/2007/05/25/nanosleep-is-better-than-sleep-and-usleep/
void msleep(uint32_t msec) {
	struct timespec timeout0;
	struct timespec timeout1;
	struct timespec* tmp;
	struct timespec* t0 = &timeout0;
	struct timespec* t1 = &timeout1;

	t0->tv_sec = msec / 1000;
	t0->tv_nsec = (msec % 1000) * (1000 * 1000);

	while((nanosleep(t0, t1) == (-1)) && (errno == EINTR)) {
		tmp = t0;
		t0 = t1;
		t1 = tmp;
	}
}

//------ THREADS ------//
//Thread relógio, alteranativa monotônica à "gettimeofday()", que deve rodar durante todo o programa alterando uma variável compartilhada que guarda o uma contagem em milisegundos
void *clockThread(void *arg) {
	while(1) {
		(*((uint64_t *)arg))++;
		msleep(1); //Dorme 1 milissegundo
	}
	return EXIT_SUCCESS;
}

//Uma thread que dorme(sleep) e depois mata(cancel) todas as outras threads que não pararam, exceto a Reporter. Então finaliza.
void* interceptorThread(void *arg) {
	InterceptorInfo infos = (InterceptorInfo)arg;
	int iter;
	bool max = false;
	pthread_t victim;
	msleep(infos->waiting);
	//printf("interceptorThread awakes\n");
	for(iter = 0; (iter < /* quantdade máxima possível de threads que ele pode cancelar */ 8) && (max == false); iter++) {
		if(infos->threads[iter] != NULL) {
			victim = *(infos->threads[iter]);
			if(infos->times[iter] == UINT64_MAX) { //Significa que ainda não terminou
				pthread_cancel(victim);
				//printf("Thread %d cancelada\n", iter);
			} else {
				//printf("Thread %d já havia terminado: %llu\n", iter, infos->times[iter]);
			}
		} else {
			max = true;
		}
	}
	return EXIT_SUCCESS;
}

//Uma thread que espera(join) todas as demais para terminar, mesmo se elas forem eternas, para então finalizar.
void *reportThread(void *arg) {
	ReporterInfo infos = (ReporterInfo)arg;
	WorkerInfo workerInfos = infos->workersData;
	SortingInfo *data;
	int iter, algoCount = bitCounter((uint32_t)workerInfos->algoIdentifier);
	int *retval, ret = 300;
	retval = &ret;
	
	data = (SortingInfo *) malloc(algoCount * sizeof(SortingInfo)); //Alocar para quantidade exata de algoritmos
	for(iter = 0; iter < algoCount; iter++) {
		data[iter] = (SortingInfo) malloc(sizeof(struct st_sortingInfo));
		data[iter]->dataArray = &(workerInfos->dataArrays[iter]); //Funciona com vetores alocados dinamicamente
		//data[iter]->dataArray = &(workerInfos->dataArrays); //Licença poética por causa do vetor fixo
		data[iter]->size = workerInfos->size;
		data[iter]->time = &(workerInfos->times[iter]);
		data[iter]->clock = workerInfos->clock;
		//workerInfos->dataArrays += workerInfos->size; //Licença poética por causa do vetor fixo
	}
	iter = 0;
	
	//Create
	if(workerInfos->algoIdentifier & 1) {
		pthread_create(infos->threads[0], NULL, mergeThread, data[iter++]);
		//printArray(*(data[iter]->dataArray), data[iter]->size);
		//iter++;
	}
	
	if(workerInfos->algoIdentifier & 2) {
		pthread_create(infos->threads[1], NULL, heapThread, data[iter++]);
		//printArray(*(data[iter]->dataArray), data[iter]->size);
		//iter++;
	}
	
	if(workerInfos->algoIdentifier & 4) {
		pthread_create(infos->threads[2], NULL, quickThread, data[iter++]);
		//printArray(*(data[iter]->dataArray), data[iter]->size);
		//iter++;
	}
	
	//Join
	if(workerInfos->algoIdentifier & 1) { //Insertion
		pthread_join(*(infos->threads[0]), (void **)retval);
		//printf("Relatório: Thread 0 terminou %s\n", (*retval == EXIT_SUCCESS) ? "com sucesso" : ((*retval == EXIT_FAILURE) ? "com falha" : ((*retval == (int)PTHREAD_CANCELED) ? "cancelada" : "em um estado indefinido")));
	}
	
	if(workerInfos->algoIdentifier & 2) { //Selection
		pthread_join(*(infos->threads[1]), (void **)retval);
		//printf("Relatório: Thread 1 terminou %s\n", (*retval == EXIT_SUCCESS) ? "com sucesso" : ((*retval == EXIT_FAILURE) ? "com falha" : ((*retval == (int)PTHREAD_CANCELED) ? "cancelada" : "em um estado indefinido")));
	}
	
	if(workerInfos->algoIdentifier & 4) { //Bubble
		pthread_join(*(infos->threads[2]),(void **)retval);
		//printf("Relatório: Thread 2 terminou %s\n", (*retval == EXIT_SUCCESS) ? "com sucesso" : ((*retval == EXIT_FAILURE) ? "com falha" : ((*retval == (int)PTHREAD_CANCELED) ? "cancelada" : "em um estado indefinido")));
	}
	
	
	
	/*
	free(SortingInfo ... em todos)!!!
	*/
	return EXIT_SUCCESS;
}

/////////////// TESTES ////////////////////
/**
int main(void) {
	struct st_reporterInfo data;
	pthread_t pClockThread; //Thread relógio
	pthread_t pReportThread; //Thread relatório
	pthread_t pinterceptorThread; //Thread interceptadora
	unsigned long clockTimer = 0; //Relógio compartilhado
	
	//Inicializando a estrutura de dados compartilhada:
	data.clock = &clockTimer;
	data.threadCount = 3;
	struct st_workerInfo
	
	pthread_create(&pClockThread, NULL, clockThread, &clockTimer); //Sempe deve-se iniciar o relógio antes de qualquer outra thread
	pthread_create(&pClockThread, NULL, clockThread, &clockTimer); //Sempe deve-se iniciar o relógio antes de qualquer outra thread

	return 0;
}
//*/
