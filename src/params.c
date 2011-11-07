#include <stdio.h>
#include "stringUtils.h"
#include "fileUtils.h"
#include "typeUtils.h"
#include "params.h"
#include "config.h"

struct param {
    int id;
    int conflicts;
    String data;
};

//Aloca 11 parâmetros
Params paramsCreate() {
	int i;
	Params newParams = (Param *) malloc(11 * sizeof(Param));
	while(i < 11) {
		newParams[i] = (Param) malloc(sizeof(struct param));
		newParams[i]->id = 0;
		newParams[i]->conflicts = 0;
		newParams[i]->data = NULL;
		i++;
	}
}

//Verifica se a campo data foi usado, e consequentemente o parêmetro. O uso de valores padrões na função principal não altera o retorno de paramIsset()
bool paramIsset(Params params, int index) {
	return (params[index]->data != NULL) ? true : false;
}

//Interpreta e armazena os parâmetros, assegurando que os ponteiros para dados de parâmetros não utilizados contenham NULL
bool paramsFetch(int mainArgc, char *mainArgv[], Params params) {
	return false; //IMPLEMENTAR
}

//Lê o arquivo de parâmetros e envia para paramsFetch() interpretar e validar, e depois usa a função paramsCheckConflitcts() para checar os conflitos. Se tudo der certo, retorna um sinal de sucesso
bool paramsFileFetch(ConstStaticString filePath, Params params) {
	bool success = true;
	String fileParams;//, **fileArgv;
	int count, size, i = 0, pos = 0, get;
	get = size = sizeOfFile(filePath, false);
	if(size != 0) {
		fileParams = (String) malloc(size);
		readFileLine(fileParams, filePath);
		count = chrCount(fileParams, ' ') + 1; //quantidade de separadores + 1 é igual à quantidade de elementos
		count++; //o primeiro item do argv é o nome do executável
		
		//String *fileArgv = (String *) malloc((count) * sizeof(String));
		String fileArgv[count];
		for(i = 1; (i < count) && (get != 0); i++) {
			get = getStringUntil(&fileParams[pos], &fileArgv[i], ' ', size);
			pos += get;
		}
		if((paramsFetch(count, fileArgv, params) == ERROR) || (paramsCheckConflitcts(params, true) == INVALID)) {
			success = false;
		}
		
		//Para simular o argv real terei que abrir mão de desalocar as strings encontradas
		//for(i = 0; i < count; i++) {
		//	free(fileArgv[i]);
		//}
	} else {
		success = false;
	}
	return success;
}

//Verifica se há conflitos entre os parâmetros, e se permitido exibe os erros na saída apropriada
bool paramsCheckConflitcts(Params params, bool displayErrors) {
	return false; //IMPLEMENTAR
}

//Limpa a lista de parâmetros
void paramsClear(Params params) {
	int i;
	while(i < 11) {
		free(params[i]->data);
		free(params[i]);
		i++;
	}
	free(params);
	return;
}

//Retorna um flag relevante para a aplicação principal, que depende do valor do parâmetro, do id e da posição do parâmetro na lista separada por virgulas de onde veio, se for o caso
int paramsSwitchCase(Params params, int paramId, ConstStaticString simplifiedData, int dataPos) {
	return 0; //IMPLEMENTAR
}

//Retorna o id de um dado parâmetro
int paramGetId(Params params, int index) {
	return params[index]->id;
}

//Retorna a soma de possíveis conflitos com um dado parâmetro
int paramGetConflict(Params params, int index) {
	return params[index]->conflicts;
}

//Retorna os dados de um dado parâmetro
ConstStaticString paramGetData(Params params, int index) {
	return params[index]->data;
}

//Atribui um novo valor ao id de um dado parâmetro
void paramSetId(Params params, int index, int newId) {
	params[index]->id = newId;
}

//Atribui um novo valor, em um dado parâmetro, representando a soma de possíveis conflitos com outros parâmetros
void paramSetConflict(Params params, int index, int newConflitctSum) {
	params[index]->conflicts = newConflitctSum;
}

//Atribui um novo valor à string de dados de um dado parâmetro
void paramSetData(Params params, int index, ConstStaticString newData) {
	params[index]->data = (String)newData;
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
512 --parallelRun=[true|false] - Each algoritm runs in a separeted thread.
1024 --values=[comma_separated_values_list] -- Accept the same type specified in 'algo'.
*/


//////// TESTES ////////
/**
int main(int argc, char *argv[]) {
	String arr[6];
	int i;
	paramsFileFetch("params.txt", NULL, arr);
	for(i = 1; i < 6; i++) {
		printf("LOL: %s\n", arr[i]);
	}
    return 0;
}
//*/
