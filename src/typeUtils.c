#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "typeUtils.h"
#include "stringUtils.h"

#include <stdio.h>

//Verifica se a string é convertível para int
bool isStrInt(const char *text, bool acceptNegative) {
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
bool isStrFloat(const char *text, bool acceptNegative) {
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
bool isStrBool(const char *text) {
	bool result;
	int size, count = 0;
	char *copy_FREEIT = NULL;
	
	size = (text != NULL) ? strlen(text) : 0;
	if(size < 1) {
		result = false;
	} else {
		if(size == 1) {
			result = isBoolChar(text[count]); //Se tiver um caracter, este terá que representar um valor lógico
		} else {
			copy_FREEIT = (char*) malloc(size + 1); //Espaço para o '\0'
			strcpy(copy_FREEIT, text);
			strToUpper(copy_FREEIT);
			result = ((strcmp(copy_FREEIT, "TRUE") == 0) || (strcmp(copy_FREEIT, "FALSE") == 0) || (strcmp(copy_FREEIT, "YES") == 0) || (strcmp(copy_FREEIT, "NO") == 0) || (strcmp(copy_FREEIT, "SIM") == 0) || (strcmp(copy_FREEIT, "NAO") == 0) || (strcmp(copy_FREEIT, "VERDADEIRO") == 0) || (strcmp(copy_FREEIT, "FALSO") == 0)); //Não usarei NÃO como chave pois é difícil tratar isso em C
			free(copy_FREEIT);
		}
	}
	return result;
}

//Deve ser usada para saber se vale à pena continuar testando outros tipos
bool isObviouslyString(const char *text) {
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
int getType(char *string) {
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
bool atob(const char *booltext, bool *newbool) {
	bool result = true;
	char *copy_FREEIT;
	
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

//Preenche um vetor de 'algoCount' vetores, de um tipo definido por parâmetro, com itens de uma lista concatenada com vírgulas
bool fillValuesArray(char *concatValues, int algoCount, int **valuesPointers, int type) {
	return false; //IMPLEMENTAR
}

//Preenche um vetor de 'algoCount' listas, de um tipo definido por parâmetro, com itens de uma lista concatenada com vírgulas
bool fillValuesList(char *concatValues, int algoCount, List *valuesPointers, int type) {
	return false; //IMPLEMENTAR
}

//Preenche um vetor de 'algoCount' vetores, de um tipo definido por parâmetro, com itens gerados aleatoriamente, dados o intervalo e a quantidade de valores a serem gerados
bool fillArrayRand(int **valuesPointers, int algoCount, int type, int begin, int end, int size) {
	return false; //IMPLEMENTAR
}

//Preenche um vetor de 'algoCount' listas, de um tipo definido por parâmetro, com itens gerados aleatoriamente, dados o intervalo e a quantidade de valores a serem gerados
bool fillListRand(List *valuesPointers, int algoCount, int type, int begin, int end, int size) {
	return false; //IMPLEMENTAR
}

//Gera um float aleatório, a partir de um inteiro num intervalo definido, dado uma semente
//Cuidados: Usar srand() antes
float floatRand(int begin, int end, float seed) {
	return 0.0; //IMPLEMENTAR
}

//Gera um int aleatório, num intervalo definido, dado uma semente
//Cuidados: Usar srand() antes
int intRand(int begin, int end, float seed) {
	return 0.0; //IMPLEMENTAR
}

//Transforma um ponteiro escondido num int em um int verdadeiro, que é o valor apontado
int getIntPointedValue(int hiddenPointer) {
	return 0; //IMPLEMENTAR
}

//Esconde um int em um int que guarda o seu endereço
int createHiddenIntPointer(int value) {
	return 0; //IMPLEMENTAR
}

//Transforma um ponteiro escondido num int em um float, que é o valor apontado
float getFloatPointedValue(int hiddenPointer) {
	return 0.0; //IMPLEMENTAR
}

//Esconde um float em um int que guarda o seu endereço
int createHiddenFloatPointer(float value) {
	return 0.0; //IMPLEMENTAR
}

//Transforma um ponteiro escondido num int em uma string, que é o valor apontado
char *getStringPointedValue(int hiddenPointer) {
	return NULL; //IMPLEMENTAR
}

//Esconde uma string em um int que guarda o seu endereço
int createHiddenStringPointer(char *value) {
	return 0; //IMPLEMENTAR
}


///////////////////////// TESTES ////////////////////////////////
/**
int main() {
	int i;
	for(i = 0; i < 65536; i++) {
		printf("%d: %dbits\n", i, bitCounter(i));
	}
}
//*/
