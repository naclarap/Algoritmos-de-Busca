#ifndef ACESSOSEQUENCIAL_H
#define ACESSOSEQUENCIAL_H

#include "item.h"
#include "utilidades.h"
#include <stdio.h>
#include <stdbool.h>

#define ITENSPAGINA 10
#define MAXTABELA 200000

//funcao que faz a pesquisa
bool AcessoSequencial(FILE* arquivo, int quantidade, int chave, bool print, tipoItem *item, Estatisticas *controleProces, Estatisticas *controlePesq);
void geraTabela(int tabelaIndices[], FILE* arquivo, int quantidade, Estatisticas *controleProces);
bool pesquisaSequencial(int tabelaIndice[], FILE* arquivo, int quantidade, int tamArquivo, tipoItem *item, Estatisticas *controlePesq);
void imprimeSequencial(FILE* arquivo, int quantidade);

#endif
