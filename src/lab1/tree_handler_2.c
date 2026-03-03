#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node *firstChild;
    struct Node *nextSibling;
} Node;

Node* createNode(int val){
    Node *n = (Node*)malloc(sizeof(Node));
    if(!n) return NULL;
    n->data = val;
    n->firstChild = n->nextSibling = NULL;
    return n;
}

void addChild(Node* parent, int val){
    Node *n = createNode(val);
    if(parent->firstChild == NULL){
        parent->firstChild = n;
    } else {
        Node *tmp = parent->firstChild;
        while (tmp->nextSibling != NULL){
            tmp = tmp->nextSibling;
        }
        tmp->nextSibling = n;
    }
}

void printTree(Node* root, int depth){
    if(!root) return;
    for(int i = 0; i < depth; ++i) printf("    ");
    printf("%d\n", root->data);
    printTree(root->firstChild, depth + 1);
    printTree(root->nextSibling, depth);
}

void freeTree(Node* root){
    if(!root) return;
    freeTree(root->firstChild);
    freeTree(root->nextSibling);
    free(root);
}

Node* deleteNode(Node* root, int val){
    if(!root) return NULL;
    if(root->data == val){
        freeTree(root->firstChild);
        Node *n = root->nextSibling;
        free(root);
        return n;
    }
    Node *prev = NULL;
    Node *curr = root->firstChild;
    while (curr != NULL) {
        if(curr->data == val){
            if(prev == NULL) root->firstChild = curr->nextSibling;
            else prev->nextSibling = curr->nextSibling;
            curr->nextSibling = NULL;
            freeTree(curr);
            return root;
        }
        deleteNode(curr, val);
        prev = curr;
        curr = curr->nextSibling;
    }
    return root;
}

int checkLevels(Node* root, int currentLevel, int* leafLevel){
    if(!root) return 1;
    if(root->firstChild == NULL){
        if(*leafLevel == -1){
            *leafLevel = currentLevel;
            return 1;
        }
        return (currentLevel == *leafLevel);
    }
    Node *child = root->firstChild;
    while(child != NULL){
        if(!checkLevels(child, currentLevel + 1, leafLevel)) return 0;
        child = child->nextSibling;
    }
    return 1;
}

Node* findNode(Node* root, int val){
    if(!root) return NULL;
    if(root->data == val) return root;
    Node *found = findNode(root->firstChild, val);
    if(found) return found;
    return findNode(root->nextSibling, val);
}

int main(void){
    Node *root = NULL;
    int option, val, parent;
    while(1){
        printf("1 - Insert\n");
        printf("2 - Print\n");
        printf("3 - Delete\n");
        printf("4 - Function\n");
        printf("0 - Exit\n");
        scanf("%d", &option);
        switch(option){
            case 1:
                printf("Enter new value: ");
                scanf("%d", &val);
                if(!root) root = createNode(val);
                else {
                    printf("Enter parent val: ");
                    scanf("%d", &parent);
                    Node *p = findNode(root, parent);
                    if(p) addChild(p, val);
                    else printf("Parent not found\n");
                }
                break;
            case 2:
                if(!root) printf("Tree is empty\n");
                else{
                    printf("---\n");
                    printTree(root, 0);
                    printf("---\n");
                }
                break;
            case 3:
                if(!root) printf("Tree is empty\n");
                else{
                    printf("Enter value: ");
                    scanf("%d", &val);
                    root = deleteNode(root, val);
                }
                break;
            case 4:
                int leafLevel = -1;
                if(!root) printf("Tree is empty\n");
                else if(checkLevels(root, 0, &leafLevel)){
                    printf("All leaves are on the level %d\n", leafLevel);
                }
                else printf("There are leaves of different levels\n");
                break;
            case 0:
                freeTree(root);
                root = NULL;
                printf("Exiting...\n");
                exit(0);
            default:
                printf("There is no such option\n");
        }
    }
    return 0;
}