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

    char texto[100];
    char palavras_chave[100];
    int opcao;

    // Ler o arquivo de texto
    printf("Digite o texto que deseja ser lido: ");
    scanf("%s", &texto);
    char *text = read_file(texto);
    if (!text)
    {
        fprintf(stderr, "Erro ao ler o arquivo de texto.\n");
        return EXIT_FAILURE;
    }

    // Carregar palavras-chave
    printf("Digite o nome do arquivo de palavras chave: ");
    scanf("%s", &palavras_chave);
    load_keywords(palavras_chave);

    printf("Qual metodo deseja o indice remissivo? 1-arvore digital 2-Tabela Hash");
    scanf("%d", &opcao);
    if (opcao == 1)
    {
        // Criar a raiz da Trie
        TrieNode *trie_root = create_node();

        // Criar índice remissivo usando Trie
        create_index_trie(trie_root, text);
        printf("Indice remissivo usando Trie:\n");
        print_index_trie(trie_root);
        delete_index_trie(trie_root);
    }
    else if (opcao == 2)
    {
        // Criar índice remissivo usando Tabela Hash
        create_index_hash(text);
        printf("Indice remissivo usando Tabela Hash:\n");
        print_index_hash();
        delete_index_hash();
    }

    // Liberar memória
    free(text);
    return 0;
}