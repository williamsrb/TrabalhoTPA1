#include "stringUtils.h"
#include "fileUtils.h"
#include "typeUtils.h"
#include "params.h"
#include "config.h"

struct param {
    int id;
    int conflicts;
    char *data;
};

//Cria a quantidade de parâmetros desejados
Param paramsCreate(int count) {
	return NULL; //IMPLEMENTAR
}

//Verifica se a campo data foi usado, e consequentemente o parêmetro. O uso de valores padrões na função principal não altera o retorno de paramIsset()
bool paramIsset(Param params, int index) {
	return false; //IMPLEMENTAR
}

//Interpreta e armazena os parâmetros, assegurando que os ponteiros para dados de parâmetros não utilizados contenham NULL
bool paramsFetch(int mainArgc, char *mainArgv[], Param params) {
	return false; //IMPLEMENTAR
}

//Lê o arquivo de parâmetros e envia para paramsFetch() interpretar e validar, e depois usa a função paramsCheckConflitcts() para checar os conflitos. Se tudo der certo, retorna um sinal de sucesso
bool paramsFileFetch(char *filePath, Param params) {
	return false; //IMPLEMENTAR
}

//Verifica se há conflitos entre os parâmetros, e se permitido exibe os erros na saída apropriada
bool paramsCheckConflitcts(Param params, bool displayErrors) {
	return false; //IMPLEMENTAR
}

//Limpa a lista de parâmetros
void paramsClear(Param params) {
	return; //IMPLEMENTAR
}

//Retorna um flag relevante para a aplicação principal, que depende do valor do parâmetro, do id e da posição do parâmetro na lista separada por virgulas de onde veio, se for o caso
int paramsSwitchCase(Param params, int paramId, char *simplifiedData, int dataPos) {
	return 0; //IMPLEMENTAR
}

//Retorna o id de um dado parâmetro
int paramGetId(Param params, int index) {
	return 0; //IMPLEMENTAR
}

//Retorna a soma de possíveis conflitos com um dado parâmetro
int paramGetConflict(Param params, int index) {
	return 0; //IMPLEMENTAR
}

//Retorna os dados de um dado parâmetro
char* paramGetData(Param params, int index) {
	return NULL; //IMPLEMENTAR
}

//Atribui um novo valor ao id de um dado parâmetro
void paramSetId(Param params, int index, int newId) {
	return; //IMPLEMENTAR
}

//Atribui um novo valor, em um dado parâmetro, representando a soma de possíveis conflitos com outros parâmetros
void paramSetConflict(Param params, int index, int newConflitctSum) {
	return; //IMPLEMENTAR
}

//Atribui um novo valor à string de dados de um dado parâmetro
void paramSetData(Param params, int index, char *newData) {
	return; //IMPLEMENTAR
}

//'Param' é um vetor de 'struct param's
//Para acessar os elementos de 'struct param' é necessário usar funçõs deste arquivo

/*
OBS.: Square and round brackets are unnecessary
1 --algo=[algorithm_identifier_sum(int),type(int|float|string)] -- Identifier must be > 0 and a binary sum: 1(bubble) + 2(insertion) + 4(selection) + 8(merge) + 16(heap) + 32(quick) + 64(radix) + 128(bucket).
2 --abortTime=[max_seconds] -- Max time to main program to wait for each algorithm response. If using threads, the latecomers will be termineted and ignored.
4 --orderBefore=[asc|desc] -- Force to order data before run algorithms.
8 --rand=[size(int),begin(int),end(int)] -- Using float type forces random numbers to be multiplied by '(float)rand()/(float)RAND_MAX'. Using string type forces begin and end to be interpreted as min and max string size
16 --fileParams=[full_path_to_file] -- Ignores all other parameters, except '*File' parameters. Must come first, otherwise will be ignored. The pointed file may not containt a path to itself.
32 --inputFile=[full_path_to_file] -- Mutually exclusive with 'values'. File not found == error.
64 --gnuplotOutputFile=[full_path_to_file] -- If file is not writable, redirects output to stdout and send error message to stderr.
128 --extraReportFile=[full_path_to_file] – Data that is not 'gnuplotable', but can be usefull.
256 --useArrays=[true|false] – Uses arrays instead of linked lists.
512 --useThreads=[true|false] - Each algoritm runs in a separeted thread.
1024 --values=[comma_separated_values_list] -- Accept the same type specified in 'algo'.
*/


//////// TESTES ////////
/**
int main(int argc, char *argv[]) {
    return 0;
}
//*/
