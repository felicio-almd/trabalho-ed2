#include "trie.h"
#include <stdlib.h>
#include <string.h>

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
    for (int i = 0; word[i] != '\0'; i++)
    {
        int index = word[i] - 'a';
        if (!current->children[index])
        {
            current->children[index] = create_node();
        }
        current = current->children[index];
    }
    current->is_end_of_word = true;
    current->positions[current->position_count++] = position;
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