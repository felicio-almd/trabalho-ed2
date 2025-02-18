#include "trie.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void normalize_char(char *c)
{
    // Remove acentos e converte para minúscula
    switch (*c)
    {
    case 'á':
    case 'à':
    case 'â':
    case 'ã':
    case 'ä':
        *c = 'a';
        break;
    case 'é':
    case 'è':
    case 'ê':
    case 'ë':
        *c = 'e';
        break;
    case 'í':
    case 'ì':
    case 'î':
    case 'ï':
        *c = 'i';
        break;
    case 'ó':
    case 'ò':
    case 'ô':
    case 'õ':
    case 'ö':
        *c = 'o';
        break;
    case 'ú':
    case 'ù':
    case 'û':
    case 'ü':
        *c = 'u';
        break;
    case 'ç':
        *c = 'c';
        break;
    default:
        *c = tolower(*c);
        break;
    }
}

TrieNode *create_node()
{
    TrieNode *node = (TrieNode *)malloc(sizeof(TrieNode));
    node->is_end_of_word = false;
    node->position_count = 0;
    for (int i = 0; i < ALPHABET_SIZE; i++)
    {
        node->children[i] = NULL;
    }
    return node;
}

void insert_trie(TrieNode *root, const char *word, int position)
{
    TrieNode *current = root;
    char normalized_char;

    for (int i = 0; word[i] != '\0' && word[i] != '.' && word[i] != ','; i++)
    {
        normalized_char = word[i];
        normalize_char(&normalized_char); // Remove acentos e converte para minúscula

        // Interrompe se não for uma letra após normalização
        if (!isalpha(normalized_char))
        {
            break;
        }

        int index = normalized_char - 'a'; // Índice baseado em letras minúsculas

        if (index < 0 || index >= ALPHABET_SIZE)
        {
            continue; // Ignora caracteres fora do intervalo (ex: 'ç' normalizado para 'c')
        }

        if (!current->children[index])
        {
            current->children[index] = create_node();
        }
        current = current->children[index];
    }

    if (current != root)
    { // Evita marcar a raiz como fim de palavra
        current->is_end_of_word = true;
        current->positions[current->position_count++] = position;
    }
}

TrieNode *search_trie(TrieNode *root, const char *word)
{
    TrieNode *current = root;
    for (int i = 0; word[i] != '\0'; i++)
    {
        int index = word[i] - 'a';
        if (!current->children[index])
        {
            return NULL;
        }
        current = current->children[index];
    }
    return current;
}

void delete_index_trie(TrieNode *root)
{
    if (!root)
        return;
    for (int i = 0; i < ALPHABET_SIZE; i++)
    {
        delete_index_trie(root->children[i]);
    }
    free(root);
}