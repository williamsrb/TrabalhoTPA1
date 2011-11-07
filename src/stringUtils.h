#ifndef _STRINGUTILS_H
#define _STRINGUTILS_H

#include <stdbool.h>
#include <stdint.h>
#include "typeUtils.h"
//Testa o tipo do caracter. Se for um dígito retorna true
bool isDigit(char digit);

//Testa se uma string inicia com uma dada chave
bool startsWith(ConstStaticString text, ConstStaticString key);

//Inicializa um vetor de strings com NULLs em cada posição
void nullStringArray(ConstValueString string[], int size);

//Desaloca e limpa um vetor de strings
void freeStringArray(ConstValueString string[], int size);

//Altera a string original, transformando letras minúsculas em maiúsculas.
//Cuidados: Usar caracteres ASCII simples. Não usar com constantes.
void strToUpper(ConstPointerString text);

//Altera a string original, transformando letras maiúsculas em minúsculas.
//Cuidados: Usar caracteres ASCII simples. Não usar com constantes.
void strToLower(ConstPointerString text);

//Retorna a posição da primeira ocorrência de uma chave em um texto
int strPos(ConstStaticString text, ConstStaticString key);

//Preenche uma string com um dado caracter de ponto a outro
void fillWith(ConstPointerString text, char filler, int start, int end);

//Usa strncpy para substituir um trecho de uma string por outro, preechendo as diferenças de tamanho com espaços
//AVISO: strncpy preenche as diferenças de espaços com '\0', por isso o uso de espaços para sanar o problema
bool strReplace(ConstPointerString text, ConstStaticString oldkey, ConstStaticString newkey);

//Substitui um caracter por outro dentro de uma string
bool chrReplace(ConstPointerString text, char oldkey, char newkey);

//Conta as ocorrências de um determinado caracter numa string
int chrCount(ConstStaticString text, char key);

//Lê uma sequência de caracteres de uma string, a partir de certo ponto, e para de ler na primeira ocorrência de um dado caracter, então retorna o tamanho da nova string
//Cuidados: Aloca uma nova string, então lembre-se de desalocá-la quado não for mais usar
//int getStringUntil(const char *text, char **foundString, int begin, char stopkey, int defaultEnd); //Versão bugada
int getStringUntil(ConstStaticString text, String *resultstring, char searchkey, int maxlen);

//Gera uma string aleatória, de tamanho também aleatório, dado o tamanho mínimo e máximo
String strRand(uint8_t min, uint8_t max, uint8_t seed);
#endif
