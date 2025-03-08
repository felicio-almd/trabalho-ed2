#ifndef PROCESSAMENTO_H
#define PROCESSAMENTO_H

#include <wchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <wctype.h>
#include <locale.h>
#include <string.h>

// Estrutura para armazenar dados processados
typedef struct
{
    wchar_t **palavras_chave;
    int num_palavras;
    wchar_t *texto;
} DadosProcessados;

// Protótipos das funções
wchar_t *normalizar_palavra(const wchar_t *palavra);
wchar_t **ler_palavras_chave(FILE *arquivo, int *num_palavras);
wchar_t *ler_texto_completo(FILE *arquivo);
DadosProcessados *processar_arquivos(const char *arquivo_palavras, const char *arquivo_texto);
void testar_leituras(const char *arquivo_palavras, const char *arquivo_texto);
void liberar_dados(DadosProcessados *dados);

#endif