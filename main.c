#include <stdio.h>
#include <stdlib.h>

enum color{red, black};

typedef struct node{
    int data;
    enum color color;
    struct node* parent;
    struct node* right;
    struct node* left;
}node;

//RB functions
node* createNode(int data);
node* createNilNode();
void RBInsert(node* root, int data);
void RBInsertFixup(node* root, node* node);

//Global variables
node* Nil;

int main(){

    Nil = createNilNode();

    return 0;
}

/* Creates a new red node, with parent, right and left child initialised to null.
 * data : data to insert into the node.
 * return : pointer to the new created node.
 */
node* createNode(int data){

    node* newNode = (node*)malloc(sizeof(struct node));
    newNode->data = data;
    newNode->color = red;
    newNode->parent = NULL;
    newNode->right = NULL;
    newNode->left = NULL;

    return newNode;
}

/* Creates a Nil node for a tree data structure, initialised with black color and all pointers to NULL.
 */
node* createNilNode(){

    node* newNode = (node*)malloc(sizeof(struct node));
    newNode->data = -1;
    newNode->color = black;
    newNode->parent = NULL;
    newNode->right = NULL;
    newNode->left = NULL;

    return newNode;
}

/* Adds a new data to the specified tree.
 * root : root of the specified tree.
 * data : integer to add to the data structure.
 */
void RBInsert(node* root, int data){

    //if the tree is empty => create a new root
    if(root == NULL){
        root = createNode(data);
        root->parent = Nil;
        root->left = Nil;
        root->right = Nil;
        root->color = black;
    }

    //search te correct position for the new node
    else{
        node* currentNode = root;
        node* lastValid;

        while( currentNode->data != -1){
            lastValid = currentNode;
            if(data < lastValid->data){
                currentNode = currentNode->left;
            }else{
                currentNode = currentNode->right;
            }
        }

        node* newNode = createNode(data);
        newNode->parent = lastValid;
        newNode->right = Nil;
        newNode->left = Nil;

        if(data < lastValid->data){
            lastValid->left = newNode;
        }else{
            lastValid->right = newNode;
        }

        RBInsertFixup(root, newNode);

    }

    return;
}

/* Fix the red and black tree after insertion of a red node.
 * root : root of the specified tree.
 * node : node just inserted to fix.
 */
void RBInsertFixup(node* root, node* node){

}

//ciao





















