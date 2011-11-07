#ifndef _TYPEUTILS_H
#define _TYPEUTILS_H

#include <stdbool.h>
#include <stdint.h>
#include "list.h"

typedef const char *const ConstStaticString;
//Dentro de função: Variável(ponteiro) não pode ser modificada, nem valor para qual ela aponta.
//Como tipo de retorno de função: Indica que o retorno não corresponde a uma nova área de memória alocada dentro da função, porém não provem de algo teve valores alterados durante a execução.
//Como parâmetro de função: Indica que a variável não terá os valores para os quais aponta alterados, e não apontará para outra área de memória que não a original.
//Recomendação: Funções que não alteram absolutamente nada na string.

typedef const char *ConstValueString;
//Dentro de função: Variável(ponteiro) pode ser modificada, porém não o valor para qual ela aponta.
//Como tipo de retorno de função: Indica que o retorno pode corresponder a uma nova área de memória alocada dentro da função, porém não provem de algo teve valores alterados durante a execução.
//Como parâmetro de função: Indica que a variável não terá os valores para os quais aponta alterados, porém poderá apontar para outra área de memória que não a original.
//Recomendação: Funções que alocam novas strings, mas não alteram nada nelas.

typedef char *const ConstPointerString;
//Dentro de função: Variável(ponteiro) não pode ser modificada, porém valor para qual ela aponta pode.
//Como tipo de retorno de função: Indica que o retorno não corresponde a uma nova área de memória alocada dentro da função, porém pode provir de algo teve valores alterados durante a execução.
//Como parâmetro de função: Indica que a variável poderá ter os valores para os quais aponta alterados, porém não apontará para outra área de memória que não a original.
//Recomendação: Funções que alteram strings previamente alocadas, sem alterar o seus ponteiros.

typedef char *String;
//Dentro de função: Variável(ponteiro) pode ser modificada, e valor para qual ela aponta também.
//Como tipo de retorno de função: Indica que o retorno pode corresponder a uma nova área de memória alocada dentro da função, e também pode provir de algo teve valores alterados durante a execução.
//Como parâmetro de função: Indica que a variável poderá ter os valores para os quais aponta alterados, e poderá apontar para outra área de memória que não a original.
//Recomendação: Combinadas operações de alteração e alocação de strings.

//Verifica se a string é convertível para int
bool isStrInt(ConstStaticString text, bool acceptNegative);

//Verifica se a string é convertível para float. Aceita inteiros.
bool isStrFloat(ConstStaticString text, bool acceptNegative);

//Verifica se a string é convertível para bool
bool isStrBool(ConstStaticString text);

//Deve ser usada para saber se vale à pena continuar testando outros tipos
bool isObviouslyString(ConstStaticString text);

//Verifica se o caracter pode representar um valor lógico
bool isBoolChar(char flag);

//Testa o tipo do parâmetro e retorna o valor correspondente:
//Tipos: 1 - Int, 2 - Float, 3 - Bool, 4 - String, 0 - Erro
int getType(ConstStaticString string);

//Converte uma string em booleano(newbool), retornando se a conversão foi bem sucedida
bool atob(ConstStaticString booltext, bool *newbool);

//Conta os bits "acesos" de um inteiro
int bitCounter(uint32_t integer);

//Preenche um vetor de 'algoCount' vetores, de um tipo definido por parâmetro, com itens de uma lista concatenada com vírgulas
void fillValuesArray(ConstStaticString concatValues, int algoCount, int **valuesPointers, int type);//int *valuesPointers[algoCount]

//Preenche um vetor de 'algoCount' listas, de um tipo definido por parâmetro, com itens de uma lista concatenada com vírgulas
void fillValuesList(ConstStaticString concatValues, int algoCount, List *valuesPointers, int type);//List valuesPointers[algoCount]

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
ConstStaticString getStringPointerValue(int hiddenPointer);

//Esconde uma string em um int que guarda o seu endereço
int createHiddenStringPointer(ConstStaticString value);
#endif
