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
    n->data[15] = '\0';
    n->left = n->right = NULL;
    return n;
}

void freeTree(Node* root){
    if(!root) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
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

void processOperator(Node** nodes, int* ns, char* ops, int* os){
    Node *r = nodes[--(*ns)];
    Node *l = nodes[--(*ns)];
    char opS[2] = {ops[--(*os)], 0};
    Node* opN = initNode(opS);
    opN->left = l;
    opN->right = r;
    nodes[(*ns)++] = opN;
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
                processOperator(nodes, &ns, ops, &os);
            }
            os--;
        }
        else{
            while(os > 0 && ops[os - 1] != '(' && priority(ops[os-1]) >= priority(expr[i])){
                processOperator(nodes, &ns, ops, &os);
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

Node* copyTree(Node* root){
    if(!root) return NULL;
    Node *newNode = initNode(root->data);
    newNode->left = copyTree(root->left);
    newNode->right = copyTree(root->right);
    return newNode;
}

Node* factorize(Node *root){
    if(!root) return NULL;
    root->left = factorize(root->left);
    root->right = factorize(root->right);

    if(strcmp(root->data, "+") == 0){
        Node *L = root->left, *R = root->right;
        if(L && R && strcmp(L->data, "*") == 0 && strcmp(R->data, "*") == 0){
            Node *common = NULL, *ostatokL = NULL, *ostatokR = NULL;
            
            // (A*B)+(A*C)
            if(isEqual(L->left, R->left)) {
                common = L->left;
                ostatokL = L->right;
                ostatokR = R->right;
            }
            // (A*B)+(C*A)
            else if(isEqual(L->left, R->right)) {
                common = L->left;
                ostatokL = L->right;
                ostatokL = R->left;
            }
            // (B*A)+(A*C)
            else if(isEqual(L->right, R->left)){
                common = L->right;
                ostatokL = L->left;
                ostatokR = R->right;
            }
            // (B*A)+(C*A)
            else if(isEqual(L->right, R->right)){
                common = L->right;
                ostatokL = L->left;
                ostatokR = R->left;
            }

            if(common) {
                Node* newCommon = copyTree(common);
                Node* newPlus = initNode("+");
                newPlus->left = copyTree(ostatokL);
                newPlus->right = copyTree(ostatokR);
                Node* newMul = initNode("*");
                newMul->left = newCommon;
                newMul->right = newPlus;

                freeTree(root);
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
    char* tests[] = {
        "a*b*d+a*b*c",
        "x*y+z*w",
        "a*b*c+a*b*d",
        "(x+y)*a+(x+y)*b"
    };
    for(int i = 0; i < 4; ++i){
        printf("\nТест %d: %s\n", i+1, tests[i]);
        Node* root = buildTree(tests[i]);
        printf("Исходное дерево:\n");
        printTree(root, 0);
        root = factorize(root);
        printf("\nПосле преобразования:\n");
        printInfix(root);
        printf("\nДерево:\n");
        printTree(root, 0);

        freeTree(root);
    }

    return 0;
}