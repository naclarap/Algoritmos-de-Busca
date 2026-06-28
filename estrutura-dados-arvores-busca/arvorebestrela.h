#ifndef ARVOREBESTRELA_H
#define ARVOREBESTRELA_H

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "utilidades.h"

//Definicoes para a arvore B estrela
#define MM_E 2
#define MM_E2 2*MM_E

typedef long TipoChave;

typedef tipoItem TipoRegistro;

typedef enum {Interna, Externa} TipoIntExt;

typedef struct TipoPaginaE* TipoApontadorE;

typedef struct TipoPaginaE {
    TipoIntExt Pt;
    union {
        struct {
            int ni;
            TipoChave ri[MM_E2];
            TipoApontadorE pi[MM_E2 + 1];
        } U0;
        struct {
            int ne;
            TipoRegistro re[MM_E2];
        } U1;
    } UU;
} TipoPaginaE;

bool ArvoreBEstrela(FILE *arquivo, int quantidade, int chave, bool print, tipoItem *item, Estatisticas *controleProces, Estatisticas *controlePesq);
void inicializaArvore (TipoApontadorE * Arvore);
void LoadArquivoABE(int qtd,FILE *arquivo, TipoApontadorE *arvore);
void PesquisaABE(int chave, TipoRegistro *x, TipoApontadorE *Ap);
void InsereNaPaginaExterna(TipoApontadorE Ap, TipoRegistro Reg);
void InsereNaPaginaInterna(TipoApontadorE Ap, TipoChave Reg, TipoApontadorE ApDir);
void InsABE(TipoRegistro Reg, TipoApontadorE Ap, short *cresceu, TipoChave *RegRetorno, TipoApontadorE *ApRetorno);
void InsereABE(TipoRegistro Reg, TipoApontadorE *Ap);
void liberaArvoreBE(TipoApontadorE Arvore);
void ImprimeRegistrosABE(TipoApontadorE arvore);

#endif