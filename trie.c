#include "trie.h"

// Função para obter o índice do caractere no trie
int get_char_index(wchar_t c)
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
    default:
        if (c >= L'a' && c <= L'z')
            return c - L'a';
        return -1;
    }
}

TrieNode *createTrieNode()
{
    TrieNode *node = (TrieNode *)malloc(sizeof(TrieNode));
    for (int i = 0; i < 128; i++)
    {
        node->children[i] = NULL;
    }
    node->keyword = NULL;
    node->positions = NULL;
    return node;
}

void insertKeyword(TrieNode *root, const wchar_t *word)
{
    TrieNode *node = root;
    size_t len = wcslen(word);
    wchar_t *normalized = (wchar_t *)malloc((len + 1) * sizeof(wchar_t));

    // Converte para minúsculas mas mantém os acentos
    for (size_t i = 0; i < len; i++)
    {
        normalized[i] = towlower(word[i]);
    }
    normalized[len] = L'\0';

    for (int i = 0; normalized[i]; i++)
    {
        int index = get_char_index(normalized[i]);
        if (index < 0)
            continue;

        if (!node->children[index])
        {
            node->children[index] = createTrieNode();
        }
        node = node->children[index];
    }

    if (node->keyword)
        free(node->keyword);
    node->keyword = wcsdup(word);
    free(normalized);
}

void addPosition(PositionNode **head, int position)
{
    position++; // Começa do 1

    PositionNode *newNode = (PositionNode *)malloc(sizeof(PositionNode));
    newNode->position = position;

    if (*head == NULL || (*head)->position > position)
    {
        newNode->next = *head;
        *head = newNode;
        return;
    }

    PositionNode *current = *head;
    while (current->next != NULL && current->next->position < position)
    {
        current = current->next;
    }

    newNode->next = current->next;
    current->next = newNode;
}

void processTextTrie(TrieNode *root, wchar_t word[256], int logicalPosition, int wordLen)
{
    TrieNode *node;
    node = root;

    for (size_t k = 0; k < wordLen; k++)
    {
        int index = get_char_index(word[k]);
        if (index < 0 || !node->children[index])
        {
            node = NULL;
            break;
        }
        node = node->children[index];
    }
    if (node && node->keyword)
    {
        addPosition(&node->positions, logicalPosition);
    }
}

void printPositions(PositionNode *node)
{
    while (node)
    {
        printf("%d", node->position);
        node = node->next;
        if (node)
            printf(" ");
    }
    printf("\n");
}

void printIndex(TrieNode *node)
{
    if (!node)
        return;
    if (node->keyword)
    {
        printf("%ls: ", node->keyword);
        printPositions(node->positions);
    }
    for (int i = 0; i < 128; i++)
    {
        printIndex(node->children[i]);
    }
}

void printTrie(TrieNode *currentNode, wchar_t *currentPath, int currentDepth, int currentIndentation)
{
    if (!currentNode)
        return;

    // Se for um nó que representa uma palavra-chave
    if (currentNode->keyword)
    {
        currentPath[currentDepth] = L'\0'; // Finaliza a string no caminho atual
        for (int i = 0; i < currentIndentation - 1; i++)
            printf("  ");
        printf("  (palavra: %ls)", currentNode->keyword);

        printf("\n");
    }

    // Percorre todos os possíveis caracteres (incluindo acentuados)
    for (int i = 0; i < 128; i++)
    {
        if (currentNode->children[i])
        {
            // Descobre qual caractere corresponde ao índice
            wchar_t currentChar = L' ';
            if (i >= 0 && i < 26)
            {
                currentChar = L'a' + i; // Caracteres de 'a' a 'z'
            }
            else
            {
                // Mapeia caracteres acentuados baseado na lógica inversa de get_char_index
                switch (i)
                {
                case 26:
                    currentChar = L'á';
                    break;
                case 27:
                    currentChar = L'à';
                    break;
                case 28:
                    currentChar = L'ã';
                    break;
                case 29:
                    currentChar = L'â';
                    break;
                case 30:
                    currentChar = L'é';
                    break;
                case 31:
                    currentChar = L'ê';
                    break;
                case 32:
                    currentChar = L'í';
                    break;
                case 33:
                    currentChar = L'ó';
                    break;
                case 34:
                    currentChar = L'ô';
                    break;
                case 35:
                    currentChar = L'õ';
                    break;
                case 36:
                    currentChar = L'ú';
                    break;
                case 37:
                    currentChar = L'ü';
                    break;
                case 38:
                    currentChar = L'ç';
                    break;
                }
            }

            currentPath[currentDepth] = currentChar; // Adiciona o caractere ao caminho atual

            // Imprime a estrutura hierárquica
            for (int j = 0; j < currentIndentation; j++)
                printf("  ");
            printf("|- %lc\n", currentChar);

            // Chamada recursiva para o próximo nó
            printTrie(currentNode->children[i], currentPath, currentDepth + 1, currentIndentation + 1);
        }
    }
}

void freeTrie(TrieNode *node)
{
    if (!node)
        return;
    for (int i = 0; i < 128; i++)
    {
        freeTrie(node->children[i]);
    }
    PositionNode *curr = node->positions;
    while (curr)
    {
        PositionNode *temp = curr;
        curr = curr->next;
        free(temp);
    }
    if (node->keyword)
        free(node->keyword);
    free(node);
}
