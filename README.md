# Estruturas de Indexação e Busca em Arquivos

Implementação em C de três métodos de organização e busca de registros em arquivos: **acesso sequencial**, **Árvore B** e **Árvore B\***, com comparação de desempenho entre eles. Projeto da disciplina de Estrutura de Dados II (UFOP).

> ⚠️Trabalho desenvolvido em grupo, como parte da disciplina.

## O que o programa faz

Recebe um arquivo de registros e realiza buscas por chave usando o método escolhido, reportando estatísticas de processamento (número de comparações/acessos) para cada estratégia — permitindo comparar o custo de cada estrutura na prática.

```
Uso: pesquisa <metodo> <quantidade> <situacao> <chave> [-P]
```

- `metodo`: técnica de busca (sequencial, Árvore B ou Árvore B\*)
- `quantidade`: quantidade de registros a processar
- `situacao`: estado da base (ex.: ordenada, desordenada)
- `chave`: chave de busca
- `-P`: (opcional) imprime as chaves visitadas durante a busca

## Estrutura do código

| Arquivo | Conteúdo |
|---|---|
| `acessoSequencial.c` | Busca sequencial em arquivo |
| `arvoreB.c` | Implementação de Árvore B (inserção, busca, balanceamento) |
| `arvorebestrela.c` | Implementação de Árvore B\* |
| `abp.c` | Árvore Binária de Pesquisa |
| `utilidades.c` | Funções auxiliares de leitura/escrita de arquivo e estatísticas |
| `main.c` | Ponto de entrada e seleção do método de busca |

## Como compilar

```bash
gcc *.c -o pesquisa
./pesquisa <metodo> <quantidade> <situacao> <chave>
```

## Conceitos aplicados

Organização de arquivos, índices, árvores balanceadas multiway (B e B\*), análise comparativa de custo de E/S entre estruturas de indexação.
