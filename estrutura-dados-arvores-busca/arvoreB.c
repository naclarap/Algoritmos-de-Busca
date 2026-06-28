#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "arvoreB.h"

int transferenciasAB = 0, comparacoesAB = 0;

bool ArvoreB(FILE *arquivo, int quantidade, int chave, bool print, tipoItem *item, Estatisticas *controleProces, Estatisticas *controlePesq){
	// Inicializa nossa árvore B
	TipoApontador arvore;
	Inicializa(&arvore);

	// Carrega n items do arquivo e insere na árvore
	controleProces->tempoI = clock();
	LoadArquivo(arquivo, quantidade, &arvore);
	controleProces->tempoF = clock();

	// Variaveis de processamento
	controleProces->tranferencias = transferenciasAB;
	controleProces->comparacoes = comparacoesAB;

	// Se possui argumento [-P], imprime todas as chaves da árvore
    if (print){
        printf("\nChaves do arquivo:\n");
		ImprimeRegistros(arvore);
        printf("\n");
    }  

	transferenciasAB = 0;
	comparacoesAB = 0;

	// Realiza a pesquisa do item na árvore
	controlePesq->tempoI = clock();
	PesquisaArvoreB(chave, item, arvore);
	controlePesq->tempoF = clock();

	// Variaveis de pesquisa
	controlePesq->tranferencias = transferenciasAB;
	controlePesq->comparacoes = comparacoesAB;

	// Libera a mémória alocada pela árvore
	LiberaArvore(arvore);

	// Se a chave for encontrada, retorna verdadeiro
	if (item->chave != chave)
		return false;

	return true;
}

void LoadArquivo(FILE *arquivo, int quantidade, TipoApontador *arvore){
	tipoItem reg;
	int cont = 0;

	// Le cada registro individualmente, carrega na memória e adiciona na árvore
	transferenciasAB++;
	while (fread(&reg, sizeof(tipoItem), 1, arquivo) == 1 && cont < quantidade){
		transferenciasAB++;
		
		Insere(reg, arvore);
		cont++;
	}

}

void Inicializa (TipoApontador *arvore){
    *arvore = NULL;
}

void PesquisaArvoreB(int chave, tipoItem *registro, TipoApontador Ap){
	// Caso a árvore vazia ou chege no nó folha, apontador nulo
	if (Ap == NULL){
		return;
	}

	// Percorre a página enquanto registro for menor que o item atual
	long i = 1;
	while (i < Ap->n && chave > Ap->regs[i - 1].chave){
		i++;
		comparacoesAB++;
	}

	// Se a posição atual for o registro procurado, retorna por referência
	comparacoesAB++;
	if (chave == Ap->regs[i - 1].chave){
		*registro = Ap->regs[i - 1];
		return;
	}

	// Se o registro procurado for menor que atual, segue o ponteiro da esquerda
	comparacoesAB++;
	if (chave < Ap->regs[i - 1].chave){
		PesquisaArvoreB(chave, registro, Ap->pont[i - 1]);
	}
	// Se maior, segue o ponteiro da direita
	else{
		PesquisaArvoreB(chave, registro, Ap->pont[i]);
	}
}

void InsereNaPagina(TipoApontador Ap, tipoItem Reg, TipoApontador ApDir){
	int cont = Ap->n;

	// Encontra a posição correta dentro da página para inserção
	while (cont > 0){
		if (Reg.chave >= Ap->regs[cont - 1].chave){	// Se o registro for maior que o atual, sai do loop e insere no final
			comparacoesAB++;
			break;
		}

		// Empurra o registro e seus nó filho 1 indice para frente
		Ap->regs[cont] = Ap->regs[cont - 1];
		Ap->pont[cont + 1] = Ap->pont[cont];

		cont--;
	}

	// Insere registro na posição correta e atualiza o número de chaves da página
	Ap->regs[cont] = Reg;
	Ap->pont[cont + 1] = ApDir;
	Ap->n++;
}

void Ins(tipoItem Reg, TipoApontador Ap, bool *Cresceu, tipoItem *RegRetorno, TipoApontador *ApRetorno){
	// Ponteiro para página temporária (recentemente criada)
	TipoApontador ApTemp;
	
	// Contadores dentro das páginas
	long i = 1;
	long j;

	// Se a árvore está vazia ou está no nodo folha, retorna o registro e indica para crescer
	if (Ap == NULL){
		*Cresceu = true;
		*RegRetorno = Reg;
		*ApRetorno = NULL;
		return;
	}

	// Pesquisa se a chave já existe e a posição correta
	while (i < Ap->n && Reg.chave > Ap->regs[i - 1].chave){
		i++;
		comparacoesAB++;
	}
	if (Reg.chave == Ap->regs[i - 1].chave){
		printf("Erro: Registro já está presente\n");
		Cresceu = false;
		comparacoesAB++;
		return;
	}
	
	// Caso seja menor que o último elemento
	if (Reg.chave < Ap->regs[i - 1].chave){
		i--;
		comparacoesAB++;
	}

	// Chamada recursiva para execução até os nodos folhas
	Ins(Reg, Ap->pont[i], Cresceu, RegRetorno, ApRetorno);	// Desce recursivamente até o nivel final, e vai retornando os valores
	// **** RECURSÃO ****
	
	// Se não deve mais crescer, inserção finalizada
	if (!*Cresceu)
		return;

	// Página tem espaço
	if (Ap->n < MM){
		InsereNaPagina(Ap, *RegRetorno, *ApRetorno);
		*Cresceu = false;
		return;
	}

	// Overflow: Página tem que ser dividida
	ApTemp = (TipoApontador)malloc(sizeof(TipoPagina));
	ApTemp->n = 0;
	ApTemp->pont[0] = NULL;

	// Se o registro estiver na primeira metade
	if (i < M + 1){
		// Insere o ultimo registro na nova pagina
		InsereNaPagina(ApTemp, Ap->regs[MM - 1], Ap->pont[MM]);
		Ap->n--;

		// Insere o desejado na pagina atual
		InsereNaPagina(Ap, *RegRetorno, *ApRetorno);
	}
	// Se o registro estiver na segunda metade, insere na nova pagina
	else
		InsereNaPagina(ApTemp, *RegRetorno, *ApRetorno);

	// Passa os registros da segunda metade para a nova página
	for (j = M + 2; j <= MM; j++)
		InsereNaPagina(ApTemp, Ap->regs[j - 1], Ap->pont[j]);

	// Define a pagina atual como valor da ordem 
	Ap->n = M;
	ApTemp->pont[0] = Ap->pont[M + 1];

	// Retorna recursivamente o registro do meio que deve subir para o nodo pai
	*RegRetorno = Ap->regs[M];
	*ApRetorno = ApTemp;
}

void Insere(tipoItem Reg, TipoApontador *Ap){
	bool Cresceu;
	tipoItem RegRetorno;
	TipoPagina *ApRetorno, *ApTemp;

	// Realiza a inserção na árvore
	Ins(Reg, *Ap, &Cresceu, &RegRetorno, &ApRetorno);

	// Se árvore precisar crescer na altura pela raíz, cria um novo nodo raiz
	if (Cresceu){
		ApTemp = (TipoPagina*)malloc(sizeof(TipoPagina));
		ApTemp->n = 1;
		ApTemp->regs[0] = RegRetorno;
		ApTemp->pont[1] = ApRetorno;
		ApTemp->pont[0] = *Ap;
		
		*Ap = ApTemp;
	}
}

void LiberaArvore(TipoApontador arvore){
    // Verifica se a árvore está nula
    if (arvore == NULL)
        return;

    // Libera a memória de todos os filhos recursivamente
    for (int i = 0; i <= arvore->n; i++){
        LiberaArvore(arvore->pont[i]);
    }

    // Libera a página atual
    free(arvore);
}

void ImprimeRegistros(TipoApontador arvore){
    // Verifica se a árvore está nula
    if (arvore == NULL)
        return;

    // Realiza a impressão de todas as chaves
    long i;
    for (i = 0; i < arvore->n; i++){
        // Imprime os registros dos filhos à esquerda
        ImprimeRegistros(arvore->pont[i]);

        // Imprime a chave atual
        printf("%d ", arvore->regs[i].chave);
    }

    // Imprime os registros do filho à direita do último registro
    ImprimeRegistros(arvore->pont[i]);
}
