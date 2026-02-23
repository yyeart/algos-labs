#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node *left;
    struct Node *right;
} Node;

Node* createNode(int value){
    Node *n = (Node*)malloc(sizeof(Node));
    if(!n) return NULL;
    n->data = value;
    n->left = n->right = NULL;
    return n; 
}

Node* insert(Node* root, int value){
    if(!root) return createNode(value);
    if(value < root->data){
        root->left = insert(root->left, value);
    }
    else if(value > root->data){
        root->right = insert(root->right, value);
    }
    return root;
}

void printTree(Node* root, int depth){
    if(!root) return;
    for(int i = 0; i < depth; ++i) printf(" ");
    printf("%d\n", root->data);
    printTree(root->left, depth + 1);
    printTree(root->right, depth + 1);
}

Node* deleteNode(Node* root, int value){
    if(!root) return NULL;
    if(value < root->data){
        root->left = deleteNode(root->left, value);
    }
    else if(value > root->data){
        root->right = deleteNode(root->right, value);
    }
    else {
        if(!root->left){
            Node* tmp = root->right;
            free(root);
            return tmp;
        }
        else if(!root->right){
            Node *tmp = root->left;
            free(root);
            return tmp;
        }
        Node *tmp = root->right;
        while(tmp && tmp->left != NULL) tmp = tmp->left;
        root->data = tmp->data;
        root->right = deleteNode(root->right, tmp->data);
    }
    return root;
}

int checkLevels(Node* root, int currentLevel, int* leafLevel){
    if(!root) return 1;
    if(!root->left && !root->right) {
        if(*leafLevel == -1){
            *leafLevel = currentLevel;
            return 1;
        }
        return (currentLevel == *leafLevel);
    }
    return checkLevels(root->left, currentLevel + 1, leafLevel) &&
        checkLevels(root->right, currentLevel + 1, leafLevel);
}

void freeTree(Node* root){
    if(!root) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

int main(void){
    Node* root = NULL;
    int option, val;
    while(1){
        printf("1 - Insert\n");
        printf("2 - Print\n");
        printf("3 - Delete\n");
        printf("4 - Function\n");
        printf("0 - Exit\n");
        scanf("%d", &option);
        switch(option){
            case 1:
                printf("Enter value: ");
                scanf("%d", &val);
                root = insert(root, val);
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