#ifndef TRIE_H
#define TRIE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <wctype.h>
#include <locale.h>

// Estruturas

/*
    Estrutura: NoPosicaoTrie
    Atributos:
        posicao: (Inteiro representando a posição lógica da palavra-chave no texto)
        proximo: (Ponteiro para o próximo nó da lista encadeada de posições)
    Objetivo: Armazena a posição lógica das ocorrências de uma palavra-chave dentro do texto.
*/
typedef struct NoPosicaoTrie
{
    int posicao;
    struct NoPosicaoTrie *proximo;
} NoPosicaoTrie;

/*
    Estrutura: NoTrie
    Atributos:
        filhos: (Array de ponteiros para nós da Trie, usado para armazenar caracteres da palavra-chave)
        palavraChave: (Ponteiro para uma string wide character (wchar_t*) que armazena a palavra-chave completa)
        posicoes: (Ponteiro para a lista de posições onde a palavra-chave ocorre no texto)
    Objetivo: Representa um nó da Trie para armazenamento e busca eficiente de palavras-chave.
*/
typedef struct NoTrie
{
    struct NoTrie *filhos[128]; // 128 posições para poder aceitar caracteres acentuados
    wchar_t *palavraChave;
    NoPosicaoTrie *posicoes;
} NoTrie;

// Protótipos das funções

/*
    Função: criarNoTrie
    Objetivo: Cria e inicializa um novo nó da Trie.
    Retorna um ponteiro para o nó criado.
*/
NoTrie *criarNoTrie();

/*
    Função: obterIndiceCaractere
    Parâmetros:
        c: (Caractere wide character (wchar_t) a ser indexado)
    Objetivo: Obtém o índice correspondente a um caractere para armazenamento na Trie.
    Retorna o índice do caractere.
*/
int obterIndiceCaractere(wchar_t c);

/*
    Função: inserirPalavraChaveTrie
    Parâmetros:
        raiz: (Ponteiro para o nó raiz da Trie)
        palavra: (Ponteiro para a string wide character (wchar_t*) representando a palavra-chave a ser inserida)
    Objetivo: Insere uma palavra-chave na Trie.
*/
void inserirPalavraChaveTrie(NoTrie *raiz, const wchar_t *palavra);

/*
    Função: adicionarPosicaoTrie
    Parâmetros:
        cabeca: (Ponteiro para o ponteiro da lista encadeada de posições)
        posicao: (Inteiro representando a posição lógica da palavra-chave no texto)
    Objetivo: Adiciona uma nova posição lógica à lista de posições de uma palavra-chave na Trie.
*/
void adicionarPosicaoTrie(NoPosicaoTrie **cabeca, int posicao);

/*
    Função: processarTextoTrie
    Parâmetros:
        raiz: (Ponteiro para o nó raiz da Trie)
        palavra: (Array wide character (wchar_t) representando a palavra a ser processada)
        posicaoLogica: (Inteiro representando a posição lógica da palavra no texto)
        tamanhoPalavra: (Inteiro representando o tamanho da palavra a ser processada)
    Objetivo: Processa o texto, identificando palavras e armazenando suas posições na Trie.
*/
void processarTextoTrie(NoTrie *raiz, wchar_t palavra[1000], int posicaoLogica, int tamanhoPalavra);

/*
    Função: imprimirPosicoesTrie
    Parâmetros:
        no: (Ponteiro para a estrutura NoPosicaoTrie que contém as posições a serem impressas)
    Objetivo: Imprime todas as posições onde uma palavra-chave ocorre no texto.
*/
void imprimirPosicoesTrie(NoPosicaoTrie *no);

/*
    Função: imprimirIndiceTrie
    Parâmetros:
        no: (Ponteiro para o nó raiz da Trie)
    Objetivo: Imprime todas as palavras-chave armazenadas na Trie junto com suas posições no texto.
*/
void imprimirIndiceTrie(NoTrie *no);

/*
    Função: imprimirTrie
    Parâmetros:
        noAtual: (Ponteiro para o nó atual da Trie)
        caminhoAtual: (Ponteiro para a string wide character armazenando o caminho da palavra atual)
        profundidadeAtual: (Inteiro representando a profundidade do nó na Trie)
        indentacaoAtual: (Inteiro representando a indentação para a impressão)
    Objetivo: Imprime a estrutura completa da Trie de forma hierárquica.
*/
void imprimirTrie(NoTrie *noAtual, wchar_t *caminhoAtual, int profundidadeAtual, int indentacaoAtual);

/*
    Função: freeTrie
    Parâmetros:
        no: (Ponteiro para o nó raiz da Trie a ser liberado)
    Objetivo: Libera a memória alocada para toda a estrutura da Trie.
*/
void freeTrie(NoTrie *no);

#endif // TRIE_H
