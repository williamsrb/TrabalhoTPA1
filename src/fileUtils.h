#ifndef _FILEUTILS_H
#define _FILEUTILS_H

#include <stdbool.h>
#include <stdlib.h>
//Lê uma linha(Unix style) de um arquivo e copia para a string informada
bool readFileLine(char *text, const char *filename);

//Escreve uma string, seguida de um pulo de linha(Unix style), em um arquivo
bool writeFileLine(const char *text, const char *filename);

//Escreve uma sequências de bytes no arquivo especificado
bool writeBinFile(const void *datastruct, size_t typesize, size_t count, const char *filename);

//Lê uma sequências de bytes do arquivo especificado
//Cuidados: Deve ser usado em conjunto com 'sizeOfBinFile'
bool readBinFile(void *datastruct, size_t typesize, size_t filesize, const char *filename);

//Calcula o tamanho de um arquivo texto ou binário
long sizeOfFile(char *filename, bool isBinary);

//Verifica se um arquivo existe
bool fileExists(const char * filename);
#endif
