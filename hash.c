#include "hash.h"
#include <stdlib.h>
#include <stdio.h>
#include <wctype.h>
#include <string.h>

// Função para normalizar palavras (mantém acentos)
static wchar_t *normalizeWord(const wchar_t *word)
{
    size_t len = wcslen(word);
    wchar_t *normalized = malloc((len + 1) * sizeof(wchar_t));

    for (size_t i = 0; i < len; i++)
    {
        normalized[i] = towlower(word[i]);
    }
    normalized[len] = L'\0';
    return normalized;
}

// Função de hash para strings wide
static int hashFunction(const wchar_t *key, int size)
{
    int hash = 0;
    for (int i = 0; key[i]; i++)
    {
        hash = (31 * hash + key[i]) % size;
    }
    return hash;
}

// Função para verificar números primos
static int isPrime(int n)
{
    if (n <= 1)
        return 0;
    if (n <= 3)
        return 1;
    if (n % 2 == 0 || n % 3 == 0)
        return 0;
    for (int i = 5; i * i <= n; i += 6)
        if (n % i == 0 || n % (i + 2) == 0)
            return 0;
    return 1;
}

// Próximo número primo
static int nextPrime(int n)
{
    while (!isPrime(n))
        n++;
    return n;
}

// Busca na tabela hash (usando linear probing)
HashEntry *searchHashTable(const HashTable *table, const wchar_t *word)
{
    wchar_t *normalized = normalizeWord(word);
    int index = hashFunction(normalized, table->size);

    for (int i = 0; i < table->size; i++)
    {
        int current = (index + i) % table->size;

        if (table->entries[current].is_active == 0)
            break;
        if (table->entries[current].is_active == 1 &&
            wcscmp(table->entries[current].keyword, normalized) == 0)
        {
            free(normalized);
            return &table->entries[current];
        }
    }
    free(normalized);
    return NULL;
}

// Redimensiona a tabela hash
static void resizeHashTable(HashTable *table)
{
    int old_size = table->size;
    HashEntry *old_entries = table->entries;

    table->size = nextPrime(table->size * 2);
    table->entries = calloc(table->size, sizeof(HashEntry));
    table->count = 0;

    for (int i = 0; i < old_size; i++)
    {
        if (old_entries[i].is_active == 1)
        {
            insertKeywordHash(table, old_entries[i].keyword);
            HashEntry *entry = searchHashTable(table, old_entries[i].keyword);
            if (entry)
            {
                entry->positions = old_entries[i].positions;
            }
        }
    }
    free(old_entries);
}

// Cria tabela hash
HashTable *createHashTable(int size)
{
    HashTable *table = malloc(sizeof(HashTable));
    table->size = size;
    table->count = 0;
    table->entries = calloc(size, sizeof(HashEntry));
    return table;
}

// Insere palavra na tabela hash
void insertKeywordHash(HashTable *table, const wchar_t *word)
{
    if ((double)table->count / table->size >= HASH_LOAD_FACTOR)
    {
        resizeHashTable(table);
    }

    wchar_t *normalized = normalizeWord(word);
    int index = hashFunction(normalized, table->size);

    for (int i = 0; i < table->size; i++)
    {
        int current = (index + i) % table->size;

        if (table->entries[current].is_active == 1)
        {
            if (wcscmp(table->entries[current].keyword, normalized) == 0)
            {
                free(normalized);
                return; // Palavra já existe
            }
        }
        else
        {
            table->entries[current].keyword = wcsdup(normalized);
            table->entries[current].positions = NULL;
            table->entries[current].is_active = 1;
            table->count++;
            free(normalized);
            return;
        }
    }
}

// Adiciona posição mantendo ordenação
void addPositionHash(PositionNodeHash **head, int position)
{
    PositionNodeHash *new_node = malloc(sizeof(PositionNodeHash));
    new_node->position = position + 1; // Posições começam em 1

    if (*head == NULL || position + 1 < (*head)->position)
    {
        new_node->next = *head;
        *head = new_node;
        return;
    }

    PositionNodeHash *current = *head;
    while (current->next && current->next->position < position + 1)
    {
        current = current->next;
    }

    new_node->next = current->next;
    current->next = new_node;
}

// Processa o texto
void processTextHash(HashTable *table, const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        perror("Erro ao abrir arquivo de texto");
        exit(1);
    }

    int logical_pos = 0;
    wchar_t buffer[1024];

    while (fgetws(buffer, sizeof(buffer) / sizeof(wchar_t), file))
    {
        wchar_t *ptr = buffer;

        while (*ptr)
        {
            while (*ptr && !iswalpha(*ptr))
            {
                ptr++;
                logical_pos++;
            }

            wchar_t word[256];
            int i = 0;
            while (*ptr && iswalpha(*ptr) && i < 255)
            {
                word[i++] = *ptr++;
            }
            word[i] = L'\0';

            if (i > 0)
            {
                wchar_t *normalized = normalizeWord(word);
                HashEntry *entry = searchHashTable(table, normalized);
                if (entry)
                {
                    addPositionHash(&entry->positions, logical_pos);
                }
                free(normalized);
                logical_pos++;
            }
        }
    }
    fclose(file);
}

// Imprime índice
void printIndexHash(const HashTable *table)
{
    for (int i = 0; i < table->size; i++)
    {
        if (table->entries[i].is_active == 1)
        {
            printf("%ls: ", table->entries[i].keyword);
            PositionNodeHash *current = table->entries[i].positions;
            while (current)
            {
                printf("%d", current->position);
                current = current->next;
                if (current)
                    printf(" ");
            }
            printf("\n");
        }
    }
}

// Libera memória
void freeHashTable(HashTable *table)
{
    for (int i = 0; i < table->size; i++)
    {
        if (table->entries[i].is_active == 1)
        {
            free(table->entries[i].keyword);
            PositionNodeHash *current = table->entries[i].positions;
            while (current)
            {
                PositionNodeHash *temp = current;
                current = current->next;
                free(temp);
            }
        }
    }
    free(table->entries);
    free(table);
}