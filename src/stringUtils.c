#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include "stringUtils.h"
#include <stdio.h>

//Testa o tipo do caracter. Se for um dígito retorna true
bool isDigit(char digit) {
	return ((digit > 47) && (digit < 58)); //48 a 57 são o valores ascii para os digitos 1 a 9
}

//Testa se uma string inicia com uma dada chave
bool startsWith(ConstStaticString text, ConstStaticString key) {
	char *mark = strstr(text, key);
	bool result = false;
	if(mark != NULL) {
		result = (text == mark);
	}
	return result;
}

//Inicializa um vetor de strings com NULLs em cada posição
void nullStringArray(ConstValueString string[], int size) {
	int count = 0;
	while(count < size) {
		string[count] = NULL;
		count++;
	}
}

//Desaloca e limpa um vetor de strings
void freeStringArray(ConstValueString string[], int size) {
	int count = 0;
	while(count < size) {
		free((void *)string[count]);
		string[count] = NULL;
		count++;
	}
}

//Altera a string original, transformando letras minúsculas em maiúsculas.
//Cuidados: Usar caracteres ASCII simples. Não usar com constantes.
void strToUpper(ConstPointerString text) {
	int count = 0;
	char step = 'a' - 'A';
	if(text != NULL) {
		while(text[count] != '\0') {
			text[count] -= (text[count] > 96 && text[count] < 123 ) ? step : 0;
			count++;
		}
	}
}

//Altera a string original, transformando letras maiúsculas em minúsculas.
//Cuidados: Usar caracteres ASCII simples. Não usar com constantes.
void strToLower(ConstPointerString text) {
	int count = 0;
	char step = 'a' - 'A';
	if(text != NULL) {
		while(text[count] != '\0') {
			text[count] += (text[count] > 64 && text[count] < 91 ) ? step : 0;
			count++;
		}
	}
}

//Retorna a posição da primeira ocorrência de uma chave em um texto
int strPos(ConstStaticString text, ConstStaticString key) {
	ConstStaticString found = strstr(text, key);
	if(found != NULL)
		return found - text;
	return -1; //Não encontrado
}

//Preenche uma string com um dado caracter de ponto a outro
void fillWith(ConstPointerString text, char filler, int start, int end) {
	while(start < end) {
		text[start] = filler;
		start++;
	}
	text[start] = '\0';
}

//Usa strncpy para substituir um trecho de uma string por outro, preechendo as diferenças de tamanho com espaços
//AVISO: strncpy preenche as diferenças de espaços com '\0', por isso o uso de espaços para sanar o problema
bool strReplace(ConstPointerString text, ConstStaticString oldkey, ConstStaticString newkey) {
 	String found, filler;
 	int oldkeysize, newkeysize, textsize, count;
 	bool result;
 	
 	oldkeysize = strlen(oldkey);
 	newkeysize = strlen(newkey);
 	textsize = strlen(text);
 	found = strstr(text, oldkey);
 	if(found == NULL) {
 		result = false;
 	} else {
 		if(oldkeysize > newkeysize) {
 			filler = (String) malloc(oldkeysize + 1);
 			strcpy(filler, newkey);
 			fillWith(filler, ' ', newkeysize, oldkeysize); //Preenche a diferença entre as duas strings com espaço
 			strncpy(found, filler, oldkeysize);
 			free(filler);
 		} else {
 			strncpy(found, newkey, oldkeysize);
 		}
 		result = true;
 	}
 	return result;
}

//Substitui um caracter por outro dentro de uma string
bool chrReplace(ConstPointerString text, char oldkey, char newkey) {
 	bool result;
 	int size, count = 0;
 	size = strlen(text);
 	while(count < size) {
 		if(text[count] == oldkey) {
 			text[count] = newkey;
 		}
 		count++;
 	}
 	return result;
}

//Conta as ocorrências de um determinado caracter numa string
int chrCount(ConstStaticString text, char key) {
 	int size, iter = 0, count = 0;
 	size = strlen(text);
 	while(iter < size) {
 		if(text[iter] == key) {
 			count++;
 		}
 		iter++;
 	}
 	return count;
}

//Lê uma sequência de caracteres de uma string, a partir de certo ponto, e para de ler na primeira ocorrência de um dado caracter, então retorna o tamanho da nova string
//Cuidados: Aloca uma nova string, então lembre-se de desalocá-la quado não for mais usar
int getStringUntil(ConstStaticString text, String *resultstring, char searchkey, int maxlen) {
	int i = 0;

//	 Obs: quando encontra o '\0' aloca 1 byte a mais do que o necessário
//	 Se isso for problema, a correção é fácil, só não acho que compense
	while (i < maxlen && text[i] != '\0' && text[i] != searchkey)
		i++;

	if(*resultstring = (String) malloc(i + 1)) {
		strncpy(*resultstring, text, i);
		(*resultstring)[i] = '\0';
		return i+1;
	}

	return 0;
}

//Gera uma string aleatória, de tamanho também aleatório, dado o tamanho mínimo e máximo
//Cuidados: Aloca uma nova string, então lembre-se de desalocá-la quado não for mais usar
#define nrand(n) ((int)(((int64_t)(rand() + seed)) % RAND_MAX)) //nrand() considera o seed na geração de um aleatório
String strRand(uint8_t min, uint8_t max, uint8_t seed) {
	char *newString, loopChar;
	int diff = max - min, size, i;
	short type;
	
	if(diff < 0) {
		newString = NULL;
	} else {
		size = nrand() % (diff+1) + min;
		newString = (char *) malloc(size + 1);
		for(i = 0; i < size; i++) {
			type = nrand() % 3;
			switch(type) {
				case 0:
					//number
					loopChar = nrand() % 10 + 48; //'0' a '9'
					break;
				case 1:
					//upper
					loopChar = nrand() % 26 + 65; //'A' a 'Z'
					break;
				case 2:
					//lower
					loopChar = nrand() % 26 + 97; //'a' a 'b'
					break;
				//É possível adicionar outras faixas de caracteres
			}
			newString[i] = loopChar;
		}
		newString[i] = '\0';
	}
	return newString;
}

//////// TESTES ////////
/**/
//int main(int argc, char *argv[]) {
//	char vetor[] = "I like to move it.This is not my fault.I'm saying the truth.I can't believe in another thing.Another else these one";
//	char *str[5];
//	int i, tam;
//
//	index = getStringUntil(vetor, &achados[0], 0, '.', strlen(vetor));
//	printf("%d -> %s\n", index, achados[0]);
//
//	if(index != 0) {
//		index = getStringUntil(vetor, &achados[1], index+1, '.', strlen(vetor));
//		printf("%d -> %s\n", index, achados[1]);
//		free(achados[2]);
//	}
//
//	if(index != 0) {
//		index = getStringUntil(vetor, &achados[2], index+1, '.', strlen(vetor));
//		printf("%d -> %s\n", index, achados[2]);
//		free(achados[3]);
//	}
//
//	char *newStr;
//	srand((unsigned)time(NULL));
//	newStr = strRand(3, 10, i);
//	printf("%s", newStr);
//	for(i = 1; i < 10; i++) {
//		newStr = strRand(3, 10, i);
//		printf(", %s", newStr);
//	}
//	putchar('\n');
//
//	printf("Strlen: %d\n",tam = strlen(vetor));
//	printf ("%d ", i = getStringUntil(vetor, &str[0], '.', tam));
//	printf ("%d ", i += getStringUntil(&vetor[i], &str[1], '.', tam));
//	printf ("%d ", i += getStringUntil(&vetor[i], &str[2], '.', tam));
//	printf ("%d ", i += getStringUntil(&vetor[i], &str[3], '.', tam));
//	printf ("%d ", i += getStringUntil(&vetor[i], &str[4], '.', tam));
//	printf ("\n%s [l] %s [l] %s [l] %s [l] %s\n", str[0], str[1], str[2], str[3], str[4]);
//    return 0;
//}
//*/

/*
//Versão bugada
//Lê uma sequência de caracteres de uma string, a partir de certo ponto, e para de ler na primeira ocorrência de um dado caracter, então retorna a posição onde encontrou o tal caracter
//Cuidados: Aloca uma nova string, então lembre-se de desalocá-la quado não for mais usar
int getStringUntil(const char *text, char **foundString, int begin, char stopkey, int defaultEnd) {
	int count = 0;
	bool found = false;
	while((count < defaultEnd) && (found == false)) {
		if(text[count] == stopkey) {
			found = true;
		}
		count++;
	}
	if(found == true) {
		*foundString = (char *) malloc(count+1);
		if(foundString != NULL) {
			strncpy(*foundString, text+begin, count);
		}
		(*foundString)[count] = '\0';
	} else {
		count = 0;
	}
	return count-1;
}
*/
