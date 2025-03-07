#include "trie.h"
#include "hash.h"

int main()
{
    setlocale(LC_ALL, "pt_BR.UTF-8");

    // --------------- IMPLEMENTAÇÃO COM TRIE ---------------
    printf("=== IMPLEMENTAÇÃO COM TRIE ===\n");
    TrieNode *root = createTrieNode();
    FILE *keyFile = fopen("palavras_chave.txt", "r");
    if (!keyFile)
    {
        perror("Erro ao abrir arquivo de palavras-chave");
        return 1;
    }

    wchar_t keyword[256];
    while (fwscanf(keyFile, L"%255ls", keyword) == 1)
    {
        insertKeyword(root, keyword);
    }
    fclose(keyFile);

    processText(root, "texto.txt");
    printf("Imprimindo via árvore digital (trie):\n\n");
    printIndex(root);

    // --------------- IMPLEMENTAÇÃO COM HASH ---------------
    printf("\n=== IMPLEMENTAÇÃO COM TABELA HASH ===\n");
    HashTable *table = createHashTable(101); // Tamanho primo da tabela

    // Lê o arquivo de palavras-chave novamente
    keyFile = fopen("palavras_chave.txt", "r");
    if (!keyFile)
    {
        perror("Erro ao abrir arquivo de palavras-chave");
        freeTrie(root);
        freeHashTable(table);
        return 1;
    }

    while (fwscanf(keyFile, L"%255ls", keyword) == 1)
    {
        insertKeywordHash(table, keyword);
    }
    fclose(keyFile);

    // Processa o arquivo de texto
    processTextHash(table, "texto.txt");
    printf("Imprimindo via tabela hash:\n\n");
    printIndexHash(table);

    // Libera a memória
    freeTrie(root);
    freeHashTable(table);

    return 0;
}