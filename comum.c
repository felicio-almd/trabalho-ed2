#include "comum.h"
#include "processamento.h"
#include "trie.h"

TrieNode OpenTrie()
{
    TrieNode *root = createTrieNode();
    FILE *keyFile = fopen("palavras_chave.txt", "r");
    if (!keyFile)
    {
        perror("Erro ao abrir arquivo de palavras-chave");
        return;
    }

    wchar_t keyword[256];
    while (fwscanf(keyFile, L"%255ls", keyword) == 1)
    {
        insertKeyword(root, keyword);
    }
    fclose(keyFile);

    return *root;
}

void ProcessTextG(int type, char buscaType, const char *filename)
{
    wchar_t *text = ler_arquivo(filename);
    size_t textLen = wcslen(text);
    int logicalposition = 0;
    if (buscaType = "trie")
    {
        TrieNode *root = OpenTrie();
    }
    for (size_t i = 0; i < textLen;)
    {
        wchar_t word[256];
        int wordLen = 0;
        size_t j;

        for (j = i; j < textLen && iswalpha(text[j]) && wordLen < 255; j++)
        {
            word[wordLen++] = towlower(text[j]);
        }
        word[wordLen] = L'\0';
        if (word)
            logicalposition++;
        i++;
    }
}
