#include "index.h"
#include "file_utils.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

void create_index_trie(TrieNode *root, const char *text) {
    char word[MAX_KEYWORD_LENGTH];
    int position = 0;
    while (sscanf(text + position, "%s", word) == 1) {
        // Converte a palavra do texto para minúsculas
        for (int i = 0; word[i]; i++) {
            word[i] = tolower(word[i]);
        }
        // Compara com as palavras-chave (já em minúsculas)
        for (int i = 0; i < keyword_count; i++) {
            if (strcmp(word, keywords[i]) == 0) {
                insert_trie(root, word, position + 1);
            }
        }
        position += strlen(word) + 1;
    }
}

void create_index_hash(const char *text)
{
    char word[MAX_KEYWORD_LENGTH];
    int position = 0;
    while (sscanf(text + position, "%s", word) == 1)
    {
        for (int i = 0; i < keyword_count; i++)
        {
            if (strcmp(word, keywords[i]) == 0)
            {
                insert_hash(word, position + 1);
            }
        }
        position += strlen(word) + 1;
    }
}

void print_index_trie(TrieNode *root)
{
    for (int i = 0; i < keyword_count; i++)
    {
        TrieNode *node = search_trie(root, keywords[i]);
        if (node)
        {
            printf("%s: ", keywords[i]);
            for (int j = 0; j < node->position_count; j++)
            {
                printf("%d ", node->positions[j]);
            }
            printf("\n");
        }
    }
}

void print_index_hash()
{
    for (int i = 0; i < keyword_count; i++)
    {
        HashNode *node = search_hash(keywords[i]);
        if (node)
        {
            printf("%s: ", keywords[i]);
            for (int j = 0; j < node->position_count; j++)
            {
                printf("%d ", node->positions[j]);
            }
            printf("\n");
        }
    }
}