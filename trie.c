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

// essa função é meio estranha
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

// essa tbm
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

void processText(TrieNode *root, const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        perror("Erro ao abrir arquivo de texto");
        exit(1);
    }

    int logicalPosition = 0;
    wchar_t buffer[1024];

    while (fgetws(buffer, sizeof(buffer) / sizeof(wchar_t), file))
    {
        size_t lineLen = wcslen(buffer);

        for (size_t i = 0; i < lineLen;)
        {
            TrieNode *node = root;
            wchar_t wordBuffer[256];
            int wordLen = 0;
            size_t j;

            // Coleta a palavra mantendo os acentos
            for (j = i; j < lineLen && iswalpha(buffer[j]) && wordLen < 255; j++)
            {
                wordBuffer[wordLen++] = towlower(buffer[j]);
            }
            wordBuffer[wordLen] = L'\0';

            if (wordLen > 0)
            {
                node = root;
                for (size_t k = 0; k < wordLen; k++)
                {
                    int index = get_char_index(wordBuffer[k]);
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

            logicalPosition++;
            i++;
        }
    }
    fclose(file);
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
