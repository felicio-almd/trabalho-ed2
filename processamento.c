#include "processamento.h"

static wchar_t *normalizar_palavra(const wchar_t *palavra)
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

static wchar_t **ler_palavras_chave(FILE *arquivo, int *num_palavras)
{
    wchar_t linha[256];
    wchar_t **palavras = NULL;
    int capacidade = 10;
    int contador = 0;

    palavras = malloc(capacidade * sizeof(wchar_t *));

    while (fgetws(linha, 256, arquivo))
    {
        // Mantém a quebra de linha original se existir
        if (contador >= capacidade)
        {
            capacidade *= 2;
            palavras = realloc(palavras, capacidade * sizeof(wchar_t *));
        }
        palavras[contador] = normalizar_palavra(linha);
        contador++;
    }

    *num_palavras = contador;
    return palavras;
}

static wchar_t *ler_texto_completo(FILE *arquivo)
{
    fseek(arquivo, 0, SEEK_END);
    long tamanho_bytes = ftell(arquivo);
    fseek(arquivo, 0, SEEK_SET);

    // Ler como bytes brutos primeiro
    char *buffer = malloc(tamanho_bytes + 1);
    fread(buffer, 1, tamanho_bytes, arquivo);
    buffer[tamanho_bytes] = '\0';

    // Converter para wide string
    size_t tamanho_wchar = mbstowcs(NULL, buffer, 0) + 1;
    wchar_t *texto = malloc(tamanho_wchar * sizeof(wchar_t));
    mbstowcs(texto, buffer, tamanho_bytes);

    free(buffer);
    return texto;
}

DadosProcessados *processar_arquivos(const char *arquivo_palavras, const char *arquivo_texto)
{
    DadosProcessados *dados = malloc(sizeof(DadosProcessados));

    // Processar palavras-chave
    FILE *fp_palavras = fopen(arquivo_palavras, "r");
    if (!fp_palavras)
    {
        perror("Erro ao abrir arquivo de palavras-chave");
        free(dados);
        return NULL;
    }
    dados->palavras_chave = ler_palavras_chave(fp_palavras, &dados->num_palavras);
    fclose(fp_palavras);

    // Processar texto completo com espaços
    FILE *fp_texto = fopen(arquivo_texto, "r");
    if (!fp_texto)
    {
        perror("Erro ao abrir arquivo de texto");
        free(dados->palavras_chave);
        free(dados);
        return NULL;
    }
    dados->texto = ler_texto_completo(fp_texto);
    fclose(fp_texto);

    return dados;
}

void testar_leituras(const char *arquivo_palavras, const char *arquivo_texto)
{
    DadosProcessados *dados = processar_arquivos(arquivo_palavras, arquivo_texto);
    if (!dados)
        return;

    wprintf(L"\n=== PALAVRAS-CHAVE ===\n");
    for (int i = 0; i < dados->num_palavras; i++)
    {
        wprintf(L"Palavra %d: %ls", i + 1, dados->palavras_chave[i]);
    }

    wprintf(L"\n=== TEXTO COMPLETO ===\n");
    wprintf(L"%ls\n", dados->texto);

    liberar_dados(dados);
}

void liberar_dados(DadosProcessados *dados)
{
    if (dados)
    {
        for (int i = 0; i < dados->num_palavras; i++)
        {
            free(dados->palavras_chave[i]);
        }
        free(dados->palavras_chave);
        free(dados->texto);
        free(dados);
    }
}