#ifndef _PARAMS_H
#define _PARAMS_H

#include <stdbool.h>
typedef struct param *Param;
typedef Param *Params; //A aplicação e este programa esperam que seja um vetor de 11 posições

//Aloca 11 parâmetros
Params paramsCreate();

//Verifica se a campo data foi usado, e consequentemente o parêmetro. O uso de valores padrões na função principal não altera o retorno de paramIsset()
bool paramIsset(Params params, int index);

//Retorna o id de um dado parâmetro
int paramGetId(Params params, int index);

//Retorna a soma de possíveis conflitos com um dado parâmetro
int paramGetConflict(Params params, int index);

//Retorna os dados de um dado parâmetro
ConstStaticString paramGetData(Params params, int index);

//Interpreta e armazena os parâmetros, assegurando que os ponteiros para dados de parâmetros não utilizados contenham NULL
bool paramsFetch(int mainArgc, char *mainArgv[], Params params);

//Lê o arquivo de parâmetros e envia para paramsFetch() interpretar e validar, e depois usa a função paramsCheckConflitcts() para checar os conflitos. Se tudo der certo, retorna um sinal de sucesso
bool paramsFileFetch(ConstStaticString filePath, Params params);

//Verifica se há conflitos entre os parâmetros, e se permitido exibe os erros na saída apropriada
bool paramsCheckConflitcts(Params params, bool displayErrors);

//Limpa a lista de parâmetros
void paramsClear(Params params);

//Retorna um flag relevante para a aplicação principal, que depende do valor do parâmetro, do id e da posição do parâmetro na lista separada por virgulas de onde veio, se for o caso
int paramsSwitchCase(Params params, int paramId, ConstStaticString simplifiedData, int dataPos);
#endif
