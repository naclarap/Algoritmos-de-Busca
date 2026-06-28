#ifndef ABP_H
#define ABP_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "item.h"
#include "utilidades.h"

typedef struct {
    tipoItem item;
    long esq;  // filho esquerdo
    long dir;  // filho direito
} tipoNo;

bool ArvoreBinaria(FILE *arq, int quantidade, int chave, bool print, tipoItem *item, Estatisticas *controleProces, Estatisticas *controlePesq);
void insereNaArvore(FILE *arqAbp, tipoItem itemInserir);
void constroiArvore(FILE *arq, FILE *arqAbp, int quantidade);
bool pesquisarAbp(FILE *arqAbp, int chave, tipoItem *item);
void imprimeAbp(FILE *arqAbp, long ponteiro);


#endif
