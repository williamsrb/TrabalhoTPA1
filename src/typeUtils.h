#ifndef _TYPEUTILS_H
#define _TYPEUTILS_H

#include <stdbool.h>
#include <stdint.h>
#include "list.h"
//Verifica se a string é convertível para int
bool isStrInt(const char *text, bool acceptNegative);

//Verifica se a string é convertível para float. Aceita inteiros.
bool isStrFloat(const char *text, bool acceptNegative);

//Verifica se a string é convertível para bool
bool isStrBool(const char *text);

//Deve ser usada para saber se vale à pena continuar testando outros tipos
bool isObviouslyString(const char *text);

//Verifica se o caracter pode representar um valor lógico
bool isBoolChar(char flag);

//Testa o tipo do parâmetro e retorna o valor correspondente:
//Tipos: 1 - Int, 2 - Float, 3 - Bool, 4 - String, 0 - Erro
int getType(char *string);

//Converte uma string em booleano(newbool), retornando se a conversão foi bem sucedida
bool atob(const char *booltext, bool *newbool);

//Conta os bits "acesos" de um inteiro
int bitCounter(uint32_t integer);

//Preenche um vetor de 'algoCount' vetores, de um tipo definido por parâmetro, com itens de uma lista concatenada com vírgulas
void fillValuesArray(char *concatValues, int algoCount, int **valuesPointers, int type);//int *valuesPointers[algoCount]

//Preenche um vetor de 'algoCount' listas, de um tipo definido por parâmetro, com itens de uma lista concatenada com vírgulas
void fillValuesList(char *concatValues, int algoCount, List *valuesPointers, int type);//List valuesPointers[algoCount]

//Preenche um vetor de 'algoCount' vetores, de um tipo definido por parâmetro, com itens gerados aleatoriamente, dados o intervalo e a quantidade de valores a serem gerados
void fillArrayRand(int **valuesPointers, int algoCount, int type, int begin, int end, int size);

//Preenche um vetor de 'algoCount' listas, de um tipo definido por parâmetro, com itens gerados aleatoriamente, dados o intervalo e a quantidade de valores a serem gerados
void fillListRand(List *valuesPointers, int algoCount, int type, int begin, int end, int size);

//Gera um float aleatório, a partir de um inteiro num intervalo definido, dado uma semente
//Cuidados: Usar srand() antes
float floatRand(int begin, int end, float seed);

//Gera um int aleatório, num intervalo definido, dado uma semente
//Cuidados: Usar srand() antes
int intRand(int begin, int end, int seed);

//Transforma um ponteiro escondido num int em um int verdadeiro, que é o valor apontado
int* getIntPointerValue(int hiddenPointer);

//Esconde um int em um int que guarda o seu endereço
int createHiddenIntPointer(int *value);

//Transforma um ponteiro escondido num int em um float, que é o valor apontado
float* getFloatPointerValue(int hiddenPointer);

//Esconde um float em um int que guarda o seu endereço
int createHiddenFloatPointer(float *value);

//Transforma um ponteiro escondido num int em uma string, que é o valor apontado
char* getStringPointerValue(int hiddenPointer);

//Esconde uma string em um int que guarda o seu endereço
int createHiddenStringPointer(char *value);
#endif
