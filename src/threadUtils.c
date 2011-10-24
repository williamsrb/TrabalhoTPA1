#include <time.h>
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "typeUtils.h"
#include "threadUtils.h"

//#define DEBUG printf("Arquivo: %s, Função: %s, Linha: %d\n", __FILE__, __func__, __LINE__)

//------------- Estruturas usadas pelas threads para troca de informação ----------------//
struct st_sortingInfo {
	int **dataArray; //Array de dados usado por 1 thread trabalhadora
	uint32_t size; //Tamanho do array.
	uint32_t *time; //Tempo de execução da thread
	uint32_t *clock; //Ponteiro para o relógio
};

struct st_workerInfo {
	int **dataArrays; //Arrays de dados usados pelas threads
	uint32_t *times; //Usado pela mãe para contabilizar o tempo das treads filhas
	uint32_t *clock; //Ponteiro para o relógio
	int algoIdentifier; //Identificador dos tipos de algoritmos a serem executados. É possível também descobrir a quantidade de arrays
	uint32_t size; //Tamanho dos arrays. Tamanho único
};

struct st_reporterInfo {
	pthread_t **threads; //Vetor de ponteiros p/ threads trabalhadoras
	WorkerInfo workersData; //Dados para passar para as threads trabalhadoras
};

struct st_interceptorInfo {
	pthread_t **threads; //Vetor de ponteiros p/ threads trabalhadoras
	uint32_t *times; //Tempo de execução das threads - Serve de flag para cancelar as threads
	uint32_t waiting; //Tempo que a thread espera antes de matar as trabalhadoras
};


//------------- Funções de uso geral -------------------------------//
SortingInfo createStructSortingInfo(int **dataArray, uint32_t size, uint32_t *time, uint32_t *clock) {
	//int *dataArrays; //Array de dados usado por 1 thread trabalhadora
	//int size; //Tamanho do array.
	//struct st_sortingInfo
	return NULL; //IMPLEMENTAR
}

WorkerInfo createStructWorkerInfo(int **dataArrays, uint32_t *times, uint32_t *clock, int algoIdentifier, uint32_t size) {
	//int **dataArrays; //Arrays de dados usados pelas threads
	//int algoIdentifier; //Identificador dos tipos de algoritmos a serem executados. É possível também descobrir a quantidade de arrays
	//int size; //Tamanho dos arrays. Tamanho único
	//st_workerInfo
	WorkerInfo newWorker = (WorkerInfo) malloc(sizeof(struct st_workerInfo));
	newWorker->dataArrays = dataArrays;
	newWorker->times = times;
	newWorker->clock = clock;
	newWorker->algoIdentifier = algoIdentifier;
	newWorker->size = size;
	return newWorker;
}

ReporterInfo createReporterInfo(pthread_t **threads, WorkerInfo workersData) {
	//pthread_t *threads; //Threads trabalhadoras
	//WorkerInfo workersData; //Dados para passar para as threads trabalhadoras
	//long *times; //Usado pel mãe para contabilizar o tempo das treads filhas
	//unsigned long *clock; //Ponteiro para o relógio
	//int threadCount; //Quantidade de threads a serem criadas
	//st_reporterInfo
	ReporterInfo newReporter = (ReporterInfo) malloc(sizeof(struct st_reporterInfo));
	newReporter->threads = threads;
	newReporter->workersData = workersData;
	return newReporter;
}

InterceptorInfo createInterceptorInfo(pthread_t **threads, uint32_t *times, uint32_t waiting) {
	//pthread_t **threads; //Vetor de ponteiros p/ threads trabalhadoras
	//time_t waiting; //Tempo que a thread espera antes de matar as trabalhadoras
	//typedef struct st_interceptorInfo
	return NULL; //IMPLEMENTAR
}

//---------------- Funções usadas pelas threads ------------------------//
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






void printArray(int arr[], int sz) {
	int i;
	if((arr != NULL) || (sz < 1)) {
		printf("Vetor:\n%d", arr[0]);
		for(i = 1; i < sz; i++) {
			printf(", %d", arr[i]);
		}
		printf("\n");
	} else {
		printf("Erro imprimindo array\n");
	}
}

void *clockThread(void *arg) {
	while(1) {
		(*((uint32_t *)arg))++;
		msleep(1); //Dorme 1 milissegundo
	}
}

//Uma thread que dorme(sleep) e depois mata(cancel) todas as outras threads que não pararam, exceto a MAE. Então finaliza.
void* interceptorThread(void *arg) {
	
}

//Uma thread que espera(join) todas as demais para terminar, mesmo se elas foram eternas, para então finalizar.
void *reportThread(void *arg) {
	ReporterInfo infos = (ReporterInfo)arg;
	WorkerInfo workerInfos = infos->workersData;
	SortingInfo *data;
	int iter, algoCount = bitCounter((uint32_t)workerInfos->algoIdentifier);
	
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
	if(workerInfos->algoIdentifier & 1) { //Insertion
		pthread_create(infos->threads[0], NULL, insertionThread, data[iter++]);
		//printArray(*(data[iter]->dataArray), data[iter]->size);
		//iter++;
	}
	
	if(workerInfos->algoIdentifier & 2) { //Selection
		pthread_create(infos->threads[1], NULL, selectionThread, data[iter++]);
		//printArray(*(data[iter]->dataArray), data[iter]->size);
		//iter++;
	}
	
	if(workerInfos->algoIdentifier & 4) { //Bubble
		pthread_create(infos->threads[2], NULL, bubbleThread, data[iter++]);
		//printArray(*(data[iter]->dataArray), data[iter]->size);
		//iter++;
	}
	
	//Join
	if(workerInfos->algoIdentifier & 1) { //Insertion
		pthread_join(*(infos->threads[0]), NULL);
	}
	
	if(workerInfos->algoIdentifier & 2) { //Selection
		pthread_join(*(infos->threads[1]), NULL);
	}
	
	if(workerInfos->algoIdentifier & 4) { //Bubble
		pthread_join(*(infos->threads[2]),NULL);
	}
	
	
	
	/*
	struct st_sortingInfo {
	int *dataArrays; //Array de dados usado por 1 thread trabalhadora
	int size; //Tamanho do array.
	*/
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
