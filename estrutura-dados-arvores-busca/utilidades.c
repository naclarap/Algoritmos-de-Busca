#include "utilidades.h"

bool verificaArgumentos(int argc, char *argv[]){
    // Verifica se o número de argumentos está correto
    if (argc < 5 || argc > 6) {
        perror("Uso: pesquisa <metodo> <quantidade> <situacao> <chave> [-P]\n");
        return false;
    }

    // Lê os argumentos da linha de comando
    int metodo = atoi(argv[1]);
    int quantidade = atoi(argv[2]);
    int situacao = atoi(argv[3]);

    // Verificação de erros no comando pesquisa
    if (metodo < 1 || metodo > 4){
        printf("\nMétodo inválido (1 a 4)");
        return 1;
    }
    if (quantidade < 0 || quantidade > 2000000){
        printf("\nQuantidade inválida (10 a 2.000.00)");
        return 1;
    }
    if (situacao < 1 || situacao > 3){
        printf("\nSituação inválida (1 a 3)");
        return 1;
    }
    if (argc == 6 && strcmp(argv[5], "[-P]") != 0){
        printf("\nArgumento opcional inválido [-P]");
        return 1;
    }

    return true;
} 

FILE* abreArquivo(int situacao){
    FILE *arquivo;
    char nomeArquivo[17];

    switch (situacao){
        case 1:     // Crescente
            strcpy(nomeArquivo, "crescente.bin");
            break;
        case 2:     // Decrescente
            strcpy(nomeArquivo, "decrescente.bin");
            break;
        case 3:     // Aleatorio
            strcpy(nomeArquivo, "aleatorio.bin");
            break;
    }

    // Abre o arquivo correto para pesquisa
    arquivo = fopen(nomeArquivo, "r+b");
    if (arquivo == NULL){
        perror("Não foi possível abrir o arquivo de dados");
        return NULL;
    }

    return arquivo;
}

void imprimeRegistro(tipoItem item){
    printf("\033[32m"); // Inicia a cor verde
    printf("\n=== Registro de chave %d ===", item.chave);
    printf("\nDado1: %ld", item.dado1);
    printf("\nDado2: %.12s\n\n", item.dado2);
    printf("\033[0m");
}

void ImprimeControle(Estatisticas controle){
    printf("\033[33m");
    printf("\nNúmero de transferências: %d", controle.tranferencias);
    printf("\nNúmero de comparações: %d", controle.comparacoes);
    printf("\nTempo de execução: %lfs\n", (double)(controle.tempoF - controle.tempoI) / CLOCKS_PER_SEC);
    printf("\033[0m");
}

void iniciaControle(Estatisticas *controle){
    controle->tranferencias = 0;
    controle->comparacoes = 0;
    controle->tempoI = 0;
    controle->tempoF = 0;
}