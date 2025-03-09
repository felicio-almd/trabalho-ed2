#ifndef HASH_H
#define HASH_H

#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <wctype.h>
#include <locale.h>
#include <string.h>

// Estruturas

/*
    Estrutura: NoPosicaoHash
    Atributos:
        posicaoHash: (Inteiro representando a posição lógica onde a palavra aparece no texto)
        proximo: (Ponteiro para o próximo nó da lista encadeada de posições)
    Objetivo: Armazena as posições em que uma palavra aparece no texto.
*/
typedef struct NoPosicaoHash
{
    int posicaoHash;
    struct NoPosicaoHash *proximo;
} NoPosicaoHash;

/*
    Estrutura: EntradaHash
    Atributos:
        palavraChave: (Ponteiro para uma string wide character armazenando a palavra-chave)
        posicoesHash: (Ponteiro para a lista encadeada de posições onde a palavra ocorre)
        ocupado: (Inteiro indicando o estado da entrada - 0: vazio, 1: ocupado, -1: deletado)
    Objetivo: Representa uma entrada na tabela hash contendo uma palavra-chave e suas ocorrências.
*/
typedef struct EntradaHash
{
    wchar_t *palavraChave;
    NoPosicaoHash *posicoesHash;
    int ocupado; // 0: vazio, 1: ocupado, -1: deletado
} EntradaHash;

/*
    Estrutura: TabelaHash
    Atributos:
        entradas: (Ponteiro para um array de entradas da tabela hash)
        tamanho: (Inteiro representando o tamanho da tabela hash)
    Objetivo: Estrutura para armazenar a tabela hash e gerenciar suas entradas.
*/
typedef struct TabelaHash
{
    EntradaHash *entradas;
    int tamanho;
} TabelaHash;

// Protótipos das funções

/*
    Função: criarTabelaHash
    Parâmetros:
        tamanho: (Inteiro representando o tamanho da tabela hash)
    Objetivo: Cria e inicializa uma nova tabela hash.
    Retorna um ponteiro para a estrutura TabelaHash criada.
*/
TabelaHash *criarTabelaHash(int tamanho);

/*
    Função: imprimirTabelaHash
    Parâmetros:
        tabela: (Ponteiro para a estrutura TabelaHash a ser impressa)
    Objetivo: Exibe o conteúdo da tabela hash.
*/
void imprimirTabelaHash(TabelaHash *tabela);

/*
    Função: funcaoHash
    Parâmetros:
        string: (Ponteiro para uma string wide character a ser mapeada para um índice)
        tamanhoTabela: (Inteiro representando o tamanho da tabela hash)
    Objetivo: Calcula o índice hash para uma string.
    Retorna um inteiro representando a posição na tabela hash.
*/
unsigned int funcaoHash(const wchar_t *string, int tamanhoTabela);

/*
    Função: inserirPalavraChaveHash
    Parâmetros:
        tabela: (Ponteiro para a estrutura TabelaHash onde a palavra será inserida)
        palavra: (Ponteiro para a string wide character a ser inserida)
    Objetivo: Insere uma palavra-chave na tabela hash.
*/
void inserirPalavraChaveHash(TabelaHash *tabela, const wchar_t *palavra);

/*
    Função: adicionarPosicaoHash
    Parâmetros:
        cabeca: (Ponteiro para o ponteiro da cabeça da lista de posições)
        posicaoHash: (Inteiro representando a posição lógica no texto)
    Objetivo: Adiciona uma nova posição na lista de posições de uma palavra-chave.
*/
void adicionarPosicaoHash(NoPosicaoHash **cabeca, int posicaoHash);

/*
    Função: processarTextoHash
    Parâmetros:
        tabela: (Ponteiro para a estrutura TabelaHash que armazenará as palavras)
        palavra: (Array de caracteres representando a palavra a ser processada)
        posicaoLogica: (Inteiro representando a posição lógica da palavra no texto)
    Objetivo: Processa uma palavra e adiciona suas informações à tabela hash.
*/
void processarTextoHash(TabelaHash *tabela, wchar_t palavra[1000], int posicaoLogica);

/*
    Função: imprimirPosicoesHash
    Parâmetros:
        no: (Ponteiro para a estrutura NoPosicaoHash contendo as posições de uma palavra)
    Objetivo: Imprime todas as posições onde a palavra aparece no texto.
*/
void imprimirPosicoesHash(NoPosicaoHash *no);

/*
    Função: imprimirIndiceHash
    Parâmetros:
        tabela: (Ponteiro para a estrutura TabelaHash)
    Objetivo: Imprime o índice completo da tabela hash, mostrando palavras e posições.
*/
void imprimirIndiceHash(TabelaHash *tabela);

/*
    Função: freeTabelaHash
    Parâmetros:
        tabela: (Ponteiro para a estrutura TabelaHash a ser liberada)
    Objetivo: Libera a memória alocada para a tabela hash.
*/
void freeTabelaHash(TabelaHash *tabela);

/*
    Função: compararEntradasHash
    Parâmetros:
        a: (Ponteiro para um elemento a ser comparado)
        b: (Ponteiro para outro elemento a ser comparado)
    Objetivo: Função de comparação para ordenação das entradas da tabela hash.
    Retorna um inteiro indicando a ordem relativa dos elementos.
*/
int compararEntradasHash(const void *a, const void *b);

#endif /* HASH_H */
