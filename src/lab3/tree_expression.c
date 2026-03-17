#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct Node {
    char data[16];
    struct Node *left, *right;
} Node;

Node* initNode(char* data){
    Node* n = (Node*)malloc(sizeof(Node));
    if(!n) exit(1);
    strcpy(n->data, data);
    n->left = n->right = NULL;
    return n;
}

int isEqual(Node* a, Node* b){
    if(!a && !b) return 1;
    if(!a || !b) return 0;
    return(strcmp(a->data, b->data) == 0)
     && isEqual(a->left, b->left) 
     && isEqual(a->right, b->right);
}

void printTree(Node* root, int level){
    if(!root) return;
    printTree(root->right, level + 1);
    for(int i = 0; i < level; ++i) printf("    ");
    printf("%s\n", root->data);
    printTree(root->left, level + 1);
}

int priority(char a){
    if(a == '+' || a == '-') return 1;
    if(a == '*' || a == '/') return 2;
    return 0;
}

Node* buildTree(char* expr){
    Node* nodes[100];
    char ops[100];
    int ns = 0, os = 0;

    for(int i = 0; expr[i]; ++i){
        if(isspace(expr[i])) continue;
        if(isalnum(expr[i])){
            char buf[16] = {0};
            int k = 0;
            while (isalnum(expr[i])) buf[k++] = expr[i++];
            i--;
            nodes[ns++] = initNode(buf);
        }
        else if (expr[i] == '('){
            ops[os++] = '(';
        }
        else if(expr[i] == ')'){
            while (os > 0 && ops[os - 1] != '('){
                Node* r = nodes[--ns];
                Node* l = nodes[--ns];
                char opStr[2] = {ops[--os], 0};
                Node *opNode = initNode(opStr);
                opNode->left = l; opNode->right = r;
                nodes[ns++] = opNode;
            }
            os--;
        }
        else{
            while(os > 0 && ops[os - 1] != '(' && priority(ops[os-1]) >= priority(expr[i])){
                Node *r = nodes[--ns];
                Node *l = nodes[--ns];
                char opStr[2] = {ops[--os], 0};
                Node *opNode = initNode(opStr);
                opNode->left = l, opNode->right = r;
                nodes[ns++] = opNode;
            }
            ops[os++] = expr[i];
        }
    }
    while(os > 0){
        Node* r = nodes[--ns];
        Node* l = nodes[--ns];
        char opStr[2] = {ops[--os], 0};
        Node* opNode = initNode(opStr);
        opNode->left = l; opNode->right = r;
        nodes[ns++] = opNode;
    }
    return nodes[0];
}

Node* factorize(Node *root){
    if(!root) return NULL;
    root->left = factorize(root->left);
    root->right = factorize(root->right);

    if(strcmp(root->data, "+") == 0){
        Node *L = root->left, *R = root->right;
        if(L && R && strcmp(L->data, "*") == 0 && strcmp(R->data, "*") == 0){
            Node *common = NULL, *remL = NULL, *remR = NULL;
            
            if(isEqual(L->left, R->left)) {
                common = L->left;
                remL = L->right;
                remR = R->right;
            }
            else if(isEqual(L->left, R->right)) {
                common = L->left;
                remL = L->right;
                remL = R->left;
            }
            else if(isEqual(L->right, R->left)){
                common = L->right;
                remL = L->left;
                remR = R->right;
            }
            else if(isEqual(L->right, R->right)){
                common = L->right;
                remL = L->left;
                remR = R->left;
            }

            if(common) {
                Node *newPlus = initNode("+");
                newPlus->left = remL;
                newPlus->right = remR;
                Node *newMul = initNode("*");
                newMul->left = common;
                newMul->right = newPlus;
                return newMul;
            }
        }
    }
    return root;
}

void printInfix(Node *root){
    if(!root) return;
    if(root->left){
        printf("(");
        printInfix(root->left);
    }
    printf("%s", root->data);
    if(root->right){
        printInfix(root->right);
        printf(")");
    }
}

int main(void){
    char expr[100] = "a*b+a*c";
    printf("Исходное выражение: %s\n", expr);

    Node *root = buildTree(expr);
    printf("\nСтруктура дерева:\n");
    printTree(root, 0);

    root = factorize(root);

    printf("\nПосле вынесения множителя\n");
    printInfix(root);
    printf("\nНовое дерево:\n");
    printTree(root, 0);

    return 0;
}