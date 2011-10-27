#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include "threadUtils.h"
#include "stringUtils.h"
#include "fileUtils.h"
#include "typeUtils.h"
#include "config.h"
#include "params.h"
#include "list.h"

//#define DEBUG printf("Arquivo: %s, Função: %s, Linha: %d\n", __FILE__, __func__, __LINE__)

struct run_data {
	Param params;
	
	int algoIdentifier; //1(bubble), 2(insertion), 4(selection), 8(merge), 16(heap), 32(quick), 64(radix), 128(bucket)
	int algoType; //1(int), 2(float), 3(string)
	int abortTime; //1 ~ CONFIG_SLEEP
	int orderBefore; //1(asc), 2(desc)
	int randSize; //1 ~ CONFIG_QUOTA
	int randBegin; //INT_MIN ~ INT_MAX, > randEnd
	int randEnd; //INT_MIN ~ INT_MAX, < randBegin
	
	char *fileParams; //Valid filesystem path - For memory optimization purposes, will contain pointer to 'argv' relevant part
	char *inputFile; //Valid filesystem path - For memory optimization purposes, will contain pointer to 'argv' relevant part
	char *gnuplotOutputFile; //Valid filesystem path - For memory optimization purposes, will contain pointer to 'argv' relevant part
	char *extraReportFile; //Valid filesystem path - For memory optimization purposes, will contain pointer to 'argv' relevant part
	
	bool useArrays;
	bool parallelRun;
	
	char *values; //Comma-separated values - For memory optimization purposes, will contain pointer to 'argv' relevant part
	int *valuesPointers[8]; //A int type array for each algorithm - Will be filled and used depending on algoType and useArrays
	List valuesPointersList[6]; //A int type list for each algorithm - Will be filled and used depending on algoType and useArrays
};

//Private
//Inicializador de dados
void initRunData(struct run_data *mainData) {
	int i = 0;
	mainData->params = paramsCreate(11);
	
	mainData->algoIdentifier = 0;
	mainData->algoType = 0;
	mainData->abortTime = 0;
	mainData->orderBefore = 0;
	mainData->randSize = 0;
	mainData->randBegin = 0;
	mainData->randEnd = 0;
	
	mainData->fileParams = NULL;
	mainData->inputFile = NULL;
	mainData->gnuplotOutputFile = NULL;
	mainData->extraReportFile = NULL;
	
	mainData->useArrays = false;
	mainData->parallelRun = false;
	
	mainData->values = NULL;
	while(i < 8) {
		mainData->valuesPointers[i] = NULL;
		i++;
	}
	i = 0;
	while(i < 7) { //Apenas 7 porque radixsort não gera relatórios avançados, devido a limitações técnicas(não é possível usar lista)
		mainData->valuesPointersList[i] = listCreate();
		i++;
	}
}

//Private
//Preenche a estrutura local com valores dos parâmetro já interpretados, e preenchendo os parâmetros não informados com valores padrões.
//Não preenche os vetores ou listas de valores
void populateRunDataUsingParams(struct run_data *mainData, char **defaultStringParams, bool isRecursive) {
	bool flag = false;
	char **aux, *pData;
	int dataSize, index, i = 0, mallocStringCount = 0;
	Param mainParams = mainData->params;
	
	if((flag = paramIsset(mainParams, /* Id: */ 16)) || fileExists(defaultStringParams[0])) { //Id:4 , Param:--fileParams=
		mainData->fileParams = flag ? paramGetData(mainParams, 4) : defaultStringParams[0]; //Este número(4) não é um Id, apenas um índice, previamente planejado para seguir a ordem crescente dos Ids, no vetor de params[]
		
		if((isRecursive == false) && (paramsFileFetch(mainData->fileParams, mainData->params) == OK)) { //Caso o parâmetro não tenha sido setado pelo usuário, mas o arquivo default exista, então ele passa o arquivo default
			populateRunDataUsingParams(mainData, defaultStringParams, true); //Informa que a chama é recursiva para a próxima execução, evitando que fileParams contenha fileParams
		} else {
			fprintf(stderr, "%sImpossível continuar devido aos erros.\n", (isRecursive ? "Chamada recursiva é PROIBIDA.\n" : ""));
			return; //Parando aqui ele não lê os demais parâmetros que não conflitavam com ele
		}
	} else { //Então os parâmetros que poderiam conflitar serão lidos. Apesar da validação já ter ocorrido em paramsCheckConflitcts(), estou evitando 'if's irrelevantes
		aux = (char **) malloc(5 * sizeof(char *));
		if(paramIsset(mainParams, /* Id: */ 1)) { //Id:1 , Param:--algo=
			pData = paramGetData(mainParams, 0); //Este número(0) não é um Id, apenas um índice, previamente planejado para seguir a ordem crescente dos Ids, no vetor de params[]
			dataSize = strlen(pData);
			index = getStringUntil(pData, &aux[0], ',', dataSize); ++mallocStringCount; //algoIdentifier para aux[0] - Depois será necessário liberar a memória alocada pela função getStringUntil()
			index += getStringUntil(&pData[index], &aux[1], ',', dataSize); ++mallocStringCount; //algoType para aux[1] - Depois será necessário liberar a memória alocada pela função getStringUntil()
			mainData->algoIdentifier = paramsSwitchCase(mainParams, /* Id: */ 1, aux[0], 0);
			mainData->algoType = paramsSwitchCase(mainParams, /* Id: */ 1, aux[1], 1);
		} else { //Então use o valor padrão
			mainData->algoIdentifier = ALGOIDENTIFIER;
			mainData->algoType = ALGOTYPE;
		}
	
		if(paramIsset(mainParams, /* Id: */ 2)) { //Id:2 , Param:--abortTime=
			pData = paramGetData(mainParams, 1); //Este número(1) não é um Id, apenas um índice, previamente planejado para seguir a ordem crescente dos Ids, no vetor de params[]
			mainData->abortTime = paramsSwitchCase(mainParams, /* Id: */ 2, pData, 0); //Como este parâmetro não tem dados divididos por vírgula, é possível pegá-lo diretamente pela variável pData
		} else { //Então use o valor padrão
			mainData->abortTime = ABORTTIME;
		}
	
		if(paramIsset(mainParams, /* Id: */ 4)) { //Id:4 , Param:--orderBefore=
			pData = paramGetData(mainParams, 2); //Este número(2) não é um Id, apenas um índice, previamente planejado para seguir a ordem crescente dos Ids, no vetor de params[]
			mainData->orderBefore = paramsSwitchCase(mainParams, /* Id: */ 4, pData, 0); //Como este parâmetro não tem dados divididos por vírgula, é possível pegá-lo diretamente pela variável pData
		} else { //Então use o valor padrão
			mainData->orderBefore = ORDERBEFORE;
		}
	
		if(paramIsset(mainParams, /* Id: */ 8)) { //Id:8 , Param:--rand=
			pData = paramGetData(mainParams, 3); //Este número(3) não é um Id, apenas um índice, previamente planejado para seguir a ordem crescente dos Ids, no vetor de params[]
			dataSize = strlen(pData);
			index = getStringUntil(pData, &aux[2], ',', dataSize); ++mallocStringCount; //algoIdentifier para aux[2] - Depois será necessário liberar a memória alocada pela função getStringUntil()
			index += getStringUntil(&pData[index], &aux[3], ',', dataSize); ++mallocStringCount; //algoType para aux[3] - Depois será necessário liberar a memória alocada pela função getStringUntil()
			index += getStringUntil(&pData[index], &aux[4], ',', dataSize); ++mallocStringCount; //algoType para aux[4] - Depois será necessário liberar a memória alocada pela função getStringUntil()
			mainData->randSize = paramsSwitchCase(mainParams, /* Id: */ 8, aux[2], 0);
			mainData->randBegin = paramsSwitchCase(mainParams, /* Id: */ 8, aux[3], 1);
			mainData->randEnd = paramsSwitchCase(mainParams, /* Id: */ 8, aux[4], 2);
		} else { //Então use o valor padrão
			mainData->randSize = RANDSIZE;
			mainData->randBegin = RANDBEGIN;
			mainData->randEnd = RANDEND;
		}
		
		if(paramIsset(mainParams, /* Id: */ 256)) { //Id:256 , Param:--useArrays=
			pData = paramGetData(mainParams, 8); //Este número(8) não é um Id, apenas um índice, previamente planejado para seguir a ordem crescente dos Ids, no vetor de params[]
			mainData->useArrays = paramsSwitchCase(mainParams, /* Id: */ 256, pData, 0); //Como este parâmetro não tem dados divididos por vírgula, é possível pegá-lo diretamente pela variável pData
		} else { //Então use o valor padrão
			mainData->useArrays = USEARRAYS;
		}
		
		if(paramIsset(mainParams, /* Id: */ 512)) { //Id:512 , Param:--parallelRun=
			pData = paramGetData(mainParams, 9); //Este número(9) não é um Id, apenas um índice, previamente planejado para seguir a ordem crescente dos Ids, no vetor de params[]
			mainData->parallelRun = paramsSwitchCase(mainParams, /* Id: */ 512, pData, 0); //Como este parâmetro não tem dados divididos por vírgula, é possível pegá-lo diretamente pela variável pData
		} else { //Então use o valor padrão
			mainData->parallelRun = PARALLELRUN;
		}
		
		if(paramIsset(mainParams, /* Id: */ 1024)) { //Id:1024 , Param:--values=
			pData = paramGetData(mainParams, 10); //Este número(10) não é um Id, apenas um índice, previamente planejado para seguir a ordem crescente dos Ids, no vetor de params[]
			mainData->values = pData;
		} else { //Então use o valor padrão
			mainData->values = defaultStringParams[4];
		}
	} //Daqui em diante os parâmetros não conflitam com fileParams
	
	if(paramIsset(mainParams, /* Id: */ 32)) { //Id:32 , Param:--inputFile=
		pData = paramGetData(mainParams, 5); //Este número(5) não é um Id, apenas um índice, previamente planejado para seguir a ordem crescente dos Ids, no vetor de params[]
		mainData->inputFile = pData;
	} else { //Então use o valor padrão
		mainData->inputFile = defaultStringParams[1];
	}
	
	if(paramIsset(mainParams, /* Id: */ 64)) { //Id:64 , Param:--gnuplotOutputFile=
		pData = paramGetData(mainParams, 6); //Este número(6) não é um Id, apenas um índice, previamente planejado para seguir a ordem crescente dos Ids, no vetor de params[]
		mainData->gnuplotOutputFile = pData;
	} else { //Então use o valor padrão
		mainData->gnuplotOutputFile = defaultStringParams[2];
	}
	
	if(paramIsset(mainParams, /* Id: */ 128)) { //Id:128 , Param:--extraReportFile=
		pData = paramGetData(mainParams, 7); //Este número(7) não é um Id, apenas um índice, previamente planejado para seguir a ordem crescente dos Ids, no vetor de params[]
		mainData->extraReportFile = pData;
	} else { //Então use o valor padrão
		mainData->extraReportFile = defaultStringParams[3];
	}
	
	while(i < mallocStringCount) {
		free(aux[i]);
	}
	free(aux);
}

/**
//Programa principal
int main(int argc, char *argv[]) {
	struct run_data mainData;
	bool success;//??
	int algoCount;
	char *defaultStringParams[] = {FILEPARAMS, INPUTFILE, GNUPLOTOUTPUTFILE, EXTRAREPORTFILE, VALUES}; //Por não ser recomendável usar macros com string, fixarei a referência na função principal
	initRunData(&mainData);
	
	paramsFetch(argc, argv, mainData.params);
	if((paramsFetch(argc, argv, mainData.params) == ERROR) || (paramsCheckConflitcts(mainData.params, false) == INVALID)) {
		fprintf(stderr, "Impossível continuar devido aos erros.\n");
	} else {
		populateRunDataUsingParams(&mainData, defaultStringParams, false);
		//Checa se foram pedidos valores aleatórios. Caso não, então deve-se utilizar 'values'
		//convertendo a massa de dados de string p/ tipo determinado e inserir nas estruturas definidas
		algoCount = bitCounter(mainData.algoIdentifier);
		if(paramIsset(mainData.params, 8) || (strlen(defaultStringParams[4]) == 0)) { //--rand= , Id: 8
			if(mainData.useArrays == true) {
				fillArrayRand(mainData.valuesPointers, algoCount, mainData.algoType, mainData.randBegin, mainData.randEnd, mainData.randSize);
			} else {
				fillListRand(mainData.valuesPointersList, algoCount, mainData.algoType, mainData.randBegin, mainData.randEnd, mainData.randSize);
			}
		} else {
			if(mainData.useArrays == true) {
				fillValuesArray(mainData.values, algoCount, mainData.valuesPointers, mainData.algoType);
			} else {
				fillValuesList(mainData.values, algoCount, mainData.valuesPointersList, mainData.algoType);
			}
		}
		//Usar parâmetros
		//chamar as threads de vetor ou lista, conforme a definição
		//parar as threads e terminar as atrasadas
		//calcular o tempo
		//gerar relatório com a saida de cada thread
		//limpar as(os) listas/vetores alocadas(os)
		//gerar saida p/ gnuplot: os resultados dependem do tipo de estrutura(vetor ou lista)
	}
	paramsClear(mainData.params);
	return EXIT_SUCCESS;
}
//*/

//Public
int runGetAlgoIdentifier(t_run_data mainData) {	return mainData->algoIdentifier;}

//Public
int runGetAlgoType(t_run_data mainData) {	return mainData->algoType;}

//Public
int runGetAbortTime(t_run_data mainData) {	return mainData->abortTime;}

//Public
int runGetOrderBefore(t_run_data mainData) {	return mainData->orderBefore;}

//Public
int runGetRandSize(t_run_data mainData) {	return mainData->randSize;}

//Public
int runGetRandBegin(t_run_data mainData) {	return mainData->randBegin;}

//Public
int runGetRandEnd(t_run_data mainData) {	return mainData->randEnd;}

//Public
char* runGetFileParams(t_run_data mainData) {	return mainData->fileParams;}

//Public
char* runGetInputFile(t_run_data mainData) {	return mainData->inputFile;}

//Public
char* runGetGnuplotOutputFile(t_run_data mainData) {	return mainData->gnuplotOutputFile;}

//Public
char* runGetExtraReportFile(t_run_data mainData) {	return mainData->extraReportFile;}

//Public
bool runGetUseArrays(t_run_data mainData) {	return mainData->useArrays;}

//Public
bool runGetUseThreads(t_run_data mainData) {	return mainData->parallelRun;}

//Public
char* runGetValues(t_run_data mainData) {	return mainData->values;}

//Public
int** runGetValuesPointers(t_run_data mainData) {	return mainData->valuesPointers;}

//Public
List* runGetValuesPointersList(t_run_data mainData) {	return mainData->valuesPointersList;}

//////////////////// TESTES ////////////////////////////
/**/
//int main(void) {
//	ReporterInfo reportData;
//	InterceptorInfo interceptionData;
//	WorkerInfo workersData;
//	pthread_t pClockThread; //Thread relógio
//	pthread_t pReportThread; //Thread relatório
//	pthread_t pInterceptorThread; //Thread interceptadora
//
//	pthread_t pInsertionThread; //Thread insertion
//	pthread_t pSelectionThread; //Thread selection
//	pthread_t pBubbleThread; //Thread bubble
//
//	int **matrix, i, j;//, mat[3][10] = {{5,3,2,8,9,7,1,6,4,10},{15,3,2,8,9,7,1,6,4,20},{25,3,2,8,9,7,1,6,4,30}};
//	uint64_t *times, t[3] = {UINT64_MAX, UINT64_MAX, UINT64_MAX}; //UINT64_MAX é inalcançável, e então serve como flag
//	int success, runSize = 2147483647/1000;
//	uint64_t clockTimer = 0; //Relógio compartilhado
//
//	pthread_t *workers[8] = {NULL};
//	workers[0] = &pInsertionThread;
//	workers[1] = &pSelectionThread;
//	workers[2] = &pBubbleThread;
//
//	srand(time(NULL));
//	matrix = (int **) malloc(3 * sizeof(int *));
//	times = (uint64_t *) malloc(3 * sizeof(uint64_t));;
//	for(i = 0; i < 3; i++) {
//		times[i] = t[i];
//		matrix[i] = (int *) malloc(runSize * sizeof(int));
//		for(j = 0; j < runSize; j++) {
//			matrix[i][j] = rand() % 65535;//mat[i][j];
//		}
//	}
//
//	workersData = createWorkerInfo((int **)matrix, /* Estou usando vetor */ NULL, times, &clockTimer, /* localmente significa bubble, insertion e selection */ 1+2+4, /* valor fixo de teste */ runSize, /* rodar em paralelo */ true);
//	reportData = createReporterInfo(workers, workersData, true);
//	interceptionData = createInterceptorInfo(workers, times, /* 30 segundos de espera */ 1200);
//
//	pthread_create(&pClockThread, NULL, clockThread, &clockTimer); //Sempe deve-se iniciar o relógio antes de qualquer outra thread
//	pthread_create(&pReportThread, NULL, reportThread, reportData);
//	pthread_create(&pInterceptorThread, NULL, interceptorThread, interceptionData);
//	//Inicializando a estrutura de dados compartilhada:
//
//	//data.clock = &clockTimer;
//	//data.threadCount = 3;
//	//WorkerInfo
//
//
//	success = pthread_join(pReportThread, NULL); //(void **)&success);
//	pthread_cancel(pInterceptorThread);
//	//
//	//free(WorkerInfo ...)
//	//free(ReporterInfo ...)
//	//
//
//	if(success == 0) {
//		int i, j;
//		for(i = 0; i < 3; i++) {
//			printf("Algoritmo %d, tempo: %llu\n", i, times[i]);
//			for(j = 0; j < (runSize <= 100 ? runSize : 100); j++) {
//				printf("%d ", matrix[i][j]);//
//			}
//			printf("\n\n");
//		}
//	} else {
//		printf("Algum erro!");
//	}
//	return 0;
//}
//*/
