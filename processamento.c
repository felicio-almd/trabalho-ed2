#include "processamento.h"

wchar_t *normalizarPalavra(const wchar_t *palavra)
{
    size_t len = wcslen(palavra);
    wchar_t *normalizada = malloc((len + 1) * sizeof(wchar_t));

    for (size_t i = 0; i < len; i++)
    {
        normalizada[i] = towlower(palavra[i]);
    }
    normalizada[len] = L'\0';
    return normalizada;
}

wchar_t **lerPalavrasChave(FILE *arquivo, int *numPalavras)
{
    wchar_t linha[1000];
    wchar_t **palavras = NULL;
    int capacidade = 10;
    int contador = 0;

    palavras = malloc(capacidade * sizeof(wchar_t *));

    // lê uma linha do arquivo e a armazena no array linha enquanto tiver linhas no arquivo
    while (fgetws(linha, 1000, arquivo))
    {
        // Remove quebra de linha
        size_t len = wcslen(linha);
        for (size_t i = 0; i < len; i++)
        {
            // se tiver quebra de linha, acabar o arquivo ou tiver um ponto, remove caractere
            if (linha[i] == L'\n' || linha[i] == L'\r' || linha[i] == L'.')
            {
                linha[i] = L'\0';
                break;
            }
        }
        // para garantir que não irá faltar espaço
        if (contador >= capacidade)
        {
            capacidade *= 2;
            palavras = realloc(palavras, capacidade * sizeof(wchar_t *));
        }
        palavras[contador] = normalizarPalavra(linha);
        contador++;
    }

    *numPalavras = contador;
    return palavras;
}

wchar_t *lerTextoCompleto(FILE *arquivo)
{
    fseek(arquivo, 0, SEEK_END);
    long tamanhoBytes = ftell(arquivo);
    fseek(arquivo, 0, SEEK_SET);

    // Le como bytes brutos primeiro
    char *buffer = malloc(tamanhoBytes + 1);
    fread(buffer, 1, tamanhoBytes, arquivo);
    buffer[tamanhoBytes] = '\0';

    // Converte para wide string pela lib
    size_t tamanho_wchar = mbstowcs(NULL, buffer, 0) + 1;
    wchar_t *texto = malloc(tamanho_wchar * sizeof(wchar_t));
    mbstowcs(texto, buffer, tamanhoBytes);

    free(buffer);
    return texto;
}

// Vai abrir e ler os arquivos
DadosProcessados *processarArquivos(const char *arquivo_palavras, const char *arquivo_texto)
{
    DadosProcessados *dados = malloc(sizeof(DadosProcessados));

    // Processar palavras-chave abrindo o arquivo
    FILE *arqPalavrasChave = fopen(arquivo_palavras, "r");
    if (!arqPalavrasChave)
    {
        perror("Erro ao abrir arquivo de palavras-chave");
        free(dados);
        return NULL;
    }

    dados->palavrasChave = lerPalavrasChave(arqPalavrasChave, &dados->numPalavras);
    fclose(arqPalavrasChave);

    // Processar texto completo com espaços e caracteres
    FILE *arqTexto = fopen(arquivo_texto, "r");
    if (!arqTexto)
    {
        perror("Erro ao abrir arquivo de texto");
        free(dados->palavrasChave);
        free(dados);
        return NULL;
    }
    dados->texto = lerTextoCompleto(arqTexto);
    fclose(arqTexto);

    return dados;
}

void liberarDados(DadosProcessados *dados)
{
    if (dados)
    {
        for (int i = 0; i < dados->numPalavras; i++)
        {
            free(dados->palavrasChave[i]);
        }
        free(dados->palavrasChave);
        free(dados->texto);
        free(dados);
    }
}