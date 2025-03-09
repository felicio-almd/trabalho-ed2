#include "trie.h"

// Função para obter o índice do caractere no trie
int obterIndiceCaractere(wchar_t c)
{
    switch (c)
    {
    case L'á':
        return 26;
    case L'à':
        return 27;
    case L'ã':
        return 28;
    case L'â':
        return 29;
    case L'é':
        return 30;
    case L'ê':
        return 31;
    case L'í':
        return 32;
    case L'ó':
        return 33;
    case L'ô':
        return 34;
    case L'õ':
        return 35;
    case L'ú':
        return 36;
    case L'ü':
        return 37;
    case L'ç':
        return 38;
    case L'@':
        return 39;
    case L'-':
        return 40;
    default:
        if (c >= L'a' && c <= L'z')
            return c - L'a';
        return -1;
    }
}

NoTrie *criarNoTrie()
{
    NoTrie *node = (NoTrie *)malloc(sizeof(NoTrie));
    for (int i = 0; i < 128; i++)
    {
        node->filhos[i] = NULL;
    }
    node->palavraChave = NULL;
    node->posicoes = NULL;
    return node;
}

void inserirPalavraChaveTrie(NoTrie *raiz, const wchar_t *palavra)
{
    NoTrie *node = raiz;
    size_t len = wcslen(palavra);
    wchar_t *normalized = (wchar_t *)malloc((len + 1) * sizeof(wchar_t));

    // Converte para minúsculas mas mantém os acentos
    for (size_t i = 0; i < len; i++)
    {
        normalized[i] = towlower(palavra[i]);
    }
    normalized[len] = L'\0';

    for (int i = 0; normalized[i]; i++)
    {
        int index = obterIndiceCaractere(normalized[i]);
        if (index < 0)
            continue;

        if (!node->filhos[index])
        {
            node->filhos[index] = criarNoTrie();
        }
        node = node->filhos[index];
    }

    if (node->palavraChave)
        free(node->palavraChave);
    node->palavraChave = wcsdup(palavra);
    free(normalized);
}

void adicionarPosicaoTrie(NoPosicaoTrie **cabeca, int posicao)
{
    posicao++; // Começa do 1

    NoPosicaoTrie *newNode = (NoPosicaoTrie *)malloc(sizeof(NoPosicaoTrie));
    newNode->posicao = posicao;

    if (*cabeca == NULL || (*cabeca)->posicao > posicao)
    {
        newNode->proximo = *cabeca;
        *cabeca = newNode;
        return;
    }

    NoPosicaoTrie *current = *cabeca;
    while (current->proximo != NULL && current->proximo->posicao < posicao)
    {
        current = current->proximo;
    }

    newNode->proximo = current->proximo;
    current->proximo = newNode;
}

void processarTextoTrie(NoTrie *raiz, wchar_t palavra[1000], int posicaoLogica, int tamanhoPalavra)
{
    NoTrie *no;
    no = raiz;

    for (size_t k = 0; k < tamanhoPalavra; k++)
    {
        // pega caractere um por um
        int index = obterIndiceCaractere(palavra[k]);
        if (index < 0 || !no->filhos[index])
        {
            no = NULL;
            break;
        }
        no = no->filhos[index];
    }
    if (no && no->palavraChave)
    {
        adicionarPosicaoTrie(&no->posicoes, posicaoLogica);
    }
}

void imprimirPosicoesTrie(NoPosicaoTrie *no)
{
    while (no)
    {
        printf("%d", no->posicao);
        no = no->proximo;
        if (no)
            printf(" ");
    }
    printf("\n");
}

void imprimirIndiceTrie(NoTrie *node)
{
    if (!node)
        return;
    if (node->palavraChave)
    {
        printf("%ls: ", node->palavraChave);
        imprimirPosicoesTrie(node->posicoes);
    }
    for (int i = 0; i < 128; i++)
    {
        imprimirIndiceTrie(node->filhos[i]);
    }
}

void imprimirTrie(NoTrie *noAtual, wchar_t *caminhoAtual, int profundidadeAtual, int indentacaoAtual)
{
    if (!noAtual)
        return;

    // Se for um nó que representa uma palavra-chave
    if (noAtual->palavraChave)
    {
        caminhoAtual[profundidadeAtual] = L'\0'; // Finaliza a string no caminho atual
        for (int i = 0; i < indentacaoAtual - 1; i++)
            printf("  ");
        printf("  (palavra: %ls)", noAtual->palavraChave);

        printf("\n");
    }

    // Percorre todos os possíveis caracteres (incluindo acentuados)
    for (int i = 0; i < 128; i++)
    {
        if (noAtual->filhos[i])
        {
            // Descobre qual caractere corresponde ao índice
            wchar_t charAtual = L' ';
            if (i >= 0 && i < 26)
            {
                charAtual = L'a' + i; // pega caracteres de 'a' a 'z'
            }
            else
            {
                // mapeia caracteres acentuados baseado na lógica inversa de obterIndiceCaractere
                switch (i)
                {
                case 26:
                    charAtual = L'á';
                    break;
                case 27:
                    charAtual = L'à';
                    break;
                case 28:
                    charAtual = L'ã';
                    break;
                case 29:
                    charAtual = L'â';
                    break;
                case 30:
                    charAtual = L'é';
                    break;
                case 31:
                    charAtual = L'ê';
                    break;
                case 32:
                    charAtual = L'í';
                    break;
                case 33:
                    charAtual = L'ó';
                    break;
                case 34:
                    charAtual = L'ô';
                    break;
                case 35:
                    charAtual = L'õ';
                    break;
                case 36:
                    charAtual = L'ú';
                    break;
                case 37:
                    charAtual = L'ü';
                    break;
                case 38:
                    charAtual = L'ç';
                    break;
                case 39:
                    charAtual = L'@';
                    break;
                case 40:
                    charAtual = L'-';
                    break;
                }
            }

            caminhoAtual[profundidadeAtual] = charAtual; // Adiciona o caractere ao caminho atual

            // Imprime a estrutura hierárquica
            for (int j = 0; j < indentacaoAtual; j++)
                printf("  ");
            printf("|- %lc\n", charAtual);

            // Chamada recursiva para o próximo nó
            imprimirTrie(noAtual->filhos[i], caminhoAtual, profundidadeAtual + 1, indentacaoAtual + 1);
        }
    }
}

void freeTrie(NoTrie *node)
{
    if (!node)
        return;
    for (int i = 0; i < 128; i++)
    {
        freeTrie(node->filhos[i]);
    }
    NoPosicaoTrie *curr = node->posicoes;
    while (curr)
    {
        NoPosicaoTrie *temp = curr;
        curr = curr->proximo;
        free(temp);
    }
    if (node->palavraChave)
        free(node->palavraChave);
    free(node);
}
