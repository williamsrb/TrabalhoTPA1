#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "list.h"

typedef struct node *Node;
struct node{
	Node next, prev;
	type value;	
};

struct list{
	Node root;
	int size;
	//Report:
	bool logEnabled;
	int acCount;
	int cmpCount;
	int mvCount;
	bool mvIcomp;
	int (*cmpFunc)(int,int);
};

List listCreate()
{
	List l = malloc(sizeof(struct list));
	l->root = NULL;
	l->size = 0;
	//Report
	l->logEnabled = false; //Flag que informa se o log deve ser realizado ou não
	l->acCount = 0; //Contador de acessos individuais
	l->cmpCount = 0; //Contador de comparações
	l->mvCount = 0; //Contador de movimentos completos (pos1 <==> pos2)
	l->mvIcomp = false; //Flag de movimentos incompletos (pos1 <= newvalor)
	l->cmpFunc = NULL; //Função externa de comparação entre valores
	return l;
}

void listSetCompareFunction(List l, int (*cmpFunc)(int,int)) {
	l->cmpFunc = cmpFunc;
}

void listClear(List l)
{
	while(listSize(l)>0)
		listRemove(l, 0);
	free(l);
}

/* retorna o tamanho da lista */
int listSize(List l)
{
	return l->size;
}

/* insere um elemento na posicao desejada
   caso a posicao seja invalida, insere no fim */
void listInsert(List l, type value, unsigned int pos)
{
	int i;
	Node aux, new = malloc(sizeof(struct node));
	new->value = value;
	aux = l->root;
	if(l->root == NULL)
	{
		l->root = new;
		new->next = new->prev = new;
	}
	else
	{
		if(pos < l->size)
		{
			for(i=0; i<pos; i++)
				aux=aux->next;
		}
		new->next = aux;
		new->prev = aux->prev;
		new->prev->next = new;
		new->next->prev = new;
		if(pos == 0)
			l->root = l->root->prev;
	}
	l->size++;
}

/* retira um elemento de uma posicao */
type listRemove(List l, unsigned int pos)
{
	int i;
	type value;
	Node aux = l->root;
	if(pos >= listSize(l))
		exit(-1);
	for(i=0; i < pos; i++)
		aux = aux->next;
	value = aux->value;
	if(pos == 0)
	{
		if(l->size == 1)
			l->root = NULL;
		else
			l->root = l->root->next;
	}
	aux->next->prev = aux->prev;
	aux->prev->next = aux->next;
	free(aux);
	l->size--;
	return value;
}

/* retorna o indice de um elemento na lista
   caso o elemento nao esteja na lista, retorna -1 */
int listPos(List l, type value)
{
	int i;
	Node aux = l->root;
	for(i=0; aux->value != value && i < l->size; i++)
		aux = aux->next;
	if(i == l->size)
		return -1;
	return i;
}

//v-v--------------- Ações passivas de log--------------------v-v//

//Troca a posição de dois valores entrei si
void listInterchange(List l, unsigned int pos1, unsigned int pos2) {
	int i, auxv;
	Node aux1 = l->root;
	Node aux2 = l->root;
	if(pos1 >= listSize(l) || pos2 >= listSize(l))
		exit(-1);
	for(i=0; i < pos1; i++)
		aux1 = aux1->next;
	for(i=0; i < pos2; i++)
		aux2 = aux2->next;
	auxv = aux1->value;
	aux1->value = aux2->value;
	aux2->value = auxv;
	if(l->logEnabled) {
		l->acCount = l->acCount + 2;
		l->mvCount++;
	}
}

//Muda o valor numa posição
void listChangeValue(List l, unsigned int pos, int newvalue)
{
	int i;
	Node aux = l->root;
	if(pos >= listSize(l))
		exit(-1);
	for(i=0; i < pos; i++)
		aux = aux->next;
	aux->value = newvalue;
	if(l->logEnabled) {
		l->acCount++;
		if(l->mvIcomp == true) {
			l->mvCount++;
			l->mvIcomp = false;
		} else {
			l->mvIcomp = true;
		}
	}
}

/* retorna o valor de uma posicao da lista */
type listValue(List l, unsigned int pos)
{
	int i;
	Node aux = l->root;
	if(pos >= listSize(l))
		exit(-1);
	for(i=0; i < pos; i++)
		aux = aux->next;
	if(l->logEnabled) {
		l->acCount++;
	}
	return aux->value;
}

//Compara duas posições da lista
//-1: pos1 < pos2, 0: pos1 = pos2, 1: pos1 > pos2
int listCompareValues(List l, unsigned int pos1, unsigned int pos2) {
	if(l->logEnabled) {
		l->cmpCount++;
	}
	return l->cmpFunc(listValue(l, pos1), listValue(l, pos2));
}

//Compara uma posição da lista com um valor externo
//-1: pos1 < pos2, 0: pos1 = pos2, 1: pos1 > pos2
int listGivenCompareValues(List l, unsigned int pos, int givenValue) {
	if(l->logEnabled) {
		l->cmpCount++;
	}
	return l->cmpFunc(listValue(l, pos), givenValue);
}
