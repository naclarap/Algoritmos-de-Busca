#include "abp.h"

int transferenciasAbp = 0, comparacoesAbp = 0;

bool ArvoreBinaria(FILE *arq, int quantidade, int chave, bool print, tipoItem *item, Estatisticas *controleProces, Estatisticas *controlePesq){
    // Abre o arquivo da árvore binária externa
    FILE *arqAbp = fopen("abp.bin", "wb+");
    if (arqAbp == NULL) {
        perror("Não foi possível abrir o arquivo ABP");
        return 2;
    }

    // Constroi a árvore no arquivo externo
    controleProces->tempoI = clock();
    constroiArvore(arq, arqAbp, quantidade);
    controleProces->tempoF = clock();

	// Variaveis de processamento
	controleProces->tranferencias = transferenciasAbp;
	controleProces->comparacoes = comparacoesAbp;

    // Imprime todas as chaves conforme argumento [-P]
    if (print){
        printf("\nChaves do arquivo:\n");
        imprimeAbp(arqAbp, 0);
        printf("\n");
    }  

    transferenciasAbp = 0;
	comparacoesAbp = 0;

    // Pesquisa
    controlePesq->tempoI = clock();
    bool encontrado = pesquisarAbp(arqAbp, chave, item);
    controlePesq->tempoF = clock();

	// Variaveis de pesquisa
	controlePesq->tranferencias = transferenciasAbp;
	controlePesq->comparacoes = comparacoesAbp;

    fclose(arqAbp);
    return encontrado;
}


void insereNaArvore(FILE *arqAbp, tipoItem itemInserir) {
    // Cria e inicializa no
    tipoNo novoItem;
    novoItem.item = itemInserir;
    novoItem.esq = -1;
    novoItem.dir = -1;

    // Posiciona o ponteiro do arquivo no final p inserir novo no		
    fseek(arqAbp, 0, SEEK_END);
    long posicaoInserir = ftell(arqAbp);
    fwrite(&novoItem, sizeof(tipoNo), 1, arqAbp);
    transferenciasAbp += 2;
	
    // Se for raiz, retorna
    if (posicaoInserir == 0)
        return;
	
    // Reinicia a leitura do arquivo
    fseek(arqAbp, 0, SEEK_SET);
    transferenciasAbp++;
    tipoNo aux;
    long ponteiro = 0; 

    transferenciasAbp++;
	
    // Percorre a arvore para inserir no na posicao correta	
    while (fread(&aux, sizeof(tipoNo), 1, arqAbp) != 0) {
        transferenciasAbp++;

        comparacoesAbp++;    
        if (itemInserir.chave < aux.item.chave) {
            // Se o filho esquerdo estiver vazio, insere aq		
            if (aux.esq == -1) {
                aux.esq = posicaoInserir / sizeof(tipoNo);
                fseek(arqAbp, -sizeof(tipoNo), SEEK_CUR);
                fwrite(&aux, sizeof(tipoNo), 1, arqAbp);
                transferenciasAbp += 2;
                break;
            }
            ponteiro = aux.esq;// Continua a busca pelo lado esquerdo
        } else {
	    //	Se o filho direito estiver vazio, insere aq		
            if (aux.dir == -1) {
                aux.dir = posicaoInserir / sizeof(tipoNo);
                fseek(arqAbp, -sizeof(tipoNo), SEEK_CUR);
                fwrite(&aux, sizeof(tipoNo), 1, arqAbp);
                transferenciasAbp += 2;
                break;
            }
            ponteiro = aux.dir;// Continua a busca pelo lado direito
        }
	// Move o ponteiro do arquivo para a posição do próximo nó a ser analisado
        fseek(arqAbp, ponteiro * sizeof(tipoNo), SEEK_SET);
        transferenciasAbp++;
    }

}

void constroiArvore(FILE *arq, FILE *arqAbp, int quantidade) {
    tipoItem itemLeitura;
    int cont = 0;

    // Insere cada item do arquivo na árvore
    transferenciasAbp++;
    while (fread(&itemLeitura, sizeof(tipoItem), 1, arq) != 0 && cont < quantidade) {
        insereNaArvore(arqAbp, itemLeitura);

        cont++;

        transferenciasAbp++;
    }

}

bool pesquisarAbp(FILE *arqAbp, int chave, tipoItem *item) {
    tipoNo aux;
    long ponteiro = 0;

    // Percorre todos os nós e ponteiros até encontrar o item
    transferenciasAbp += 2;
    while (fseek(arqAbp, ponteiro * sizeof(tipoNo), SEEK_SET) == 0 && fread(&aux, sizeof(tipoNo), 1, arqAbp)) {
        transferenciasAbp += 2;
        
        comparacoesAbp += 2;
        // Se encontrar, retorna o item
        if (aux.item.chave == chave){
            *item = aux.item;
            return true;
        }
        // Desce para o ponteiro correto
        else if (chave > aux.item.chave)
            ponteiro = aux.dir;
        else
            ponteiro = aux.esq;
    }

    return false;
}

void imprimeAbp(FILE *arqAbp, long ponteiro) {
    // Verifica se o ponteiro é válido
    if (ponteiro == -1) {
        return; // Se o ponteiro for -1, não há nó para visitar
    }

    tipoNo aux;

    // Posiciona o cursor no nó atual
    fseek(arqAbp, ponteiro * sizeof(tipoNo), SEEK_SET);

    // Lê o nó atual
    if (fread(&aux, sizeof(tipoNo), 1, arqAbp) != 1) {
        return;
    }

    // Percorre a subárvore esquerda
    imprimeAbp(arqAbp, aux.esq);

    // Imprime a chave do nó atual
    printf("%d ", aux.item.chave);

    // Percorre a subárvore direita
    imprimeAbp(arqAbp, aux.dir);
}
