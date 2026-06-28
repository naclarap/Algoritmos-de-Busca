#ifndef ARVORE_H
#define ARVOREB_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "item.h"
#include "utilidades.h"

// Ordem da árvore (número mínimo de registros por página)
#define M 20
#define MM M * 2

// Apontador para as páginas
typedef struct TipoPagina* TipoApontador;

// Estrutura para cada página (nodo) da árvore
typedef struct TipoPagina {
    short n; // número de chaves por páginas
    tipoItem regs[MM];		// vetor de chaves do nó
    TipoApontador pont[MM + 1];		// vetor para cada subárvore
} TipoPagina;

bool ArvoreB(FILE *arquivo, int quantidade, int chave, bool print, tipoItem *item, Estatisticas *controleProces, Estatisticas *controlePesq);
void LoadArquivo(FILE *arquivo, int quantidade, TipoApontador *arvore);
void Inicializa(TipoApontador *arvore);
void PesquisaArvoreB(int chave, tipoItem *registro, TipoApontador Ap);
void Insere(tipoItem Reg, TipoApontador *Ap);
void LiberaArvore(TipoApontador arvore);
void ImprimeRegistros(TipoApontador arvore);

#endif