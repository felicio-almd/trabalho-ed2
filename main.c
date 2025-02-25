#include "trie.h"

int main()
{
    setlocale(LC_ALL, "pt_BR.UTF-8");

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
    printf("Imprimindo via arvore digital:\n\n");
    printIndex(root);
    printf("\n");
    freeTrie(root);

    return 0;
}