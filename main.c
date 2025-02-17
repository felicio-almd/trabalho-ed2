#include <stdio.h>
#include <stdlib.h>
#include "file_utils.h"
#include "trie.h"
#include "hash.h"
#include "index.h"
#include <locale.h>

int main()
{
    setlocale(LC_ALL, "pt_BR.UTF-8");
    // Ler o arquivo de texto
    char *text = read_file("texto.txt");
    if (!text)
    {
        fprintf(stderr, "Erro ao ler o arquivo de texto.\n");
        return EXIT_FAILURE;
    }

    // Carregar palavras-chave
    load_keywords("palavras_chave.txt");

    // Criar a raiz da Trie
    TrieNode *trie_root = create_node();

    // Criar índice remissivo usando Trie
    create_index_trie(trie_root, text);
    printf("Indice remissivo usando Trie:\n");
    print_index_trie(trie_root);

    printf("\n");

    // Criar índice remissivo usando Tabela Hash
    create_index_hash(text);
    printf("Indice remissivo usando Tabela Hash:\n");
    print_index_hash();

    // Liberar memória
    delete_index_trie(trie_root);
    delete_index_hash();
    free(text);

    return 0;
}