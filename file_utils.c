#include "file_utils.h"
#include <stdio.h>
#include <stdlib.h>

char keywords[MAX_KEYWORDS][MAX_KEYWORD_LENGTH];
int keyword_count = 0; // Defina a variável global aqui

char *read_file(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        perror("Erro ao abrir o arquivo");
        return NULL;
    }

    char *text = (char *)malloc(MAX_TEXT_SIZE * sizeof(char));
    if (!text)
    {
        perror("Erro ao alocar memória");
        fclose(file);
        return NULL;
    }

    fread(text, sizeof(char), MAX_TEXT_SIZE, file);
    fclose(file);

    return text;
}

void load_keywords(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        perror("Erro ao abrir o arquivo de palavras-chave");
        exit(EXIT_FAILURE);
    }

    while (fscanf(file, "%s", keywords[keyword_count]) != EOF)
    {
        keyword_count++;
    }

    fclose(file);
}