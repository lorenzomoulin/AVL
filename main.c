#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Definição da estrutura Node
typedef struct Node {
    char chave[100];
    struct Node *left;
    struct Node *right;
    int height;
} Node;

// Função para criar um novo nó
Node* newNode(char* chave) {
    Node* node = (Node*)malloc(sizeof(Node));
    strcpy(node->chave, chave);
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return node;
}

// Função para obter a altura de um nó
int height(Node *N) {
    if (N == NULL)
        return 0;
    return N->height;
}

// Função para realizar uma rotação à direita
Node* rightRotate(Node *y) {
    Node *x = y->left;
    Node *T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = (height(y->left) > height(y->right) ? height(y->left) : height(y->right)) + 1;
    x->height = (height(x->left) > height(x->right) ? height(x->left) : height(x->right)) + 1;

    return x;
}

// Função para realizar uma rotação à esquerda
Node* leftRotate(Node *x) {
    Node *y = x->right;
    Node *T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = (height(x->left) > height(x->right) ? height(x->left) : height(x->right)) + 1;
    y->height = (height(y->left) > height(y->right) ? height(y->left) : height(y->right)) + 1;

    return y;
}

// Função para obter o balanço de um nó
int getBalance(Node *N) {
    if (N == NULL)
        return 0;
    return height(N->left) - height(N->right);
}


// Função para inserir um nó na árvore AVL
Node* insert(Node* node, char* chave) {
    if (node == NULL)
        return newNode(chave);

    if (strcmp(chave, node->chave) < 0)
        node->left = insert(node->left, chave);
    else if (strcmp(chave, node->chave) > 0)
        node->right = insert(node->right, chave);
    else
        return node;

    node->height = 1 + (height(node->left) > height(node->right) ? height(node->left) : height(node->right));

    int balance = getBalance(node);

    // Caso Left Left
    if (balance > 1 && strcmp(chave, node->left->chave) < 0)
        return rightRotate(node);

    // Caso Right Right
    if (balance < -1 && strcmp(chave, node->right->chave) > 0)
        return leftRotate(node);

    // Caso Left Right
    if (balance > 1 && strcmp(chave, node->left->chave) > 0) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // Caso Right Left
    if (balance < -1 && strcmp(chave, node->right->chave) < 0) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

// Função para buscar um nó na árvore AVL
Node* find(Node* node, char* chave) {
    if (node == NULL)
        return NULL;

    if (strcmp(chave, node->chave) == 0)
        return node;

    if (strcmp(chave, node->chave) < 0)
        return find(node->left, chave);

    return find(node->right, chave);
}

// Função para percorrer a árvore em pré-ordem
void preOrder(Node *root) {
    if (root != NULL) {
        printf("Palavra: %s\n", root->chave);
        preOrder(root->left);
        preOrder(root->right);
    }
}

void inOrder(Node *root) {
    if (root != NULL) {
        inOrder(root->left);
        printf("Palavra: %s\n", root->chave);
        inOrder(root->right);
    }
}

int compareStrings(const void *a, const void *b) {
    const char *strA = (const char *)a;
    const char *strB = (const char *)b;
    return strcmp(strA, strB);
}

// Função para trocar dois elementos
void swap(char *x, char *y) {
    char temp = *x;
    *x = *y;
    *y = temp;
}

// Função para reverter uma parte do array
void reverse(char *begin, char *end) {
    while (begin < end) {
        swap(begin, end);
        begin++;
        end--;
    }
}

// Implementação da função next_permutation
bool next_permutation(char *str, int n) {
    int i = n - 2;

    // Encontra a primeira posição 'i' tal que str[i] < str[i + 1]
    while (i >= 0 && str[i] >= str[i + 1]) {
        i--;
    }

    // Se não há tal posição, a permutação atual é a maior
    if (i < 0) {
        return false;
    }

    // Encontra a primeira posição 'j' tal que str[j] > str[i]
    int j = n - 1;
    while (str[j] <= str[i]) {
        j--;
    }

    // Troca os elementos str[i] e str[j]
    swap(&str[i], &str[j]);

    // Reverte a sequência de str[i + 1] até str[n - 1]
    reverse(str + i + 1, str + n - 1);

    return true;
}

// Função principal
int main(int argc, char **argv) {
    Node *root = NULL;
    char palavra[100];
    FILE *f = fopen("palavras.txt", "r");
    while (fscanf(f, "%s", palavra) != EOF) {
        root = insert(root, palavra);
    }  
    fclose(f);

    int n;
    printf("Digite o numero de caracteres: ");
    scanf("%d", &n);
    char alfabeto[64];
    printf("Digite as letras separadas por espaco: ");
    for (int i = 0 ; i < n; ++i) {
        scanf(" %c", &alfabeto[i]);
    }
    alfabeto[n] = '\0';
    qsort(alfabeto, n, sizeof(char), compareStrings);
    
    do {
        if (find(root, alfabeto) != NULL) {
            printf("%s | valida\n", alfabeto);
        }
        else
            printf("%s | invalida\n", alfabeto);
    } while (next_permutation(alfabeto, n));
    return 0;
}
