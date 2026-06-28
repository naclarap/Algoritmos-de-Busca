#include "utilidades.h"
#include "acessoSequencial.h"
#include "abp.h"
#include "arvoreB.h"
#include "arvorebestrela.h"

bool Pesquisa(FILE *arquivo, int metodo, int quantidade, int situacao, int chave, bool print, tipoItem *item, Estatisticas *controleProces, Estatisticas *controlePesq);

int main(int argc, char *argv[]) {
    // Verifica se o número de argumentos está correto
    if (!verificaArgumentos(argc, argv)) {
        printf("Uso: pesquisa <metodo> <quantidade> <situacao> <chave> [-P]\n");
        return 1;
    }

   // Lê os argumentos da linha de comando
    int metodo =        atoi(argv[1]);
    int quantidade =    atoi(argv[2]);
    int situacao =      atoi(argv[3]);
    int chave =         atoi(argv[4]);
    bool printChaves =  (argc == 6 && strcmp(argv[5], "[-P]") == 0);

    // Abre o arquivo correto
    FILE* arquivo = abreArquivo(situacao);
    if(arquivo == NULL)
        return 2;

    // Configura o item a ser pesquisado e variáveis de controle
    tipoItem item;
    Estatisticas controleProcess, controlePesq;
    iniciaControle(&controleProcess);
    iniciaControle(&controlePesq);

    // Pesquisa de acordo com os parâmetros e retorna se foi bem sucedida
    if (Pesquisa(arquivo, metodo, quantidade, situacao, chave, printChaves, &item, &controleProcess, &controlePesq)){
        printf("\nRegistro encontrado!");
        imprimeRegistro(item);
    }
    else
        printf("\nRegistro não encontrado!\n\n");

    // Variáveis de teste
    printf("\n* PRÉ-PROCESSAMENTO *");
    ImprimeControle(controleProcess);
    printf("\n* PESQUISA *");
    ImprimeControle(controlePesq);
    printf("\n");

    fclose(arquivo);
    return 0;
}

// Escolhe o método especificado
bool Pesquisa(FILE *arquivo, int metodo, int quantidade, int situacao, int chave, bool print, tipoItem *item, Estatisticas *controleProces, Estatisticas *controlePesq){

    switch (metodo){
        case 1:
            printf("\nACESSO SEQUENCIAL\n");
            return AcessoSequencial(arquivo, quantidade, chave, print, item, controleProces, controlePesq);
        case 2:
            printf("\nÁRVORE BINÁRIA\n");
            return ArvoreBinaria(arquivo, quantidade, chave, print, item, controleProces, controlePesq);
        case 3:
            printf("\nÁRVORE B\n");
            return ArvoreB(arquivo, quantidade, chave, print, item, controleProces, controlePesq);
        case 4:
            printf("\nÁRVORE B*\n");
            return ArvoreBEstrela(arquivo, quantidade, chave, print, item, controleProces, controlePesq);
    }

    return false;
}