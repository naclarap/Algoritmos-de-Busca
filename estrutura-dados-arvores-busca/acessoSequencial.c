#include "acessoSequencial.h"

bool AcessoSequencial(FILE* arquivo, int quantidade, int chave, bool print, tipoItem *item, Estatisticas *controleProces, Estatisticas *controlePesq){
    int tabelaIndices[MAXTABELA];

    // Gera a tabela de índices de acordo com o arquivo
    controleProces->tempoI = clock();
    geraTabela(tabelaIndices, arquivo, quantidade, controleProces);
    controleProces->tempoF = clock();

    // Se possui argumento [-P], imprime todas as chaves do arquivo
    if (print){
        printf("\nChaves do arquivo:\n");
        imprimeSequencial(arquivo, quantidade);
        printf("\n");
    }

    // Pesquisa
    controlePesq->tempoI = clock();
    bool encontrado =  pesquisaSequencial(tabelaIndices, arquivo, quantidade, chave, item, controlePesq);
    controlePesq->tempoF = clock();

    return encontrado;
} 

void geraTabela(int tabelaIndices[], FILE* arquivo, int quantidade, Estatisticas *controleProces){
    tipoItem item[ITENSPAGINA];
    int contador = 0;
    int posicao = 0;

    // Lê o numero de itens por página e salva o menor da página
    while (fread(&item, sizeof(tipoItem), ITENSPAGINA, arquivo) > 0 && contador < quantidade){
        contador += ITENSPAGINA;

        tabelaIndices[posicao] = item[0].chave;
        posicao++;

        controleProces->tranferencias++;
    }
}

bool pesquisaSequencial(int tabelaIndice[], FILE* arquivo, int quantidade, int chave, tipoItem *item, Estatisticas *controlePesq){
    //declarar variaveis para pesquisa
    tipoItem pagina[ITENSPAGINA];
    int i = 0, quantidadeItens = 0;
    long deslocamento;

    //procurar pagina onde pode o item esta
    controlePesq->comparacoes++;
    while ((i < quantidade) && (tabelaIndice[i] <= chave)){
        controlePesq->comparacoes++;
        i++;
    }

    //se a primeira chave for menor que o item desejado
    //o item não existe
    if(i == 0)
        return false;
    
    //pode ser que a ultima pagina esteja incompleta
    if(i < quantidade)
        quantidadeItens = ITENSPAGINA;
    else{
        //calcular a quantidade de itens na ultima pagina
        quantidadeItens = (quantidade / sizeof(tipoItem)) % ITENSPAGINA;
        if (quantidadeItens == 0)
            quantidadeItens = ITENSPAGINA;
    }

    //pagina desejada
    //calcula onde a pagina comeca no arquivo
    deslocamento = (i - 1) * ITENSPAGINA * sizeof(tipoItem);
    fseek(arquivo, deslocamento, SEEK_SET);
    fread(&pagina, sizeof(tipoItem), quantidadeItens, arquivo);

    controlePesq->tranferencias += 2;

    //pesquisar sequencialmente na pagina lida
    for(int i = 0; i < quantidadeItens; i++){
        controlePesq->comparacoes++;
        
        if(pagina[i].chave == chave){
            *item = pagina[i];
            return true;
        }
    }
    return false;
}

void imprimeSequencial(FILE* arquivo, int quantidade){
    tipoItem item[ITENSPAGINA];
    int contador = 0;

    fseek(arquivo, 0, SEEK_SET);

    // Lê blocos de itens e imprime todos
    while (fread(&item, sizeof(tipoItem), ITENSPAGINA, arquivo) > 0 && contador < quantidade){
        contador += ITENSPAGINA;

        for(int i = 0; i < ITENSPAGINA; i++)
            printf("%d ", item[i].chave);
    }
}