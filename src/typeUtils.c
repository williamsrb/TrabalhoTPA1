#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "config.h"
#include "typeUtils.h"
#include "stringUtils.h"
#include "list.h"

//Verifica se a string é convertível para int
bool isStrInt(ConstStaticString text, bool acceptNegative) {
	bool result = true;
	int size, count = 0;
	
	size = (text != NULL) ? strlen(text) : 0;
	if(size < 1) {
		result = false;
	} else {
		if((acceptNegative == true) && (size > 1) && (text[0] == '-')) { //Possui sinal negativo no início
			count++;
		}
		while((count < size) && (result != false)) {
			result = isDigit(text[count]);
			count++;
		}
	}
	return result;
}

//Verifica se a string é convertível para float. Aceita inteiros.
bool isStrFloat(ConstStaticString text, bool acceptNegative) {
	bool result = true;
	int size, count = 0;
	bool dotFound = false;
	
	size = (text != NULL) ? strlen(text) : 0;
	if(size < 1) {
		result = false;
	} else {
		if(size > 1){
			if((acceptNegative == true) && (text[0] == '-')) { //Possui sinal negativo no início
				count++;
			}
			result = isDigit(text[count]); //Obrigatoriamente o primeiro caracter, depois do teste anterior, tem que ser um número
		}
		while((count < size-1) && (result != false) && (dotFound == false)) { //Sair antes de testar a última posição
			if((text[count] == '.')) {
				dotFound = true;
			} else if(!isDigit(text[count])) {
				result = false;
			}
			count++;
		}
		result = isDigit(text[count]); //Obrigatoriamente o próximo caracter, depois do loop anterior, tem que ser um número
		while((count < size) && (result != false)) {
			result = isDigit(text[count]);
			count++;
		}
	}
	return result;
}

//Verifica se a string é convertível para bool
bool isStrBool(ConstStaticString text) {
	bool result;
	int size, count = 0;
	String copy_FREEIT = NULL;
	
	size = (text != NULL) ? strlen(text) : 0;
	if(size < 1) {
		result = false;
	} else {
		if(size == 1) {
			result = isBoolChar(text[count]); //Se tiver um caracter, este terá que representar um valor lógico
		} else {
			copy_FREEIT = (String) malloc(size + 1); //Espaço para o '\0'
			strcpy(copy_FREEIT, text);
			strToUpper(copy_FREEIT);
			result = ((strcmp(copy_FREEIT, "TRUE") == 0) || (strcmp(copy_FREEIT, "FALSE") == 0) || (strcmp(copy_FREEIT, "YES") == 0) || (strcmp(copy_FREEIT, "NO") == 0) || (strcmp(copy_FREEIT, "SIM") == 0) || (strcmp(copy_FREEIT, "NAO") == 0) || (strcmp(copy_FREEIT, "VERDADEIRO") == 0) || (strcmp(copy_FREEIT, "FALSO") == 0)); //Não usarei NÃO como chave pois é difícil tratar isso em C
			free(copy_FREEIT);
		}
	}
	return result;
}

//Deve ser usada para saber se vale à pena continuar testando outros tipos
bool isObviouslyString(ConstStaticString text) {
	int size;
	
	size = (text != NULL) ? strlen(text) : 0;
	return !((size > 0) && (isDigit(text[0]) || (text[0] == '-') || isBoolChar(text[0])));
}

//Verifica se o caracter pode representar um valor lógico
bool isBoolChar(char flag) {
	return ((flag == 'S') || (flag == 's') || (flag == 'Y') || (flag == 'y') || (flag == 'N') || (flag == 'n') || (flag == 'V') || (flag == 'v') || (flag == 'T') || (flag == 't') || (flag == 'F') || (flag == 'f'));
}


//Testa o tipo do parâmetro e retorna o valor correspondente:
//Tipos: 1 - Int, 2 - Float, 3 - Bool, 4 - String, 0 - Erro
int getType(ConstStaticString string) {
	int result = 0;
	if(string != NULL) {
		if(!isObviouslyString(string)) {
			if(isStrInt(string, true)) {
				result = 1;
			} else if(isStrFloat(string, true)) {
				result = 2;
			} else if(isStrBool(string)) {
				result = 3;
			} else {
				result = 4;
			}
		} else {
			result = 4;
		}
	}
	return result;
}

//Converte uma string em booleano(newbool), retornando se a conversão foi bem sucedida
bool atob(ConstStaticString booltext, bool *newbool) {
	bool result = true;
	String copy_FREEIT;
	
	copy_FREEIT = (char*) malloc(strlen(booltext) + 1); //Espaço para o '\0'
	strcpy(copy_FREEIT, booltext);
	strToUpper(copy_FREEIT);
	if((strcmp(copy_FREEIT, "T") == 0) || (strcmp(copy_FREEIT, "Y") == 0) || (strcmp(copy_FREEIT, "S") == 0) || (strcmp(copy_FREEIT, "V") == 0) || (strcmp(copy_FREEIT, "TRUE") == 0) || (strcmp(copy_FREEIT, "YES") == 0) || (strcmp(copy_FREEIT, "SIM") == 0) || (strcmp(copy_FREEIT, "VERDADEIRO") == 0)) {
		*newbool = true;
	} else if((strcmp(copy_FREEIT, "F") == 0) || (strcmp(copy_FREEIT, "N") == 0) || (strcmp(copy_FREEIT, "FALSE") == 0) || (strcmp(copy_FREEIT, "NO") == 0) || (strcmp(copy_FREEIT, "NAO") == 0) || (strcmp(copy_FREEIT, "FALSO") == 0)) {
		*newbool = false;
	} else {
		result = false;
	}
	free(copy_FREEIT);
	return result;
}

int bitCounter(unsigned int integer) {
	int comparer = 1, counter = 0;
	if(integer != 0) {
		do {
			counter += ((integer & comparer) ? 1 : 0);
			comparer <<= 1;
		} while(comparer <= integer);
	}
	return counter;
}

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

//Função genérica para preenchimento de um vetor de valores de um dado tipo, a partir de uma lista concatenada separada por vírgulas.
//Visível localmente apenas(static)
static void fillValuesPointers(ConstStaticString concatValues, int algoCount, int **valuesPointersArray, List *valuesPointersList, int type) {
	int *newInt, localInt, i = 0, j = 0, pos = 0, get, arrsize = chrCount(concatValues, ',') + 1, strsize = strlen(concatValues);
	float *newFloat, localFloat;
	String varchar, newString;
	bool success = true;
	get = strsize;
	if((strsize >= 1) && ((type == 1) || (type == 2) || (type == 3)) && ((valuesPointersArray != NULL) || (valuesPointersList != NULL)) && (concatValues != NULL) && (algoCount > 0) && (algoCount < 9)) {
		if(valuesPointersArray != NULL) {
			for(j = 0; j < algoCount; j++) {
				valuesPointersArray[j] = (int *) malloc(arrsize * sizeof(int)); //Cada vetor alocado para cad algoritmo ganha um vetor de inteiros, que conterão endereços para valores qualquer tipo
			}
		}
		
		for(j = 0; (j < arrsize) && (get != 0) && (success != false); j++) {
			get = getStringUntil(&concatValues[pos], &varchar, ',', strsize);
			pos += get;
			if(pos != 0) {
				switch(type) {
					case 1: //Int
						if(isStrInt(varchar, true)) { //Os dados já foram validados, mas deve-se conferir para desencargo de consciência
							localInt = atoi(varchar);
							for(i = 0; i < algoCount; i++) {
								newInt = (int *) malloc(sizeof(int));
								*newInt = localInt;
								if(valuesPointersArray != NULL) {
									valuesPointersArray[i][j] = createHiddenIntPointer(newInt);
								} else {
									listInsert(valuesPointersList[i], createHiddenIntPointer(newInt), j);
								}
							}
						} else {
							success = false;
						}
						free(varchar);
						break;
					case 2: //Float
						if(isStrFloat(varchar, true)) { //Os dados já foram validados, mas deve-se conferir para desencargo de consciência
							localFloat = atof(varchar);
							for(i = 0; i < algoCount; i++) {
								newFloat = (float *) malloc(sizeof(float));
								*newFloat = localFloat;
								if(valuesPointersArray != NULL) {
									valuesPointersArray[i][j] = createHiddenFloatPointer(newFloat);
								} else {
									listInsert(valuesPointersList[i], createHiddenFloatPointer(newFloat), j);
								}
							}
						} else {
							success = false;
						}
						free(varchar);
						break;
					case 3: //String
						if(strlen(varchar) > 0) { //Os dados já foram validados, mas deve-se conferir para desencargo de consciência
							for(i = 0; i < algoCount; i++) {
								newString = (String) malloc(get + 1);
								strcpy(newString, varchar);
								if(valuesPointersArray != NULL) {
									valuesPointersArray[i][j] = createHiddenStringPointer(newString);
								} else {
									listInsert(valuesPointersList[i], createHiddenStringPointer(newString), j);
								}
							}
						} else {
							success = false;
						}
						free(varchar);
						break;
					default:
						success = false;
						free(varchar);
						break;
				}
			}
		}
	} else {
		success = false;
	}
	if(success == false) {
		printf("Ocorreu um erro importante demais para ser ignorado: Era esperado um tipo de massa de dados num tamanho mínimo, o que não ocorreu.\nVerifique o tamanho e o tipo da massa de dados.\nEncontrei: tamanho \"%d caracteres\" e tipo \"%s\"\n", strsize, ((type == 1) ? "int" : ((type == 2) ? "float" : ((type == 3) ? "string" : "indefinido"))));
		exit(EXIT_FAILURE);
	}
}

//Preenche um vetor de 'algoCount' vetores, de um tipo definido por parâmetro, com itens de uma lista concatenada com vírgulas. Se quiser usar lista basta enviar o campo array como NULL e usar uma lista válida em "valuesPointersList".
//Cuidados: Aloca um novo vetor de um dado tipo, então lembre-se de desalocá-los quado não for mais usar. Utilizar uma lista não validada pode causa um comportamento inesperado na função.
void fillValuesArray(ConstStaticString concatValues, int algoCount, int **valuesPointers, int type) {
	fillValuesPointers(concatValues, algoCount, valuesPointers, NULL, type);
}

//Preenche um vetor de 'algoCount' listas, de um tipo definido por parâmetro, com itens de uma lista concatenada com vírgulas
//Cuidados: Aloca uma nova lista de ponteiros e um vetor de um dado tipo, então lembre-se de desalocá-los quado não for mais usar
void fillValuesList(ConstStaticString concatValues, int algoCount, List *valuesPointers, int type) {
	fillValuesPointers(concatValues, algoCount, NULL, valuesPointers, type);
}

//Função genérica para preenchimento de um vetor de valores de um dado tipo, a partir de uma lista concatenada separada por vírgulas.
//Visível localmente apenas(static)
static void fillPointersRand(int **valuesPointersArray, List *valuesPointersList, int algoCount, int type, int begin, int end, int size) {
	int *newInt, localInt, i = 0, j = 0, stringSize;
	float *newFloat, localFloat;
	String newString, localString;
	bool success = true;
	if(((type == 1) || (type == 2) || (type == 3)) && (begin <= end) && (size <= CONFIG_QUOTA) && ((valuesPointersArray != NULL) || (valuesPointersList != NULL)) && (algoCount > 0) && (algoCount < 9)) {
		if(valuesPointersArray != NULL) {
			for(j = 0; j < algoCount; j++) {
				valuesPointersArray[j] = (int *) malloc(size * sizeof(int)); //Cada vetor alocado para cad algoritmo ganha um vetor de inteiros, que conterão endereços para valores qualquer tipo
			}
		}
		
		for(j = 0; j < size; j++) {
			switch(type) {
				case 1: //Int
					localInt = intRand(begin, end, j);
					for(i = 0; i < algoCount; i++) {
						newInt = (int *) malloc(sizeof(int));
						*newInt = localInt;
						if(valuesPointersArray != NULL) {
							valuesPointersArray[i][j] = createHiddenIntPointer(newInt);
						} else {
							listInsert(valuesPointersList[i], createHiddenIntPointer(newInt), j);
						}
					}
					break;
				case 2: //Float
					localFloat = floatRand(begin, end, (1.0/(j+1)));
					for(i = 0; i < algoCount; i++) {
						newFloat = (float *) malloc(sizeof(float));
						*newFloat = localFloat;
						if(valuesPointersArray != NULL) {
							valuesPointersArray[i][j] = createHiddenFloatPointer(newFloat);
						} else {
							listInsert(valuesPointersList[i], createHiddenFloatPointer(newFloat), j);
						}
					}
					break;
				case 3: //String
					localString = strRand((uint8_t) begin, (uint8_t) end, (uint8_t) j);
					stringSize = strlen(localString);
					for(i = 0; i < algoCount; i++) {
						newString = (String) malloc(stringSize + 1);
						strcpy(newString, localString);
						if(valuesPointersArray != NULL) {
							valuesPointersArray[i][j] = createHiddenStringPointer(newString);
						} else {
							listInsert(valuesPointersList[i], createHiddenStringPointer(newString), j);
						}
					}
					free(localString);
					break;
			}
		}
	} else {
		success = false;
	}
	if(success == false) {
		printf("Ocorreu um erro importante demais para ser ignorado.\nPode ser útil: tamanho \"%d caracteres\" e tipo \"%s\"\n", size, ((type == 1) ? "int" : ((type == 2) ? "float" : ((type == 3) ? "string" : "indefinido"))));
		exit(EXIT_FAILURE);
	}
}

//Preenche um vetor de 'algoCount' vetores, de um tipo definido por parâmetro, com itens gerados aleatoriamente, dados o intervalo e a quantidade de valores a serem gerados
//Cuidados: Usar srand((unsigned)time(NULL)) na aplicação principal.
void fillArrayRand(int **valuesPointers, int algoCount, int type, int begin, int end, int size) {
	fillPointersRand(valuesPointers, NULL, algoCount, type, begin, end, size);
}

//Preenche um vetor de 'algoCount' listas, de um tipo definido por parâmetro, com itens gerados aleatoriamente, dados o intervalo e a quantidade de valores a serem gerados
//Cuidados: Usar srand((unsigned)time(NULL)) na aplicação principal.
void fillListRand(List *valuesPointers, int algoCount, int type, int begin, int end, int size) {
	fillPointersRand(NULL, valuesPointers, algoCount, type, begin, end, size);
}

//Gera um int aleatório, num intervalo definido, dado uma semente
//Cuidados: Usar srand() antes
#define nrand(n) ((int)(((int64_t)(rand() + seed)) % RAND_MAX))
int intRand(int begin, int end, int seed) {
	return (nrand() % (end-begin+1) + begin);
}

//Gera um float aleatório, a partir de um inteiro num intervalo definido, dado uma semente
//Cuidados: Usar srand() antes
#define drand(n) ((float)(((long double)(rand() + seed)) / RAND_MAX)) //(float)rand()/(float)RAND_MAX;
float floatRand(int begin, int end, float seed) {
	int diff = end - begin;
	return begin + drand() * diff;
}

//Transforma um ponteiro escondido num int em um int verdadeiro, que é o valor apontado
int* getIntPointerValue(int hiddenPointer) {
	return (int *) hiddenPointer;
}

//Esconde um int em um int que guarda o seu endereço
int createHiddenIntPointer(int *value) {
	return (int) value;
}

//Transforma um ponteiro escondido num int em um float, que é o valor apontado
float* getFloatPointerValue(int hiddenPointer) {
	return (float *) hiddenPointer;
}

//Esconde um float em um int que guarda o seu endereço
int createHiddenFloatPointer(float *value) {
	return (int) value;
}

//Transforma um ponteiro escondido num int em uma string, que é o valor apontado
ConstStaticString getStringPointerValue(int hiddenPointer) {
	return (ConstStaticString) hiddenPointer;
}

//Esconde uma string em um int que guarda o seu endereço
int createHiddenStringPointer(ConstStaticString value) {
	return (int) value;
}


///////////////////////// TESTES ////////////////////////////////
/**
int main() {
	
//	int *valuesPointersArray[8], i, j, *localInt;
//	float *localFloat;
//	List valuesPointersList[8];
//	char *myStr = "Lorem,ipsum,dolor,sit,amet,consectetur,adipisicing,elit,sed,do,eiusmod", *localString;
//	srand((unsigned)time(NULL));
//
//	for(i = 0; i < 8; i++) {
//		valuesPointersArray[i] = NULL;
//	}
//
//	for(i = 0; i < 8; i++) {
//		valuesPointersList[i] = listCreate();
//	}
//
//	fillValuesArray(myStr, 8, valuesPointersArray, 3);
//
//	printf("Array:\n");
//	for(i = 0; i < 8; i++) {
//		for(j = 0; j < 11; j++) {
//			localString = getStringPointerValue(valuesPointersArray[i][j]);
//			printf("[%d][%d]: %s\n", i, j, localString);
//		}
//	}
//
//	////////////////////////////////////////////////////////////
//
//	fillValuesList(myStr, 8, valuesPointersList, 3);
//
//	printf("List:\n");
//	for(i = 0; i < 8; i++) {
//		for(j = 0; j < 11; j++) {
//			localString = getStringPointerValue(listValue(valuesPointersList[i], j));
//			printf("[%d][%d]: %s\n", i, j, localString);
//		}
//	}
//
//	//---------------------------------------------------------------//
//
//	fillArrayRand(valuesPointersArray, 8, 3, 4, 10, 11);
//
//	printf("Array:\n");
//	for(i = 0; i < 8; i++) {
//		for(j = 0; j < 11; j++) {
//			localString = getStringPointerValue(valuesPointersArray[i][j]);
//			printf("[%d][%d]: %s\n", i, j, localString);
//		}
//	}
//
//	////////////////////////////////////////////////////////////
//
//	fillListRand(valuesPointersList, 8, 3, 4, 10, 11);
//
//	printf("List:\n");
//	for(i = 0; i < 8; i++) {
//		for(j = 0; j < 11; j++) {
//			localString = getStringPointerValue(listValue(valuesPointersList[i], j));
//			printf("[%d][%d]: %s\n", i, j, localString);
//		}
//	}
	
}
//*/
