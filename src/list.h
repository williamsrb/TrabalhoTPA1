#ifndef __LIST_H
#define __LIST_H

typedef int type;
typedef struct list *List;

List listCreate();

void listClear(List l);

/* insere um elemento na posicao desejada
   caso a posicao seja invalida, insere no fim */
void listInsert(List l, type v, unsigned int pos);

/* retira um elemento de uma posicao */
type listRemove(List l, unsigned int pos);

/* retorna o tamanho da list */
int listSize(List l);

/* retorna o valor de uma posicao da list */
type listValue(List l, unsigned int pos);

/* retorna o indice de um elemento na list
   caso o elemento nao esteja na list, retorna -1 */
int listPos(List l, type v);

//v-v--------------- Ações passivas de log--------------------v-v//

//Troca a posição de dois valores entrei si
void listInterchange(List l, unsigned int pos1, unsigned int pos2);

//Muda o valor numa posição
void listChangeValue(List l, unsigned int pos, int newvalue);

/* retorna o valor de uma posicao da lista */
type listValue(List l, unsigned int pos);

//Compara duas posições da lista
//-1: pos1 < pos2, 0: pos1 = pos2, 1: pos1 > pos2
int listCompareValues(List l, unsigned int pos1, unsigned int pos2);

//Compara uma posição da lista com um valor externo
//-1: pos1 < pos2, 0: pos1 = pos2, 1: pos1 > pos2
int listGivenCompareValues(List l, unsigned int pos, int givenValue);
#endif
