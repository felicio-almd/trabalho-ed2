#include "trie.h"
#include "comum.h"

int main()
{
    setlocale(LC_ALL, "pt_BR.UTF-8");

    TrieNode *root = processText("trie", "texto.txt");
    printf("Imprimindo via arvore digital:\n\n");
    printIndex(root);
    printf("\n");
    freeTrie(root);

    return 0;
}
