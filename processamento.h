#ifndef PROCESSAMENTO_H
#define PROCESSAMENTO_H

#include <wchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <wctype.h>
#include <locale.h>
#include <string.h>

// Estruturas

/*
    Estrutura: DadosProcessados
    Atributos:
        palavrasChave: (Ponteiro para um array de strings wide character (wchar_t*) que armazena as palavras-chave processadas)
        numPalavras: (Inteiro que representa o número de palavras-chave armazenadas)
        texto: (Ponteiro para uma string wide character (wchar_t*) que armazena o texto completo processado)
    Objetivo: Armazenar os dados processados, incluindo as palavras-chave e o texto completo.
*/
typedef struct dadosProcessados
{
    wchar_t **palavrasChave;
    int numPalavras;
    wchar_t *texto;
} DadosProcessados;

// Protótipos das funções

/*
    Função: normalizarPalavra
    Parâmetros:
        palavra: (Ponteiro para uma string wide character (wchar_t*) que representa a palavra a ser normalizada)
    Objetivo: Normaliza uma palavra, removendo acentos e convertendo-a para minúsculas.
    Retorna um ponteiro para a palavra normalizada.
*/
wchar_t *normalizarPalavra(const wchar_t *palavra);

/*
    Função: lerPalavrasChave
    Parâmetros:
        arquivo: (Ponteiro para o arquivo que contém as palavras-chave)
        numPalavras: (Ponteiro para um inteiro que armazenará o número de palavras-chave lidas)
    Objetivo: Lê as palavras-chave de um arquivo e as armazena em um array de strings wide character.
    Retorna um ponteiro para o array de palavras-chave.
*/
wchar_t **lerPalavrasChave(FILE *arquivo, int *numPalavras);

/*
    Função: lerTextoCompleto
    Parâmetros:
        arquivo: (Ponteiro para o arquivo que contém o texto completo)
    Objetivo: Lê o texto completo de um arquivo e o armazena em uma string wide character.
    Retorna um ponteiro para o texto completo lido.
*/
wchar_t *lerTextoCompleto(FILE *arquivo);

/*
    Função: processarArquivos
    Parâmetros:
        arquivo_palavras: (Ponteiro para uma string que representa o nome do arquivo contendo as palavras-chave)
        arquivo_texto: (Ponteiro para uma string que representa o nome do arquivo contendo o texto completo)
    Objetivo: Processa os arquivos de palavras-chave e texto, normalizando as palavras e armazenando os dados na estrutura DadosProcessados.
    Retorna um ponteiro para a estrutura DadosProcessados contendo os dados processados.
*/
DadosProcessados *processarArquivos(const char *arquivo_palavras, const char *arquivo_texto);

/*
    Função: liberarDados
    Parâmetros:
        dados: (Ponteiro para a estrutura DadosProcessados que contém os dados a serem liberados)
    Objetivo: Libera a memória alocada para a estrutura DadosProcessados, incluindo as palavras-chave e o texto.
*/
void liberarDados(DadosProcessados *dados);

#endif // PROCESSAMENTO_H