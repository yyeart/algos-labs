#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {RED, BLACK} Color;

typedef struct Node {
    char key[7];
    double value;
    Color color;
    struct Node *left, *right, *parent;
} Node;

Node* NIL;

void initNIL(){
    NIL = (Node*)malloc(sizeof(Node));
    NIL->color = BLACK;
    NIL->left = NIL->right = NIL->parent = NIL;
}

void freeTree(Node* root){
    if(root == NIL) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

void leftRotate(Node** root, Node* x){
    if(x->right == NIL) return;
    Node* y = x->right;
    x->right = y->left;
    if(y->left != NIL){
        y->left->parent = x;
    }
    y->parent = x->parent;
    if(x->parent == NIL){
        *root = y;
    }
    else if(x == x->parent->left){
        x->parent->left = y;
    }
    else {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}

void rightRotate(Node** root, Node *y){
    if (y->left == NIL) return;
    Node* x = y->left;
    y->left = x->right;
    if(x->right != NIL){
        x->right->parent = y;
    }
    x->parent = y->parent;
    if(y->parent == NIL){
        *root = x;
    }
    else if(y == y->parent->right){
        y->parent->right = x;
    }
    else{
        y->parent->left = x;
    }
    x->right = y;
    y->parent = x;
}

Node* createNode(char* key, double value){
    Node* n = (Node*)malloc(sizeof(Node));
    strcpy(n->key, key);
    n->value = value;
    n->color = RED;
    n->left = n->right = n->parent = NIL;
    return n;
}

void insertFix(Node** root, Node* z){
    while(z->parent->color == RED){
        if(z->parent == z->parent->parent->left){
            Node* y = z->parent->parent->right;
            if(y->color == RED){
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            }
            else {
                if(z == z->parent->right){
                    z = z->parent;
                    leftRotate(root, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rightRotate(root, z->parent->parent);
            }
        }
        else {
            Node* y = z->parent->parent->left;
            if(y->color == RED){
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            }
            else {
                if(z == z->parent->left){
                    z = z->parent;
                    rightRotate(root, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                leftRotate(root, z->parent->parent);
            }
        }
    }
    (*root)->color = BLACK;
}

void insert(Node** root, char* key, double value){
    Node* z = createNode(key, value);
    Node* y = NIL;
    Node* x = *root;
    while(x != NIL){
        y = x;
        int cmp = strcmp(z->key, x->key);
        if(cmp == 0){
            free(z);
            return; // дубликат ключа
        }
        else if(cmp < 0) {
            x = x->left;
        }
        else{
            x = x->right;
        }
    }
    z->parent = y;
    if(y == NIL){
        *root = z;
    }
    else if(strcmp(z->key, y->key) < 0){
        y->left = z;
    }
    else {
        y->right = z;
    }
    insertFix(root, z);
}

Node* search(Node* root, char* key){
    while(root != NIL){
        int cmp = strcmp(key, root->key);
        if(cmp == 0) return root;
        else if(cmp < 0) root = root->left;
        else root = root->right;
    }
    return NIL;
}

void printTree(FILE* fo, Node* root, int depth){
    if(root == NIL) return;
    printTree(fo, root->right, depth+1);
    for(int i = 0; i < depth; ++i) fprintf(fo, "   ");
    fprintf(fo, "%s(%.2f)[%c]\n", root->key, root->value, root->color == RED ? 'R' : 'B');
    printTree(fo, root->left, depth+1);
}

Node* treeMin(Node* x){
    while(x->left != NIL){
        x = x->left;
    }
    return x;
}

void transplant(Node** root, Node* u, Node* v){
    if(u->parent == NIL){
        *root = v;
    }
    else if(u == u->parent->left){
        u->parent->left = v;
    }
    else {
        u->parent->right = v;
    }
    v->parent = u->parent;
}

void deleteFix(Node** root, Node* x){
    while(x != *root && x->color == BLACK){
        if(x == x->parent->left){
            Node* w = x->parent->right;
            if(w->color == RED){
                w->color = BLACK;
                x->parent->color = RED;
                leftRotate(root, x->parent);
                w = x->parent->right;
            }
            if(w->left->color == BLACK && w->right->color == BLACK){
                w->color = RED;
                x = x->parent;
            }
            else {
                if(w->right->color == BLACK){
                    w->left->color = BLACK;
                    w->color = RED;
                    rightRotate(root, w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;
                leftRotate(root, x->parent);
                x = *root;
            }
        }
        else {
            Node* w = x->parent->left;
            if(w->color == RED){
                w->color = BLACK;
                x->parent->color = RED;
                rightRotate(root, x->parent);
                w = x->parent->left;
            }
            if(w->right->color == BLACK && w->left->color == BLACK){
                w->color = RED;
                x = x->parent;
            }
            else {
                if(w->left->color == BLACK){
                    w->right->color = BLACK;
                    w->color = RED;
                    leftRotate(root, w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color = BLACK;
                rightRotate(root, x->parent);
                x = *root;
            }
        }
    }
    x->color = BLACK;
}

void deleteNode(Node** root, char* key){
    Node* z = search(*root, key);
    if(z == NIL) return;

    Node* y = z;
    Node* x;
    Color yOriginal = y->color;
    if(z->left == NIL){
        x = z->right;
        transplant(root, z, z->right);
    }
    else if(z->right == NIL){
        x = z->left;
        transplant(root, z, z->left);
    }
    else {
        y = treeMin(z->right);
        yOriginal = y->color;
        x = y->right;
        if(y->parent == z){
            if(x != NIL) x->parent = y;
        }
        else {
            transplant(root, y, y->right);
            if(y != z->right){
                y->right = z->right;
                y->right->parent = y;
            }
        }
        transplant(root, z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }
    if(yOriginal == BLACK) deleteFix(root, x);
    free(z);
}

void processFile(const char *in, const char *out){
    FILE *fi = fopen(in, "r");
    FILE *fo = fopen(out, "w");
    Node* root = NIL;
    int cmd;
    char key[7];
    double val;
    while(fscanf(fi, "%d", &cmd) != EOF){
        fprintf(fo, "%d ", cmd);
        switch(cmd){
            case 1:
                fscanf(fi, "%s %lf", key, &val);
                fprintf(fo, "%s %.2f\n", key, val);
                insert(&root, key, val);
                break;
            case 2:
                fscanf(fi, "%s", key);
                fprintf(fo, "%s\n", key);
                deleteNode(&root, key);
                break;
            case 3:
                fprintf(fo, "\n");
                printTree(fo, root, 0);
                break;
            case 4:
                fscanf(fi, "%s", key);
                Node* res = search(root, key);
                fprintf(fo, "%s -> ", key);
                if(res != NIL) fprintf(fo, "%.2f\n", res->value);
                else fprintf(fo, "not found\n");
                break;
        }
    }
    freeTree(root);
    fclose(fi);
    fclose(fo);
}

int main(void){
    initNIL();
    processFile("input.txt", "output.txt");
    free(NIL);
    return 0;
}