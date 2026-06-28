#include "arvorebestrela.h"	

int transferenciasABE = 0, comparacoesABE = 0;

// Funcao que e chamada pra main
bool ArvoreBEstrela(FILE *arquivo, int quantidade, int chave, bool print, tipoItem *item, Estatisticas *controleProces, Estatisticas *controlePesq){
	// Inicializa nossa árvore B*
	TipoApontadorE arvore;
	inicializaArvore(&arvore);

	// Carrega n items do arquivo e insere na árvore
	controleProces->tempoI = clock();
	LoadArquivoABE(quantidade, arquivo, &arvore);
	controleProces->tempoF = clock();

	// Variaveis de processamento
	controleProces->tranferencias = transferenciasABE;
	controleProces->comparacoes = comparacoesABE;

	transferenciasABE = 0;
	comparacoesABE = 0;

    // Se possui argumento [-P], imprime todas as chaves da árvore
    if (print){
        printf("\nChaves do arquivo:\n");
		ImprimeRegistrosABE(arvore);
        printf("\n");
    }

	// Realiza a pesquisa do item na árvore
	controlePesq->tempoI = clock();
	PesquisaABE(chave, item,  &arvore);
	controlePesq->tempoF = clock();

	// Variaveis de pesquisa
	controlePesq->tranferencias = transferenciasABE;
	controlePesq->comparacoes = comparacoesABE;

	// Libera a mémória alocada pela árvore
	liberaArvoreBE(arvore);

	// Se a chave for encontrada, retorna verdadeiro
	if (item->chave != chave)
		return false;

	return true;  
}

void LoadArquivoABE(int qtd, FILE *arquivo, TipoApontadorE *arvore){
	TipoRegistro reg;
	int cont = 0;

    // Le cada registro individualmente, carrega na memória e adiciona na árvore
    transferenciasABE++;
	while (fread(&reg, sizeof(TipoRegistro), 1, arquivo) == 1 && cont < qtd){
        transferenciasABE++;

		InsereABE(reg, arvore);
		cont++;
	}
}

void inicializaArvore (TipoApontadorE * Arvore){
    *Arvore = NULL;
}

void PesquisaABE(int chave, TipoRegistro *x, TipoApontadorE *Ap){
    int i;
    TipoApontadorE Pag = *Ap;

    // caso seja um no de indices
    if (Pag->Pt == Interna){
        i = 1;
        comparacoesABE++;
        // faz o caminhamento pelos indices
        while (i < Pag->UU.U0.ni && chave > Pag->UU.U0.ri[i - 1]){
            comparacoesABE++;
            i++;
        }

        comparacoesABE++;

        if (chave < Pag->UU.U0.ri[i - 1])
            // desce pra esquerda(chave e menor que o indice)
            PesquisaABE(chave, x, &Pag->UU.U0.pi[i - 1]);
        else 
            //desce pra direita(chave e maior que o indice)
            PesquisaABE(chave, x, &Pag->UU.U0.pi[i]);
        return;    

    }

    i = 1;

    // caso seja nos de registros
    comparacoesABE++;
    //faz o caminhamento pelos nos de registros
    while (i < Pag->UU.U1.ne && chave > Pag->UU.U1.re[i - 1].chave){
        transferenciasABE++;
        i++;
    }
    comparacoesABE++;
    // achando o registro depois de uma pesquisa na pagina
    if (chave == Pag->UU.U1.re[i - 1].chave){
        comparacoesABE++;
        *x = Pag->UU.U1.re[i - 1];
    }
}

void InsereNaPaginaExterna(TipoApontadorE Ap, TipoRegistro Reg){
    bool validador;
    int j = Ap->UU.U1.ne;

    // validador para dizer quando deve ser inserida (valor false implica insercao)
    if(j > 0)
        validador = true;
    else
        validador = false;  

    while(validador){
        comparacoesABE++;
        // caso em que a chave deve ser inserida
        if(Reg.chave > Ap->UU.U1.re[j-1].chave){
            validador = false;
            break;
        }

        // caminhamento pelo no folha
        Ap->UU.U1.re[j] = Ap->UU.U1.re[j-1];
        j--;

        // a quantidade de registros chegou ao fim
        if(j < 1)
            validador = false;
    }

    //chave inserida
    Ap->UU.U1.re[j] = Reg;
    Ap->UU.U1.ne++;
}

void InsereNaPaginaInterna(TipoApontadorE Ap, TipoChave Reg, TipoApontadorE ApDir){
    bool validador;
    int j = Ap->UU.U0.ni;

    // validador para dizer quando deve ser inserida (valor false implica insercao)
    if(j > 0)
        validador = true;
    else
        validador = false; 

    while (validador){
        comparacoesABE++;

        // caso em que o indice deve ser inserida
        if (Reg >= Ap->UU.U0.ri[j - 1]){
            validador = false;
            break;
        }

        // caminhamento pelo no de indices
        Ap->UU.U0.ri[j] = Ap->UU.U0.ri[j - 1];
        Ap->UU.U0.pi[j+1] = Ap->UU.U0.pi[j];
        j--;

        // a quantidade chegou ao fim
        if (j < 1)
            validador = false;
    }

    // indice sendo inserido e apontadores sendo atualizados
    Ap->UU.U0.ri[j]= Reg;
    Ap->UU.U0.pi[j + 1] = ApDir;
    Ap->UU.U0.ni++;
}


void InsABE(TipoRegistro Reg, TipoApontadorE Ap, short *cresceu, TipoChave *RegRetorno, TipoApontadorE *ApRetorno){
    long i = 1;
    long j;

    // apontador para uma pagina nova
    TipoApontadorE ApTemp;

    // caso for externa
    if(Ap->Pt == Externa){
        *cresceu = true;
        *RegRetorno = Reg.chave;
        *ApRetorno = NULL;

        comparacoesABE++;

        // caminha por um no folha para dar valor ao indice i
        while(i < Ap->UU.U1.ne && Reg.chave > Ap->UU.U1.re[i-1].chave){
            comparacoesABE++;
            i++;
        }

        //caso ja exista o registro na arvore
        comparacoesABE++;
        if(Reg.chave == Ap->UU.U1.re[i-1].chave){
            *cresceu = false;
            return;
        }

        comparacoesABE++;

        // corrige a posicao do indice i
        if(Reg.chave < Ap->UU.U1.re[i-1].chave) 
            i--;

        // verifica se tem espaço para inserir
        if(Ap->UU.U1.ne < MM_E2){ 
            InsereNaPaginaExterna(Ap, Reg);
            *cresceu = false;
            return;
        }

        // overflow,
        // aloca uma nova pag
        ApTemp = (TipoApontadorE) malloc(sizeof(TipoPaginaE));
        ApTemp->UU.U1.ne = 0;
        ApTemp->Pt = Externa;

        //Verifica para onde a chave ira, ficar na pag atual ou ir pra nova
        if(i < MM_E + 1){
            //Insere o item na pagina que ja existe e coloca o ultimo registro na nova pagina
            InsereNaPaginaExterna(ApTemp, Ap->UU.U1.re[MM_E2 - 1]);
            Ap->UU.U1.ne--;

            //Insere o novo item na pagina atual
            InsereNaPaginaExterna(Ap, Reg);
        }
        // Inserindo o item que deu o overflow na pagina vizinha
        else InsereNaPaginaExterna(ApTemp, Reg);
            
        //Colocando os outros valores na nova pag
        for(j = MM_E + 1; j <= MM_E2; j++)
            InsereNaPaginaExterna(ApTemp, Ap->UU.U1.re[j-1]);
            
        // atualiza os valores para correcao da arvore pos overflow    
        *RegRetorno = Ap->UU.U1.re[MM_E].chave;
        Ap->UU.U1.ne = MM_E;
        *ApRetorno = ApTemp;

        return;
    }
    else{ //Ap->Pt == Interna
        comparacoesABE++;

        // caminha por um no de indices para dar valor ao indice i
        while(i < Ap->UU.U0.ni && Reg.chave > Ap->UU.U0.ri[i-1]){ 
            i++;
        }

        comparacoesABE++;

        // corrige o valor do indice i
        if(Reg.chave < Ap->UU.U0.ri[i-1]) 
            i--;

        // caminhamento recursivo pela arvore 
        InsABE(Reg, Ap->UU.U0.pi[i], cresceu, RegRetorno, ApRetorno);

        // condicao de parada
        if(!*cresceu) 
            return;

        //Verifica se a pagina NAO crescera e insere
        if(Ap->UU.U0.ni < MM_E2){ 
            InsereNaPaginaInterna(Ap, *RegRetorno, *ApRetorno);
            *cresceu = false;

            return;
        }

        // overflow,
        // aloca uma nova pag
        ApTemp = (TipoApontadorE) malloc(sizeof(TipoPaginaE));
        ApTemp->Pt = Interna;
        ApTemp->UU.U0.ni = 0;
        ApTemp->UU.U0.pi[0] = NULL;

        //Verifica para onde a chave ira
        if(i < MM_E + 1){
            //Insere o item na pagina que ja existe e coloca o ultimo registro na nova pagina
            InsereNaPaginaInterna(ApTemp, Ap->UU.U0.ri[MM_E2 - 1], Ap->UU.U0.pi[MM_E2]);
            Ap->UU.U0.ni--;
            //Insere o novo item na pagina atual
            InsereNaPaginaInterna(Ap, *RegRetorno, *ApRetorno);
        }
        // Inserindo o item que deu o overflow na pagina vizinha
        else InsereNaPaginaInterna(ApTemp, *RegRetorno, *ApRetorno);
            
        //Colocando os outros valores na nova pag
        for(j = MM_E + 2; j <= MM_E2; j++)
            InsereNaPaginaInterna(ApTemp, Ap->UU.U0.ri[j-1], Ap->UU.U0.pi[j]);
            

        // atualiza os valores para correcao da arvore pos overflow    
        Ap->UU.U0.ni = MM_E;
        ApTemp->UU.U0.pi[0] = Ap->UU.U0.pi[MM_E + 1];
        *RegRetorno = Ap->UU.U0.ri[MM_E];
        *ApRetorno = ApTemp;
    }
}

void InsereABE(TipoRegistro Reg, TipoApontadorE *Ap){
    short Cresceu;
    TipoChave RegRetorno;
    TipoPaginaE *ApRetorno = NULL, *ApTemp = NULL;

    
    //caso seja uma nova arvore
    if(*Ap == NULL){
        TipoPaginaE *ApTemp = (TipoPaginaE *)malloc(sizeof(TipoPaginaE));

        ApTemp->Pt = Externa;
        ApTemp->UU.U1.ne = 1;
        ApTemp->UU.U1.re[0] = Reg;
        *Ap = ApTemp;

        return;
    }
    
    InsABE(Reg, *Ap, &Cresceu, &RegRetorno, &ApRetorno);

    // caso haja a necessidade de um novo nodo raiz
    if (Cresceu){ 
        ApTemp = (TipoPaginaE *)malloc(sizeof(TipoPaginaE));
        ApTemp->Pt = Interna;
        ApTemp->UU.U0.ni = 1;
        ApTemp->UU.U0.ri[0] = RegRetorno;
        ApTemp->UU.U0.pi[1] = ApRetorno;
        ApTemp->UU.U0.pi[0] = *Ap;
        *Ap = ApTemp;
    }

}

void liberaArvoreBE(TipoApontadorE Arvore){
    int i = 0;

    if (Arvore->Pt == Externa){
        free(Arvore);
        
        return;
    }

    while (i <= Arvore->UU.U0.ni) {
        liberaArvoreBE(Arvore->UU.U0.pi[i]);

        if (i == Arvore->UU.U0.ni){
            free(Arvore);
            break; 
        }

        i++;
    } 
}

void ImprimeRegistrosABE(TipoApontadorE arvore) {
    // Verifica se a árvore está nula
    if (arvore == NULL)
        return;

    // Se a página for interna
    if (arvore->Pt == Interna) {
        long i;
        for (i = 0; i < arvore->UU.U0.ni; i++) {
            // Imprime os registros dos filhos à esquerda
            ImprimeRegistrosABE(arvore->UU.U0.pi[i]);

            // Imprime a chave atual
            printf("%ld ", arvore->UU.U0.ri[i]);
        }
        // Imprime os registros do filho à direita do último registro
        ImprimeRegistrosABE(arvore->UU.U0.pi[i]);
    }
    else { // Se a página for externa
        long i;
        for (i = 0; i < arvore->UU.U1.ne; i++) 
            printf("%d ", arvore->UU.U1.re[i].chave);
    }
}

