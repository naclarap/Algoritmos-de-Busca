#ifndef UTILIDADES_H
#define UTILIDADES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "item.h"

// Estatística de teste de préprocessamento e pesquisa
typedef struct{
    int tranferencias;
    int comparacoes;
    clock_t tempoI;
    clock_t tempoF;
} Estatisticas;

bool verificaArgumentos(int argc, char *argv[]);
FILE* abreArquivo(int situacao);
void imprimeRegistro(tipoItem item);
void ImprimeControle(Estatisticas controle);
void iniciaControle(Estatisticas *controle);

#endif