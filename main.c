#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define COUNT 5
#define N 20
#define MAXLENGTH 32768

enum color{red, black};

typedef struct node{
    int data;
    enum color color;
    struct node* parent;
    struct node* right;
    struct node* left;
    struct lightNode* carsRoot;
    struct lightNode* neighborsRoot;
}node;

typedef struct lightNode{
    int data;
    enum color color;
    struct lightNode* parent;
    struct lightNode* right;
    struct lightNode* left;
}lightNode;

//RB functions
node* createNode(int data);
node* createNilNode();
node* RBInsert(node* root, node* newNode);
node* RBInsertFixup(node* root, node* node);
void changeColor(node* node);
node* rightRotate(node* root, node* node);
node* leftRotate(node* root, node* node);
void inOrderVisit(node* root);
node* searchNode(node* root, int data);
node* RBDelete(node* root, int data);
node* searchMinimum(node* root);
node* searchMaximum(node* root);
node* searchSuccessor(node* node);
node* searchPredecessor(node* node);
node* RBDeleteFixup(node* root, node* node);

//Light RB functions
lightNode* createLightNode(int data);
lightNode* createNilLightNode();
lightNode* lightRBInsert(lightNode* root, lightNode* newNode);
lightNode* lightRBInsertFixup(lightNode* root, lightNode* node);
void lightChangeColor(lightNode* node);
lightNode* lightRightRotate(lightNode* root, lightNode* node);
lightNode* lightLeftRotate(lightNode* root, lightNode* node);
void lightInOrderVisit(lightNode* root);
lightNode* lightSearchNode(lightNode* root, int data);
lightNode* lightRBDelete(lightNode* root, int data);
lightNode* lightRBDeleteFixup(lightNode* root, lightNode* node);
lightNode* lightSearchPredecessor(lightNode* node);
lightNode* lightSearchSuccessor(lightNode* node);
lightNode* lightSearchMinimum(lightNode* root);
lightNode* lightSearchMaximum(lightNode* root);
void freeLightTree(lightNode* root);


//imported functions
void print2D(node* root);
void print2DUtil(node* root, int space);

//Global variables
node* Nil;
lightNode* lightNil;

int main(){
    int perry = 0, distance = 0, numOfCars = 0, autonomy = 0, start = 0, end = 0;
    Nil = createNilNode();
    lightNil = createNilLightNode();
    node* stationsRoot = NULL;
    node* newNode = NULL;
    lightNode* newCar = NULL;
    char line[MAXLENGTH] = {};
    char * param;

    while(fgets(line, sizeof(line), stdin)){

        //aggiungi-stazione command
        if(line[12] == 'z'){
            param = strtok(line + sizeof(char)*18, " ");
            distance = atoi(param);
            param = strtok(NULL, " ");
            numOfCars = atoi(param);
            //printf("command = %c\ndistance = %d\nnumOfCars = %d\n", line[12], distance, numOfCars);

            newNode = createNode(distance);
            stationsRoot = RBInsert(stationsRoot, newNode);

            for( int i = 0; i < numOfCars; i++){
                param = strtok(NULL, " ");
                autonomy = atoi(param);
                //printf("autonomy of car %d = %d\n", i, autonomy);

                newCar = createLightNode(autonomy);
                newNode->carsRoot = lightRBInsert(newNode->carsRoot, newCar);
            }
        }

            //demolisci-stazione command
        else if(line[12] == 'a'){
            param = strtok(line + sizeof(char)*19, " ");
            distance = atoi(param);
            //printf("command = %c\ndistance = %d\n", line[12], distance);
        }

            //aggiungi-auto command
        else if(line[12] == 'o'){
            param = strtok(line + sizeof(char)*14, " ");
            distance = atoi(param);
            param = strtok(NULL, " ");
            autonomy = atoi(param);
            printf("command = %c\ndistance = %d\nautonomy = %d\n", line[12], distance, autonomy);
        }

            //rottama-auto command
        else if(line[12] == '\0'){
            param = strtok(line + sizeof(char)*13, " ");
            distance = atoi(param);
            param = strtok(NULL, " ");
            autonomy = atoi(param);
            printf("command = %c\ndistance = %d\nautonomy = %d\n", line[12], distance, autonomy);
        }

            //pianifica-percorso command
        else{
            param = strtok(line + sizeof(char)*19, " ");
            start = atoi(param);
            param = strtok(NULL, " ");
            end = atoi(param);
            printf("command = %c\nstart = %d\nend = %d\n", line[12], start, end);
        }
    }

    return 0;
}

//R&B Functions
//--------------------------------------
/**
 * Creates a new red node, with parent, right and left child initialised to null.
 * @param data : integer saved into the node
 * @return pointer to the new node
 */
node* createNode(int data){

    node* newNode = (node*)malloc(sizeof(struct node));
    newNode->data = data;
    newNode->color = red;
    newNode->parent = NULL;
    newNode->right = NULL;
    newNode->left = NULL;
    newNode->carsRoot = NULL;
    newNode->neighborsRoot = NULL;

    return newNode;
}

/**
 * Creates a Nil node for a tree data structure, initialised with black color and all pointers to NULL.
 */
node* createNilNode(){

    node* newNode = (node*)malloc(sizeof(struct node));
    newNode->data = -1;
    newNode->color = black;
    newNode->parent = NULL;
    newNode->right = NULL;
    newNode->left = NULL;
    newNode->carsRoot = NULL;
    newNode->neighborsRoot = NULL;

    return newNode;
}

/**
 * Adds a new node to the specified tree.
 * @param root : root of the specified tree.
 * @param newNode : node to add to the data structure.
 * @return pointer to the new root in case rotations modify the old root
 */
node* RBInsert(node* root, node* newNode){
    node* newRoot;


    //if the tree is empty => create a new root
    if(root == NULL){
        root = newNode;
        root->parent = Nil;
        root->left = Nil;
        root->right = Nil;
        root->color = black;

        newRoot = root;
    }

        //search te correct position for the new node
    else{
        node* currentNode = root;
        node* lastValid;

        while(currentNode->data != -1){
            lastValid = currentNode;
            if(newNode->data < lastValid->data){
                currentNode = currentNode->left;
            }else{
                currentNode = currentNode->right;
            }
        }

        newNode->parent = lastValid;
        newNode->right = Nil;
        newNode->left = Nil;

        if(newNode->data < lastValid->data){
            lastValid->left = newNode;
        }else{
            lastValid->right = newNode;
        }

        newRoot = RBInsertFixup(root, newNode);
    }
    return newRoot;
}

/**
 * Fix the red and black tree after insertion of a red node.
 * @param root : root of the specified tree.
 * @param node : node just inserted to fix.
 * @return pointer to the new root in case rotations modify the old root
 */
node* RBInsertFixup(node* root, node* node){
    struct node* newRoot = root;
    struct node* father = node->parent;
    struct node* grandfather = father->parent;

    //case 0 : node is the root
    if(root == node){
        node->color = black;
    }

        //case 1 : father is black
    else if(father->color == black){
        return newRoot;
    }

        //parent is grandfather's left child
    else if(father == grandfather->left){
        struct node* uncle = grandfather->right;

        //case 2 : uncle is red
        if(uncle->color == red){
            changeColor(father);
            changeColor(grandfather);
            changeColor(uncle);
            newRoot = RBInsertFixup(root, grandfather);
        }

            //case 3 : uncle is black, node is right child
        else if(uncle->color == black && node == father->right){
            newRoot = leftRotate(root, father);
            newRoot = RBInsertFixup(newRoot, father);
        }

            //case 4 : uncle is black, node is left child
        else{
            changeColor(father);
            changeColor(grandfather);
            newRoot = rightRotate(root, grandfather);
        }
    }

        //parent is grandfather's right child
    else{
        struct node* uncle = grandfather->left;

        //case 2 : uncle is red
        if(uncle->color == red){
            changeColor(father);
            changeColor(grandfather);
            changeColor(uncle);
            newRoot = RBInsertFixup(root, grandfather);
        }

            //case 3 : uncle is black, node is left child
        else if(uncle->color == black && node == father->left){
            newRoot = rightRotate(root, father);
            newRoot = RBInsertFixup(newRoot, father);
        }

            //case 4 : uncle is black, node is right child
        else{
            changeColor(father);
            changeColor(grandfather);
            newRoot = leftRotate(root, grandfather);
        }
    }

    return newRoot;
}

/**
 * Changes the color of the specified node to the opposite color.
 * @param node : node to modify
 */
void changeColor(node* node){
    if(node->color == red)
        node->color = black;
    else
        node->color = red;
}

/**
 * Performs a right rotation to the specified node.
 * @param root : root of node's tree
 * @param node : node to right-rotate
 * @return the new root if the old root is rotated, else the old root
 */
node* rightRotate(node* root, node* node){
    struct node* y = node;
    struct node* x = node->left;
    struct node* b = x->right;
    struct node* d = node->parent;
    struct node* newRoot = root;

    //fix pointers of the nodes
    if(b->data != -1)
        b->parent = y;
    y->left = b;
    x->right = y;
    y->parent = x;
    x->parent = d;
    if(d->data != -1){
        if(y == d->left)
            d->left = x;
        else
            d->right = x;
    }

    //if the rotated node was the old root, then a new root is settled
    if(y == root)
        newRoot = x;

    return newRoot;
}

/**
 * Performs a left rotation to the specified node.
 * @param root : root of node's tree
 * @param node : node to left-rotate
 * @return the new root if the old root is rotated, else the old root
 */
node* leftRotate(node* root, node* node){
    struct node* x = node;
    struct node* y = node->right;
    struct node* d = node->parent;
    struct node* b = y->left;
    struct node* newRoot = root;

    //fix pointers of the nodes
    if(b->data != -1)
        b->parent = x;
    x->right = b;
    x->parent = y;
    y->left = x;
    y->parent = d;
    if(d->data != -1){
        if(x == d->left)
            d->left = y;
        else
            d->right = y;
    }


    //if the rotated node was the old root, then a new root is settled
    if(x == root)
        newRoot = y;

    return newRoot;
}

/**
 * In-order visit of the specified tree
 * @param root : root of the specified tree to visit
 */
void inOrderVisit(node* root){
    if(root->data != -1) {
        inOrderVisit(root->left);
        printf("%d ", root->data);
        inOrderVisit(root->right);
    }
}

/**
 * Perfmorms a search of an element in the specified tree
 * @param root : root of the specified tree
 * @param data : data to search in the tree
 * @return pointer to the node containing the data; if isn't found then returns null
 */
node* searchNode(node* root, int data){
    if (data == root->data)
        return root;
    else if( root->data == -1)
        return NULL;
    else if (data < root->data)
        return searchNode(root->left, data);
    else
        return searchNode(root->right, data);
}

/**
 * Deletes a data in the specified tree if present
 * @param root : root of the specified tree
 * @param data : data to delete
 * @return pointer to the new root in case rotations modify the old root
 */
node* RBDelete(node* root, int data){
    struct node* newRoot = root;
    struct node* wanted = searchNode(root, data);

    if(wanted != NULL){

        //find which node is actually going to be deleted
        struct node* toDelete;
        if(wanted->left->data == -1 || wanted->right->data == -1)
            toDelete = wanted;
        else
            toDelete = searchSuccessor(wanted);

        //find the subtree of the node to delete and adjust the pointers
        struct node* subtree;
        if(toDelete->left->data != -1)
            subtree = toDelete->left;
        else
            subtree = toDelete->right;

        /*
        if(subtree->data != -1){
            subtree->parent = toDelete->parent;
        }
         */

        subtree->parent = toDelete->parent;

        if(toDelete->parent->data == -1)
            newRoot = subtree;
        else if(toDelete == toDelete->parent->left)
            toDelete->parent->left = subtree;
        else
            toDelete->parent->right = subtree;

        if(toDelete != wanted)
            wanted->data = toDelete->data;

        //if the deleted node is black then must be invoked the fixup function on the node that replaced the deleted one
        if(toDelete->color == black)
            newRoot = RBDeleteFixup(newRoot, subtree);

        free(toDelete);
    }
    else{
        printf("%d not found\n", data);
    }

    return newRoot;
}

/**
 * Fix red and black tree after deletion of a black node
 * @param root : root of the specified tree
 * @param node : node that replaced the one that has been deleted
 * @return : pointer to the new root in case rotations modify the old root
 */
node* RBDeleteFixup(node* root, node* node){
    struct node* father = node->parent;
    struct node* newRoot = root;

    //if node reached the root of the tree => recolor it black
    if(node == root)
        node->color = black;

    else{
        //case if node is the left child
        if(node == father->left){
            struct node* brother = father->right;

            //case 0 : node is red
            if(node->color == red) {
                node->color = black;
            }

                //case 1 : node is black, brother is red
            else if(node->color == black && brother->color == red){
                changeColor(brother);
                changeColor(father);
                newRoot = leftRotate(root, father);
                newRoot = RBDeleteFixup(newRoot, node);
            }

                //case 2 : node is black, brother is black, nephews are black
            else if(node->color == black && brother->color == black && brother->left->color == black && brother->right->color == black){
                changeColor(brother);
                newRoot = RBDeleteFixup(root, father);
            }

                //case 3 : node is black, brother is black, right-nephew is black &&  left-nephew is red
            else if(node->color == black && brother->color == black && brother->right->color == black &&  brother->left->color == red){
                changeColor(brother);
                changeColor(brother->left);
                newRoot = rightRotate(root, brother);
                newRoot = RBDeleteFixup(newRoot, node);
            }

                //case 4 : node is black, brother is black, right-nephew is red
            else{
                brother->color = father->color;
                father->color = black;
                brother->right->color = black;
                newRoot = leftRotate(root, father);
            }
        }
            //case if node is the right child
        else{
            struct node* brother = father->left;

            //case 0 : node is red
            if(node->color == red) {
                node->color = black;
            }

                //case 1 : node is black, brother is red
            else if(node->color == black && brother->color == red){
                changeColor(brother);
                changeColor(father);
                newRoot = rightRotate(root, father);
                newRoot = RBDeleteFixup(newRoot, node);
            }

                //case 2 : node is black, brother is black, nephews are black
            else if(node->color == black && brother->color == black && brother->left->color == brother->right->color == black){
                changeColor(brother);
                newRoot = RBDeleteFixup(root, father);
            }

                //case 3 : node is black, brother is black, left-nephew is black &&  right-nephew is red
            else if(node->color == black && brother->color == black && brother->left->color == black &&  brother->right->color == red){
                changeColor(brother);
                changeColor(brother->right);
                newRoot = leftRotate(root, brother);
                newRoot = RBDeleteFixup(newRoot, node);
            }

                //case 4 : node is black, brother is black, left-nephew is red
            else{
                brother->color = father->color;
                father->color = black;
                brother->left->color = black;
                newRoot = rightRotate(root, father);
            }
        }
    }

    Nil->parent = NULL;

    return newRoot;
}

/**
 * Returns the successor of a certain node
 * @param node : node whose successor is desired
 * @return pointer to successor node, returns NULL if has no successor
 */
node* searchSuccessor(node* node){

    //if has a right subtree => the minimum is the minimum valor of that subtree
    if(node->right->data != -1){
        return searchMinimum(node->right);
    }

    //else the successor is the first parent node that has as left child the subtree in which node is inserted
    struct node* father = node->parent;
    while(father->data != -1 &&  father->right == node){
        node = father;
        father = father->parent;
    }

    if(father->data != -1)
        return father;
    else
        return NULL;
}

/**
 * Returns the predecessor of a certain node
 * @param node : node whose predecessor is desired
 * @return pointer to predecessor node, returns NULL if has no predecessor
 */
node* searchPredecessor(node* node){

    //if has a right subtree => the maximum is the maximum valor of that subtree
    if(node->left->data != -1){
        return searchMaximum(node->left);
    }

    //else the successor is the first parent node that has as right child the subtree in which node is inserted
    struct node* father = node->parent;
    while(father->data != -1 &&  father->left == node){
        node = father;
        father = father->parent;
    }

    if(father->data != -1)
        return father;
    else
        return NULL;
}

/**
 * Search the minimum valor of a specified tree
 * @param root : root of the specified tree
 * @return pointer to the node with minimum integer of the tree
 */
node* searchMinimum(node* root){
    struct node* current = root;

    while(current->left->data != -1){
        current = current->left;
    }
    return current;
}

/**
 * Search the maximum valor of a specified tree
 * @param root : root of the specified tree
 * @return pointer to the node with maximum integer of the tree
 */
node* searchMaximum(node* root){
    struct node* current = root;

    while(current->right->data != -1){
        current = current->right;
    }
    return current;
}

/**
 * Free the memory reserved for the specified tree
 * @param root : root of the tree to free
 */
void freeTree(node* root){
    if(root == Nil)
        return;
    freeTree(root->left);
    freeTree(root->right);
}

//Light R&B functions
//---------------------------------------------
/**
 * Creates a new red light-node, with parent, right and left child initialised to null.
 * @param data : integer saved into the light-node
 * @return pointer to the new light-node
 */
lightNode * createLightNode(int data){

    lightNode * newNode = (lightNode*)malloc(sizeof(struct lightNode));
    newNode->data = data;
    newNode->color = red;
    newNode->parent = NULL;
    newNode->right = NULL;
    newNode->left = NULL;

    return newNode;
}

/**
 * Creates a Nil light-node for a light-tree data structure, initialised with black color and all pointers to NULL.
 */
lightNode* createNilLightNode(){

    lightNode* newNode = (lightNode*)malloc(sizeof(struct lightNode));
    newNode->data = -1;
    newNode->color = black;
    newNode->parent = NULL;
    newNode->right = NULL;
    newNode->left = NULL;

    return newNode;
}

/**
 * Adds a new light-node to the specified tree.
 * @param root : root of the specified tree.
 * @param newNode : light-node to add to the data structure.
 * @return pointer to the new root in case rotations modify the old root
 */
lightNode* lightRBInsert(lightNode* root, lightNode* newNode){
    lightNode* newRoot;

    //if the tree is empty => create a new root
    if(root == NULL){
        root = newNode;
        root->parent = lightNil;
        root->left = lightNil;
        root->right = lightNil;
        root->color = black;

        newRoot = root;
    }

        //search te correct position for the new node
    else{
        lightNode* currentNode = root;
        lightNode* lastValid;

        while( currentNode->data != -1){
            lastValid = currentNode;
            if(newNode->data < lastValid->data){
                currentNode = currentNode->left;
            }else{
                currentNode = currentNode->right;
            }
        }

        newNode->parent = lastValid;
        newNode->right = lightNil;
        newNode->left = lightNil;

        if(newNode->data < lastValid->data){
            lastValid->left = newNode;
        }else{
            lastValid->right = newNode;
        }

        newRoot = lightRBInsertFixup(root, newNode);
    }
    return newRoot;
}

/**
 * Fix the red and black tree after insertion of a red light-node.
 * @param root : root of the specified tree.
 * @param node : light-node just inserted to fix.
 * @return pointer to the new root in case rotations modify the old root
 */
lightNode* lightRBInsertFixup(lightNode* root, lightNode* node){
    lightNode* newRoot = root;
    lightNode* father = node->parent;
    lightNode* grandfather = father->parent;

    //case 0 : node is the root
    if(root == node){
        node->color = black;
    }

        //case 1 : father is black
    else if(father->color == black){
        return newRoot;
    }

        //parent is grandfather's left child
    else if(father == grandfather->left){
        lightNode* uncle = grandfather->right;

        //case 2 : uncle is red
        if(uncle->color == red){
            lightChangeColor(father);
            lightChangeColor(grandfather);
            lightChangeColor(uncle);
            newRoot = lightRBInsertFixup(root, grandfather);
        }

            //case 3 : uncle is black, node is right child
        else if(uncle->color == black && node == father->right){
            newRoot = lightLeftRotate(root, father);
            newRoot = lightRBInsertFixup(newRoot, father);
        }

            //case 4 : uncle is black, node is left child
        else{
            lightChangeColor(father);
            lightChangeColor(grandfather);
            newRoot = lightRightRotate(root, grandfather);
        }
    }

        //parent is grandfather's right child
    else{
        lightNode* uncle = grandfather->left;

        //case 2 : uncle is red
        if(uncle->color == red){
            lightChangeColor(father);
            lightChangeColor(grandfather);
            lightChangeColor(uncle);
            newRoot = lightRBInsertFixup(root, grandfather);
        }

            //case 3 : uncle is black, node is left child
        else if(uncle->color == black && node == father->left){
            newRoot = lightRightRotate(root, father);
            newRoot = lightRBInsertFixup(newRoot, father);
        }

            //case 4 : uncle is black, node is right child
        else{
            lightChangeColor(father);
            lightChangeColor(grandfather);
            newRoot = lightLeftRotate(root, grandfather);
        }
    }

    return newRoot;
}

/**
 * Changes the color of the specified light-node to the opposite color.
 * @param node : light-node to modify
 */
void lightChangeColor(lightNode* node){
    if(node->color == red)
        node->color = black;
    else
        node->color = red;
}

/**
 * Performs a right rotation to the specified light-node.
 * @param root : root of light-node's tree
 * @param node : light-node to right-rotate
 * @return the new root if the old root is rotated, else the old root
 */
lightNode* lightRightRotate(lightNode* root, lightNode* node){
    lightNode* y = node;
    lightNode* x = node->left;
    lightNode* b = x->right;
    lightNode* d = node->parent;
    lightNode* newRoot = root;

    //fix pointers of the nodes
    if(b->data != -1)
        b->parent = y;
    y->left = b;
    x->right = y;
    y->parent = x;
    x->parent = d;
    if(d->data != -1){
        if(y == d->left)
            d->left = x;
        else
            d->right = x;
    }

    //if the rotated node was the old root, then a new root is settled
    if(y == root)
        newRoot = x;

    return newRoot;
}

/**
 * Performs a left rotation to the specified light-node.
 * @param root : root of light-node's tree
 * @param node : light-node to left-rotate
 * @return the new root if the old root is rotated, else the old root
 */
lightNode* lightLeftRotate(lightNode* root, lightNode* node){
    lightNode* x = node;
    lightNode* y = node->right;
    lightNode* d = node->parent;
    lightNode* b = y->left;
    lightNode* newRoot = root;

    //fix pointers of the nodes
    if(b->data != -1)
        b->parent = x;
    x->right = b;
    x->parent = y;
    y->left = x;
    y->parent = d;
    if(d->data != -1){
        if(x == d->left)
            d->left = y;
        else
            d->right = y;
    }


    //if the rotated node was the old root, then a new root is settled
    if(x == root)
        newRoot = y;

    return newRoot;
}

/**
 * In-order visit of the specified light-tree
 * @param root : root of the specified light-tree to visit
 */
void lightInOrderVisit(lightNode* root){
    if(root->data != -1) {
        lightInOrderVisit(root->left);
        printf("%d ", root->data);
        lightInOrderVisit(root->right);
    }
}

/**
 * Perfmorms a search of an element in the specified light-tree
 * @param root : root of the specified light-tree
 * @param data : data to search in the light-tree
 * @return pointer to the light-node containing the data; if isn't found then returns null
 */
lightNode* lightSearchNode(lightNode* root, int data){
    if (data == root->data)
        return root;
    else if( root->data == -1)
        return NULL;
    else if (data < root->data)
        return lightSearchNode(root->left, data);
    else
        return lightSearchNode(root->right, data);
}

/**
 * Deletes a data in the specified light-tree if present
 * @param root : root of the specified light-tree
 * @param data : data to delete
 * @return pointer to the new root in case rotations modify the old root
 */
lightNode* lightRBDelete(lightNode* root, int data){
    lightNode* newRoot = root;
    lightNode* wanted = lightSearchNode(root, data);

    if(wanted != NULL){

        //find which node is actually going to be deleted
        lightNode* toDelete;
        if(wanted->left->data == -1 || wanted->right->data == -1)
            toDelete = wanted;
        else
            toDelete = lightSearchSuccessor(wanted);

        //find the subtree of the node to delete and adjust the pointers
        lightNode* subtree;
        if(toDelete->left->data != -1)
            subtree = toDelete->left;
        else
            subtree = toDelete->right;

        /*
        if(subtree->data != -1){
            subtree->parent = toDelete->parent;
        }
         */

        subtree->parent = toDelete->parent;

        if(toDelete->parent->data == -1)
            newRoot = subtree;
        else if(toDelete == toDelete->parent->left)
            toDelete->parent->left = subtree;
        else
            toDelete->parent->right = subtree;

        if(toDelete != wanted)
            wanted->data = toDelete->data;

        //if the deleted node is black then must be invoked the fixup function on the node that replaced the deleted one
        if(toDelete->color == black)
            newRoot = lightRBDeleteFixup(newRoot, subtree);

        free(toDelete);
    }
    else{
        printf("%d not found\n", data);
    }

    return newRoot;
}

/**
 * Fix red and black light-tree after deletion of a black light-node
 * @param root : root of the specified light-tree
 * @param node : light-node that replaced the one that has been deleted
 * @return : pointer to the new root in case rotations modify the old root
 */
lightNode* lightRBDeleteFixup(lightNode* root, lightNode* node){
    lightNode* father = node->parent;
    lightNode* newRoot = root;

    //if node reached the root of the tree => recolor it black
    if(node == root)
        node->color = black;

    else{
        //case if node is the left child
        if(node == father->left){
            lightNode* brother = father->right;

            //case 0 : node is red
            if(node->color == red) {
                node->color = black;
            }

                //case 1 : node is black, brother is red
            else if(node->color == black && brother->color == red){
                lightChangeColor(brother);
                lightChangeColor(father);
                newRoot = lightLeftRotate(root, father);
                newRoot = lightRBDeleteFixup(newRoot, node);
            }

                //case 2 : node is black, brother is black, nephews are black
            else if(node->color == black && brother->color == black && brother->left->color == black && brother->right->color == black){
                lightChangeColor(brother);
                newRoot = lightRBDeleteFixup(root, father);
            }

                //case 3 : node is black, brother is black, right-nephew is black &&  left-nephew is red
            else if(node->color == black && brother->color == black && brother->right->color == black &&  brother->left->color == red){
                lightChangeColor(brother);
                lightChangeColor(brother->left);
                newRoot = lightRightRotate(root, brother);
                newRoot = lightRBDeleteFixup(newRoot, node);
            }

                //case 4 : node is black, brother is black, right-nephew is red
            else{
                brother->color = father->color;
                father->color = black;
                brother->right->color = black;
                newRoot = lightLeftRotate(root, father);
            }
        }
            //case if node is the right child
        else{
            lightNode* brother = father->left;

            //case 0 : node is red
            if(node->color == red) {
                node->color = black;
            }

                //case 1 : node is black, brother is red
            else if(node->color == black && brother->color == red){
                lightChangeColor(brother);
                lightChangeColor(father);
                newRoot = lightRightRotate(root, father);
                newRoot = lightRBDeleteFixup(newRoot, node);
            }

                //case 2 : node is black, brother is black, nephews are black
            else if(node->color == black && brother->color == black && brother->left->color == brother->right->color == black){
                lightChangeColor(brother);
                newRoot = lightRBDeleteFixup(root, father);
            }

                //case 3 : node is black, brother is black, left-nephew is black &&  right-nephew is red
            else if(node->color == black && brother->color == black && brother->left->color == black &&  brother->right->color == red){
                lightChangeColor(brother);
                lightChangeColor(brother->right);
                newRoot = lightLeftRotate(root, brother);
                newRoot = lightRBDeleteFixup(newRoot, node);
            }

                //case 4 : node is black, brother is black, left-nephew is red
            else{
                brother->color = father->color;
                father->color = black;
                brother->left->color = black;
                newRoot = lightRightRotate(root, father);
            }
        }
    }

    Nil->parent = NULL;

    return newRoot;
}

/**
 * Returns the successor of a certain light-node
 * @param node : light-node whose successor is desired
 * @return pointer to successor light-node, returns NULL if has no successor
 */
lightNode* lightSearchSuccessor(lightNode* node){

    //if has a right subtree => the minimum is the minimum valor of that subtree
    if(node->right->data != -1){
        return lightSearchMinimum(node->right);
    }

    //else the successor is the first parent node that has as left child the subtree in which node is inserted
    lightNode* father = node->parent;
    while(father->data != -1 &&  father->right == node){
        node = father;
        father = father->parent;
    }

    if(father->data != -1)
        return father;
    else
        return NULL;
}

/**
 * Returns the predecessor of a certain light-node
 * @param node : light-node whose predecessor is desired
 * @return pointer to predecessor light-node, returns NULL if has no predecessor
 */
lightNode* lightSearchPredecessor(lightNode* node){

    //if has a right subtree => the maximum is the maximum valor of that subtree
    if(node->left->data != -1){
        return lightSearchMaximum(node->left);
    }

    //else the successor is the first parent node that has as right child the subtree in which node is inserted
    lightNode* father = node->parent;
    while(father->data != -1 &&  father->left == node){
        node = father;
        father = father->parent;
    }

    if(father->data != -1)
        return father;
    else
        return NULL;
}

/**
 * Search the minimum valor of a specified light-tree
 * @param root : root of the specified light-tree
 * @return pointer to the light-node with minimum integer of the light-tree
 */
lightNode* lightSearchMinimum(lightNode* root){
    lightNode* current = root;

    while(current->left->data != -1){
        current = current->left;
    }
    return current;
}

/**
 * Search the maximum valor of a specified light-tree
 * @param root : root of the specified light-tree
 * @return pointer to the light-node with maximum integer of the light-tree
 */
lightNode* lightSearchMaximum(lightNode* root){
    lightNode* current = root;

    while(current->right->data != -1){
        current = current->right;
    }
    return current;
}

/**
 * Free the memory reserved for the specified light-tree
 * @param root : root of the light-tree to free
 */
void freeLightTree(lightNode* root){
    if(root == lightNil)
        return;
    freeLightTree(root->left);
    freeLightTree(root->right);
}

//Imported functions
//---------------------------------------------

// Function to print binary tree in 2D
// It does reverse inorder traversal
void print2DUtil(node* root, int space)
{
    // Base case
    if (root->data == -1)
        return;

    // Increase distance between levels
    space += COUNT;

    // Process right child first
    print2DUtil(root->right, space);

    // Print current node after space
    // count
    printf("\n");
    for (int i = COUNT; i < space; i++)
        printf(" ");
    printf("%d|%u\n", root->data, root->color);

    // Process left child
    print2DUtil(root->left, space);
}

// Wrapper over print2DUtil()
void print2D(node* root)
{
    // Pass initial space count as 0
    print2DUtil(root, 0);
}