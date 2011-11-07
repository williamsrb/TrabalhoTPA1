#ifndef _CONFIG_H
#define _CONFIG_H

//AVISO
//Considerando que os dados daqui são do domínio do analista, não serão validados no programa

#include <stdbool.h>
#include "list.h"
#include "typeUtils.h"

typedef struct run_data *t_run_data;

//Domínio do analista - Início

//Configurações importantes, altere com cuidado
#define VALID false //Relevante p/ paramsCheckConflitcts
#define INVALID true //Relevante p/ paramsCheckConflitcts
#define OK false //Relevante p/ paramsFetch e paramsFileFetch
#define ERROR true //Relevante p/ paramsFetch e paramsFileFetch
//AVISO: 2.147.483.647 corresponde a INT_MAX. Não ultrapasse 2.147.483.647/100 se você não tiver pelo menos 2GB de memória.
#define CONFIG_QUOTA 65536 //ESTE É O LIMITE DE TAMANHO DA MASSA DE DADOS, não é tamanho padrão, é apenas um limite p/ o usuário
#define CONFIG_SLEEPLIMIT 5*60 //5 minutos é demais, não acha? Não é tempo padrão, é apenas um limite p/ o usuário

//Valores default dos parâmetros
//AVISO: Para quantidades grandes jamais use o Bubble. Para quantidades perto de INT_MAX não use algoritmos recursivos ou que usam espaços extras para ordenação(radix e bucket)
#define ALGOIDENTIFIER 1+2+4+8+16+32+128 //Todos exceto radix
#define ALGOTYPE 1 //Inteiro
#define ABORTTIME 30 //em segundos
#define ORDERBEFORE 0 //deixando 0 ele não pré-ordena
#define RANDSIZE 1024 //tamanho padrão da massa de dados aleatória, só usado caso VALUES esteja vazio
#define RANDBEGIN 0 //deixar números positivos - begin sempre tem que ser menor que end, só usado caso VALUES esteja vazio
#define RANDEND 65536 //quero evitar números repetidos - end sempre tem que ser maior que begin, só usado caso VALUES esteja vazio
#define FILEPARAMS ("params.txt") //lerá automaticamente este arquivo caso exista, a não ser que o usuário informe outro caminho
#define INPUTFILE ("in.txt") //lerá automaticamente este arquivo caso exista, a não ser que o usuário informe outro caminho
#define GNUPLOTOUTPUTFILE ("gnuplot_report.dat") //gerará automaticamente este arquivo ou sobreescreve-lo-á, caso exista, a não ser que o usuário informe outro caminho
#define EXTRAREPORTFILE ("report.txt") //gerará automaticamente este arquivo ou sobreescreve-lo-á, caso exista, a não ser que o usuário informe outro caminho
#define USEARRAYS false //não usar arrays é o mesmo que habilitar relatórios avançados, uma vez que acessando através de funções é mais fácil acompanhar as operações
//AVISO: Para garantir que nenhuma thread seja "assassinada" use o bom senso ao combinar essa propriedade com o tamanho da massa de dados
#define PARALLELRUN true //quis usar threads para não perder tempo, ocupando o máximo possível o processador
#define VALUES ("5,3,2,8,9,7,1,6,4,10") //Precisa ser do mesmo tipo que ALGOTYPE, e só é usado se ambos não forem informados pelo usuário

//Domínio do analista - Fim

//Funções públicas
int runGetAlgoIdentifier(t_run_data mainData);

int runGetAlgoType(t_run_data mainData);

int runGetAbortTime(t_run_data mainData);

int runGetOrderBefore(t_run_data mainData);

int runGetRandSize(t_run_data mainData);

int runGetRandBegin(t_run_data mainData);

int runGetRandEnd(t_run_data mainData);

ConstStaticString runGetFileParams(t_run_data mainData);

ConstStaticString runGetInputFile(t_run_data mainData);

ConstStaticString runGetGnuplotOutputFile(t_run_data mainData);

ConstStaticString runGetExtraReportFile(t_run_data mainData);

bool runGetUseArrays(t_run_data mainData);

bool runGetUseThreads(t_run_data mainData);

ConstStaticString runGetValues(t_run_data mainData);

int** runGetValuesPointers(t_run_data mainData);

List* runGetValuesPointersList(t_run_data mainData);

#endif


