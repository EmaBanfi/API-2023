#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define COUNT 5
#define N 20
#define DIM 32768
#define M 100

enum color{red, black};

typedef struct stationNode{
    int data;
    enum color color;
    struct stationNode* parent;
    struct stationNode* right;
    struct stationNode* left;
    int biggestCar;
    struct carNode* carsRoot;
    struct carNode* neighborsRoot;
}stationNode;

typedef struct carNode{
    int data;
    enum color color;
    struct carNode* parent;
    struct carNode* right;
    struct carNode* left;
}carNode;

typedef struct neighborNode{
    int data;
    struct neighborNode* generator;
    enum color color;
    struct  neighborNode* parent;
    struct neighborNode* right;
    struct neighborNode* left;
}neighborNode;

typedef struct borderNode{
    int data;
    struct borderNode* next;
}borderNode;

//stations-tree functions
stationNode* createNode(int data);
stationNode* createNilNode();
stationNode* RBInsert(stationNode** root, int data);
void RBInsertFixup(stationNode** root, stationNode* node);
void changeColor(stationNode* node);
void rightRotate(stationNode** root, stationNode* node);
void leftRotate(stationNode** root, stationNode* node);
void inOrderVisit(stationNode* root);
stationNode* searchNode(stationNode* root, int data);
int RBDelete(stationNode** root, int data);
stationNode* searchMinimum(stationNode* root);
stationNode* searchMaximum(stationNode* root);
stationNode* searchSuccessor(stationNode* node);
stationNode* searchPredecessor(stationNode* node);
void RBDeleteFixup(stationNode** root, stationNode* node);
void freeTree(stationNode* root);
void selectNeighbors(stationNode* root, int min, int max);

//cars-tree functions
carNode* createLightNode(int data);
carNode* createNilLightNode();
carNode* lightRBInsert(carNode* root, carNode* newNode);
carNode* lightRBInsertFixup(carNode* root, carNode* node);
void lightChangeColor(carNode* node);
carNode* lightRightRotate(carNode* root, carNode* node);
carNode* lightLeftRotate(carNode* root, carNode* node);
void lightInOrderVisit(carNode* root);
carNode* lightSearchNode(carNode* root, int data);
carNode* lightRBDelete(carNode* root, int data);
carNode* lightRBDeleteFixup(carNode* root, carNode* node);
carNode* lightSearchPredecessor(carNode* node);
carNode* lightSearchSuccessor(carNode* node);
carNode* lightSearchMinimum(carNode* root);
carNode* lightSearchMaximum(carNode* root);
void freeLightTree(carNode* root);

//neighbors-tree functions
neighborNode* createNeighborNode(int data, neighborNode* generator);
neighborNode* createNilNeighborNode();
neighborNode* neighborRBInsert(neighborNode** root, neighborNode* generator, int data);
void neighborRBInsertFixup(neighborNode** root, neighborNode* node);
void neighborChangeColor(neighborNode* node);
void neighborRightRotate(neighborNode** root, neighborNode* node);
void neighborLeftRotate(neighborNode** root, neighborNode* node);
void freeNeighborTree(neighborNode* root);
neighborNode* searchNeighborNode(neighborNode* root, int data);

//queue functions
borderNode* createBorderNode(int data);
void enqueue(borderNode** head, borderNode** tail, borderNode* node);
int dequeue(borderNode** head, borderNode** tail);

//Route functions
neighborNode* RightToLeftRoute(stationNode* stationsRoot, neighborNode** candidatesNodesRoot, neighborNode* generator, borderNode** head, borderNode** tail, int min, int max, int end, int currentBorder);
neighborNode* LeftToRightRoute(stationNode* stationsRoot, neighborNode** candidatesNodesRoot, neighborNode* generator, borderNode** head, borderNode** tail, int min, int max, int end, int currentBorder);
void routePlanner(stationNode* stationsRoot, int start, int end);

//Global variables
stationNode* Nil;
carNode* lightNil;
neighborNode* neighborNil;

int main(){
    int distance = 0, numOfCars = 0, autonomy = 0, start = 0, end = 0, max = 0, temp = 0;
    Nil = createNilNode();
    lightNil = createNilLightNode();
    neighborNil = createNilNeighborNode();
    stationNode* stationsRoot = Nil;
    stationNode* newNode = NULL;
    carNode* newCar = NULL;
    char* space;

    char line[DIM] = {};
    while(fgets(line, sizeof(line), stdin)) {
        *(strchr(line, '\n')) = '\0';

        //aggiungi-stazione command
        if(line[12] == 'z'){

            distance = atoi(line+sizeof(char)*18);
            space = strchr(line+sizeof(char)*18, ' ');
            numOfCars = atoi(space + sizeof(char)*1);
            newNode = RBInsert(&stationsRoot, distance);


            temp = 0;
            max = 0;
            if(newNode != NULL){
                for( int i = 0; i < numOfCars; i++){
                    space = strchr(space + sizeof(char)*1, ' ');
                    autonomy = atoi(space + sizeof(char)*1);
                    temp = autonomy;
                    if(temp > max)
                        max = temp;

                    newCar = createLightNode(autonomy);
                    newNode->carsRoot = lightRBInsert(newNode->carsRoot, newCar);
                }
                newNode->biggestCar = max;

                printf("aggiunta\n");
            }
            else{
                printf("non aggiunta\n");
            }

        }

            //demolisci-stazione command
        else if(line[12] == 'a'){

            distance = atoi(line + sizeof(char)*19);


            if(stationsRoot != Nil){
                int ornitorinco = RBDelete(&stationsRoot, distance);
                if(ornitorinco == 1){
                    printf("demolita\n");
                }
                else
                    printf("non demolita\n");
            }
            else
                printf("non demolita\n");
        }

            //aggiungi-auto command
        else if(line[12] == 'o'){

            distance = atoi(line+sizeof(char)*14);
            space = strchr(line+sizeof(char)*14, ' ');
            autonomy = atoi(space + sizeof(char)*1);


            newNode = searchNode(stationsRoot, distance);
            if(newNode != NULL){
                newNode->carsRoot = lightRBInsert(newNode->carsRoot, createLightNode(autonomy));

                if(newNode->biggestCar < autonomy){
                    newNode->biggestCar = autonomy;
                }

                printf("aggiunta\n");
            }else{
                printf("non aggiunta\n");
            }

        }

            //rottama-auto command
        else if(line[12] == ' '){

            distance = atoi(line+sizeof(char)*13);
            space = strchr(line+sizeof(char)*13, ' ');
            autonomy = atoi(space + sizeof(char)*1);

            newNode = searchNode(stationsRoot, distance);
            if(newNode != NULL && newNode->carsRoot != lightNil){
                newNode->carsRoot = lightRBDelete(newNode->carsRoot, autonomy);

                if(autonomy == newNode->biggestCar){
                    if(newNode->carsRoot->data == -1)
                        newNode->biggestCar = 0;
                    else
                        newNode->biggestCar = lightSearchMaximum(newNode->carsRoot)->data;
                }
            }
            else{
                printf("non rottamata\n");
            }
        }

            //pianifica-percorso command
        else{
            start = atoi(line+sizeof(char)*19);
            space = strchr(line+sizeof(char)*19, ' ');
            end = atoi(space + sizeof(char)*1);

            routePlanner(stationsRoot, start, end);
        }
    }
    freeTree(stationsRoot);
    free(Nil);
    free(lightNil);
    free(neighborNil);


    return 0;
}

//R&B Functions
//--------------------------------------
/**
 * Creates a new red stationNode, with parent, right and left child initialised to null.
 * @param data : integer saved into the stationNode
 * @return pointer to the new stationNode
 */
stationNode* createNode(int data){

    stationNode* newNode = (stationNode*)malloc(sizeof(struct stationNode));
    newNode->data = data;
    newNode->color = red;
    newNode->parent = NULL;
    newNode->right = NULL;
    newNode->left = NULL;
    newNode->carsRoot = lightNil;
    newNode->neighborsRoot = lightNil;
    newNode->biggestCar = 0;

    return newNode;
}

/**
 * Creates a Nil stationNode for a tree data structure, initialised with black color and all pointers to NULL.
 */
stationNode* createNilNode(){

    stationNode* newNode = (stationNode*)malloc(sizeof(struct stationNode));
    newNode->data = -1;
    newNode->color = black;
    newNode->parent = NULL;
    newNode->right = NULL;
    newNode->left = NULL;
    newNode->carsRoot = NULL;
    newNode->neighborsRoot = NULL;
    newNode->biggestCar = 0;

    return newNode;
}

/**
 * Adds a new stationNode to the specified tree, if isn't already present.
 * @param root : root of the specified tree.
 * @param data : stationNode to add to the data structure.
 * @return pointer to the new inserted stationNode if isn't already present, else null.
 */
stationNode* RBInsert(stationNode** root, int data){
    stationNode* newNode = NULL;

    //if the tree is empty => create a new root
    if((*root) == Nil){
        newNode = createNode(data);
        newNode->parent = Nil;
        newNode->left = Nil;
        newNode->right = Nil;
        newNode->color = black;

        *root = newNode;
    }

        //search te correct position for the new stationNode
    else{
        stationNode* currentNode = *root;
        stationNode* lastValid = NULL;

        while(currentNode->data != -1 && currentNode->data != data){
            lastValid = currentNode;
            if(data < lastValid->data){
                currentNode = currentNode->left;
            }else{
                currentNode = currentNode->right;
            }
        }

        //add the new station if isn't already present
        if(currentNode->data != data){
            newNode = createNode(data);
            newNode->parent = lastValid;
            newNode->right = Nil;
            newNode->left = Nil;

            if(newNode->data < lastValid->data){
                lastValid->left = newNode;
            }else{
                lastValid->right = newNode;
            }

            RBInsertFixup(root, newNode);
        }
    }
    return newNode;
}

/**
 * Fix the red and black tree after insertion of a red stationNode.
 * @param root : root of the specified tree.
 * @param node : stationNode just inserted to fix.
 */
void RBInsertFixup(stationNode** root, stationNode* node){
    struct stationNode* father = node->parent;
    struct stationNode* grandfather = father->parent;

    //case 0 : stationNode is the root
    if(*root == node){
        node->color = black;
    }

        //case 1 : father is black
    else if(father->color == black){
        return;
    }

        //parent is grandfather's left child
    else if(father == grandfather->left){
        struct stationNode* uncle = grandfather->right;

        //case 2 : uncle is red
        if(uncle->color == red){
            changeColor(father);
            changeColor(grandfather);
            changeColor(uncle);
            RBInsertFixup(root, grandfather);
        }

            //case 3 : uncle is black, stationNode is right child
        else if(uncle->color == black && node == father->right){
            leftRotate(root, father);
            RBInsertFixup(root, father);
        }

            //case 4 : uncle is black, stationNode is left child
        else{
            changeColor(father);
            changeColor(grandfather);
            rightRotate(root, grandfather);
        }
    }

        //parent is grandfather's right child
    else{
        struct stationNode* uncle = grandfather->left;

        //case 2 : uncle is red
        if(uncle->color == red){
            changeColor(father);
            changeColor(grandfather);
            changeColor(uncle);
            RBInsertFixup(root, grandfather);
        }

            //case 3 : uncle is black, stationNode is left child
        else if(uncle->color == black && node == father->left){
            rightRotate(root, father);
            RBInsertFixup(root, father);
        }

            //case 4 : uncle is black, stationNode is right child
        else{
            changeColor(father);
            changeColor(grandfather);
            leftRotate(root, grandfather);
        }
    }
}

/**
 * Changes the color of the specified stationNode to the opposite color.
 * @param node : stationNode to modify
 */
void changeColor(stationNode* node){
    if(node->color == red)
        node->color = black;
    else
        node->color = red;
}

/**
 * Performs a right rotation to the specified stationNode.
 * @param root : root of stationNode's tree
 * @param node : stationNode to right-rotate
 */
void rightRotate(stationNode** root, stationNode* node){
    struct stationNode* y = node;
    struct stationNode* x = node->left;
    struct stationNode* b = x->right;
    struct stationNode* d = node->parent;

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

    //if the rotated stationNode was the old root, then a new root is settled
    if(y == *root)
        *root = x;
}

/**
 * Performs a left rotation to the specified stationNode.
 * @param root : root of stationNode's tree
 * @param node : stationNode to left-rotate
 */
void leftRotate(stationNode** root, stationNode* node){
    struct stationNode* x = node;
    struct stationNode* y = node->right;
    struct stationNode* d = node->parent;
    struct stationNode* b = y->left;

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


    //if the rotated stationNode was the old root, then a new root is settled
    if(x == *root)
        *root = y;
}

/**
 * In-order visit of the specified tree
 * @param root : root of the specified tree to visit
 */
void inOrderVisit(stationNode* root){
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
 * @return pointer to the stationNode containing the data; if isn't found then returns null
 */
stationNode* searchNode(stationNode* root, int data){
    if (data == root->data)
        return root;
    else if(root->data == -1)
        return NULL;
    else if (data < root->data)
        return searchNode(root->left, data);
    else
        return searchNode(root->right, data);
}

/**
 * Deletes a data in the stations-tree if present, deleting also his cars-tree and deleting him from the graph
 * @param root : root of the specified tree
 * @param data : data to delete
 * @return 1 if data was found and deleted, 0 else
 */
int RBDelete(stationNode** root, int data){
    struct stationNode* wanted = searchNode(*root, data);
    carNode* carsToDelete = NULL;

    if(wanted != NULL){

        //find which stationNode is actually going to be deleted
        struct stationNode* toDelete;
        if(wanted->left->data == -1 || wanted->right->data == -1)
            toDelete = wanted;
        else
            toDelete = searchSuccessor(wanted);

        //find the subtree of the stationNode to delete and adjust the pointers
        struct stationNode* subtree;
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
            *root = subtree;
        else if(toDelete == toDelete->parent->left)
            toDelete->parent->left = subtree;
        else
            toDelete->parent->right = subtree;

        //if stationNode to delete isn't the wanted stationNode => copy the second stationNode's data, cars, biggestCar and neighbors to the first one to keep
        if(toDelete != wanted){
            wanted->data = toDelete->data;
            carsToDelete = wanted->carsRoot;
            wanted->carsRoot = toDelete->carsRoot;
            wanted->biggestCar = toDelete->biggestCar;
        }else{
            carsToDelete = toDelete->carsRoot;
        }

        //if the deleted stationNode is black then must be invoked the fixup function on the stationNode that replaced the deleted one
        if(toDelete->color == black)
            RBDeleteFixup(root, subtree);

        if(carsToDelete != NULL){
            freeLightTree(carsToDelete);
        }

        free(toDelete);
        return 1;
    }
    else{
        return 0;
    }
}

/**
 * Fix red and black tree after deletion of a black stationNode
 * @param root : root of the specified tree
 * @param node : stationNode that replaced the one that has been deleted
 */
void RBDeleteFixup(stationNode** root, stationNode* node){
    struct stationNode* father = node->parent;

    //if stationNode reached the root of the tree => recolor it black
    if(node == *root)
        node->color = black;

    else{
        //case if stationNode is the left child
        if(node == father->left){
            struct stationNode* brother = father->right;
            /*
            if(brother == Nil){
                print2D(*root);
            }
             */

            //case 0 : stationNode is red
            if(node->color == red) {
                node->color = black;
            }

                //case 1 : stationNode is black, brother is red
            else if(node->color == black && brother->color == red){
                changeColor(brother);
                changeColor(father);
                leftRotate(root, father);
                RBDeleteFixup(root, node);
            }

                //case 2 : stationNode is black, brother is black, nephews are black
            else if(node->color == black && brother->color == black && brother->left->color == black && brother->right->color == black){
                changeColor(brother);
                RBDeleteFixup(root, father);
            }

                //case 3 : stationNode is black, brother is black, right-nephew is black &&  left-nephew is red
            else if(node->color == black && brother->color == black && brother->right->color == black &&  brother->left->color == red){
                changeColor(brother);
                changeColor(brother->left);
                rightRotate(root, brother);
                RBDeleteFixup(root, node);
            }

                //case 4 : stationNode is black, brother is black, right-nephew is red
            else{
                brother->color = father->color;
                father->color = black;
                brother->right->color = black;
                leftRotate(root, father);
            }
        }
            //case if stationNode is the right child
        else{
            struct stationNode* brother = father->left;

            //case 0 : stationNode is red
            if(node->color == red) {
                node->color = black;
            }

                //case 1 : stationNode is black, brother is red
            else if(node->color == black && brother->color == red){
                changeColor(brother);
                changeColor(father);
                rightRotate(root, father);
                RBDeleteFixup(root, node);
            }

                //case 2 : stationNode is black, brother is black, nephews are black
            else if(node->color == black && brother->color == black && brother->left->color ==black && brother->right->color == black){
                changeColor(brother);
                RBDeleteFixup(root, father);
            }

                //case 3 : stationNode is black, brother is black, left-nephew is black &&  right-nephew is red
            else if(node->color == black && brother->color == black && brother->left->color == black &&  brother->right->color == red){
                changeColor(brother);
                changeColor(brother->right);
                leftRotate(root, brother);
                RBDeleteFixup(root, node);
            }

                //case 4 : stationNode is black, brother is black, left-nephew is red
            else{
                brother->color = father->color;
                father->color = black;
                brother->left->color = black;
                rightRotate(root, father);
            }
        }
    }

    Nil->parent = NULL;
}

/**
 * Returns the successor of a certain stationNode
 * @param node : stationNode whose successor is desired
 * @return pointer to successor stationNode, returns NULL if has no successor
 */
stationNode* searchSuccessor(stationNode* node){

    //if has a right subtree => the minimum is the minimum valor of that subtree
    if(node->right->data != -1){
        return searchMinimum(node->right);
    }

    //else the successor is the first parent stationNode that has as left child the subtree in which stationNode is inserted
    struct stationNode* father = node->parent;
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
 * Returns the predecessor of a certain stationNode
 * @param node : stationNode whose predecessor is desired
 * @return pointer to predecessor stationNode, returns NULL if has no predecessor
 */
stationNode* searchPredecessor(stationNode* node){

    //if has a right subtree => the maximum is the maximum valor of that subtree
    if(node->left->data != -1){
        return searchMaximum(node->left);
    }

    //else the successor is the first parent stationNode that has as right child the subtree in which stationNode is inserted
    struct stationNode* father = node->parent;
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
 * @return pointer to the stationNode with minimum integer of the tree
 */
stationNode* searchMinimum(stationNode* root){
    struct stationNode* current = root;

    while(current->left->data != -1){
        current = current->left;
    }
    return current;
}

/**
 * Search the maximum valor of a specified tree
 * @param root : root of the specified tree
 * @return pointer to the stationNode with maximum integer of the tree
 */
stationNode* searchMaximum(stationNode* root){
    struct stationNode* current = root;

    while(current->right->data != -1){
        current = current->right;
    }
    return current;
}

/**
 * Free the memory reserved for the specified tree
 * @param root : root of the tree to free
 */
void freeTree(stationNode* root){
    if(root == Nil)
        return;
    freeTree(root->left);
    freeTree(root->right);
    freeLightTree(root->carsRoot);
    free(root);
}

//Light R&B functions
//---------------------------------------------
/**
 * Creates a new red light-stationNode, with parent, right and left child initialised to null.
 * @param data : integer saved into the light-stationNode
 * @return pointer to the new light-stationNode
 */
carNode * createLightNode(int data){

    carNode * newNode = (carNode*)malloc(sizeof(struct carNode));
    newNode->data = data;
    newNode->color = red;
    newNode->parent = NULL;
    newNode->right = NULL;
    newNode->left = NULL;

    return newNode;
}

/**
 * Creates a Nil light-stationNode for a light-tree data structure, initialised with black color and all pointers to NULL.
 */
carNode* createNilLightNode(){

    carNode* newNode = (carNode*)malloc(sizeof(struct carNode));
    newNode->data = -1;
    newNode->color = black;
    newNode->parent = NULL;
    newNode->right = NULL;
    newNode->left = NULL;

    return newNode;
}

/**
 * Adds a new light-stationNode to the specified tree.
 * @param root : root of the specified tree.
 * @param newNode : light-stationNode to add to the data structure.
 * @return pointer to the new root in case rotations modify the old root
 */
carNode* lightRBInsert(carNode* root, carNode* newNode){
    carNode* newRoot;

    //if the tree is empty => create a new root
    if(root == lightNil){
        root = newNode;
        root->parent = lightNil;
        root->left = lightNil;
        root->right = lightNil;
        root->color = black;

        newRoot = root;
    }

        //search te correct position for the new stationNode
    else{
        carNode* currentNode = root;
        carNode* lastValid = NULL;

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
 * Fix the red and black tree after insertion of a red light-stationNode.
 * @param root : root of the specified tree.
 * @param node : light-stationNode just inserted to fix.
 * @return pointer to the new root in case rotations modify the old root
 */
carNode* lightRBInsertFixup(carNode* root, carNode* node){
    carNode* newRoot = root;
    carNode* father = node->parent;
    carNode* grandfather = father->parent;

    //case 0 : stationNode is the root
    if(root == node){
        node->color = black;
    }

        //case 1 : father is black
    else if(father->color == black){
        return newRoot;
    }

        //parent is grandfather's left child
    else if(father == grandfather->left){
        carNode* uncle = grandfather->right;

        //case 2 : uncle is red
        if(uncle->color == red){
            lightChangeColor(father);
            lightChangeColor(grandfather);
            lightChangeColor(uncle);
            newRoot = lightRBInsertFixup(root, grandfather);
        }

            //case 3 : uncle is black, stationNode is right child
        else if(uncle->color == black && node == father->right){
            newRoot = lightLeftRotate(root, father);
            newRoot = lightRBInsertFixup(newRoot, father);
        }

            //case 4 : uncle is black, stationNode is left child
        else{
            lightChangeColor(father);
            lightChangeColor(grandfather);
            newRoot = lightRightRotate(root, grandfather);
        }
    }

        //parent is grandfather's right child
    else{
        carNode* uncle = grandfather->left;

        //case 2 : uncle is red
        if(uncle->color == red){
            lightChangeColor(father);
            lightChangeColor(grandfather);
            lightChangeColor(uncle);
            newRoot = lightRBInsertFixup(root, grandfather);
        }

            //case 3 : uncle is black, stationNode is left child
        else if(uncle->color == black && node == father->left){
            newRoot = lightRightRotate(root, father);
            newRoot = lightRBInsertFixup(newRoot, father);
        }

            //case 4 : uncle is black, stationNode is right child
        else{
            lightChangeColor(father);
            lightChangeColor(grandfather);
            newRoot = lightLeftRotate(root, grandfather);
        }
    }

    return newRoot;
}

/**
 * Changes the color of the specified light-stationNode to the opposite color.
 * @param node : light-stationNode to modify
 */
void lightChangeColor(carNode* node){
    if(node->color == red)
        node->color = black;
    else
        node->color = red;
}

/**
 * Performs a right rotation to the specified light-stationNode.
 * @param root : root of light-stationNode's tree
 * @param node : light-stationNode to right-rotate
 * @return the new root if the old root is rotated, else the old root
 */
carNode* lightRightRotate(carNode* root, carNode* node){
    carNode* y = node;
    carNode* x = node->left;
    carNode* b = x->right;
    carNode* d = node->parent;
    carNode* newRoot = root;

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

    //if the rotated stationNode was the old root, then a new root is settled
    if(y == root)
        newRoot = x;

    return newRoot;
}

/**
 * Performs a left rotation to the specified light-stationNode.
 * @param root : root of light-stationNode's tree
 * @param node : light-stationNode to left-rotate
 * @return the new root if the old root is rotated, else the old root
 */
carNode* lightLeftRotate(carNode* root, carNode* node){
    carNode* x = node;
    carNode* y = node->right;
    carNode* d = node->parent;
    carNode* b = y->left;
    carNode* newRoot = root;

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


    //if the rotated stationNode was the old root, then a new root is settled
    if(x == root)
        newRoot = y;

    return newRoot;
}

/**
 * In-order visit of the specified light-tree
 * @param root : root of the specified light-tree to visit
 */
void lightInOrderVisit(carNode* root){
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
 * @return pointer to the light-stationNode containing the data; if isn't found then returns null
 */
carNode* lightSearchNode(carNode* root, int data){

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
carNode* lightRBDelete(carNode* root, int data){
    carNode* newRoot = root;
    carNode* wanted = lightSearchNode(root, data);

    if(wanted != NULL){

        //find which stationNode is actually going to be deleted
        carNode* toDelete;
        if(wanted->left->data == -1 || wanted->right->data == -1)
            toDelete = wanted;
        else
            toDelete = lightSearchSuccessor(wanted);

        //find the subtree of the stationNode to delete and adjust the pointers
        carNode* subtree;
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

        //if the deleted stationNode is black then must be invoked the fixup function on the stationNode that replaced the deleted one
        if(toDelete->color == black)
            newRoot = lightRBDeleteFixup(newRoot, subtree);

        free(toDelete);
        printf("rottamata\n");
    }
    else{
        printf("non rottamata\n");
    }

    return newRoot;
}

/**
 * Fix red and black light-tree after deletion of a black light-stationNode
 * @param root : root of the specified light-tree
 * @param node : light-stationNode that replaced the one that has been deleted
 * @return : pointer to the new root in case rotations modify the old root
 */
carNode* lightRBDeleteFixup(carNode* root, carNode* node){
    carNode* father = node->parent;
    carNode* newRoot = root;

    //if stationNode reached the root of the tree => recolor it black
    if(node == root)
        node->color = black;

    else{
        //case if stationNode is the left child
        if(node == father->left){
            carNode* brother = father->right;

            //case 0 : stationNode is red
            if(node->color == red) {
                node->color = black;
            }

                //case 1 : stationNode is black, brother is red
            else if(node->color == black && brother->color == red){
                lightChangeColor(brother);
                lightChangeColor(father);
                newRoot = lightLeftRotate(root, father);
                newRoot = lightRBDeleteFixup(newRoot, node);
            }

                //case 2 : stationNode is black, brother is black, nephews are black
            else if(node->color == black && brother->color == black && brother->left->color == black && brother->right->color == black){
                lightChangeColor(brother);
                newRoot = lightRBDeleteFixup(root, father);
            }

                //case 3 : stationNode is black, brother is black, right-nephew is black &&  left-nephew is red
            else if(node->color == black && brother->color == black && brother->right->color == black &&  brother->left->color == red){
                lightChangeColor(brother);
                lightChangeColor(brother->left);
                newRoot = lightRightRotate(root, brother);
                newRoot = lightRBDeleteFixup(newRoot, node);
            }

                //case 4 : stationNode is black, brother is black, right-nephew is red
            else{
                brother->color = father->color;
                father->color = black;
                brother->right->color = black;
                newRoot = lightLeftRotate(root, father);
            }
        }
            //case if stationNode is the right child
        else{
            carNode* brother = father->left;

            //case 0 : stationNode is red
            if(node->color == red) {
                node->color = black;
            }

                //case 1 : stationNode is black, brother is red
            else if(node->color == black && brother->color == red){
                lightChangeColor(brother);
                lightChangeColor(father);
                newRoot = lightRightRotate(root, father);
                newRoot = lightRBDeleteFixup(newRoot, node);
            }

                //case 2 : stationNode is black, brother is black, nephews are black
            else if(node->color == black && brother->color == black && brother->left->color == black && brother->right->color == black){
                lightChangeColor(brother);
                newRoot = lightRBDeleteFixup(root, father);
            }

                //case 3 : stationNode is black, brother is black, left-nephew is black &&  right-nephew is red
            else if(node->color == black && brother->color == black && brother->left->color == black &&  brother->right->color == red){
                lightChangeColor(brother);
                lightChangeColor(brother->right);
                newRoot = lightLeftRotate(root, brother);
                newRoot = lightRBDeleteFixup(newRoot, node);
            }

                //case 4 : stationNode is black, brother is black, left-nephew is red
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
 * Returns the successor of a certain light-stationNode
 * @param node : light-stationNode whose successor is desired
 * @return pointer to successor light-stationNode, returns NULL if has no successor
 */
carNode* lightSearchSuccessor(carNode* node){

    //if has a right subtree => the minimum is the minimum valor of that subtree
    if(node->right->data != -1){
        return lightSearchMinimum(node->right);
    }

    //else the successor is the first parent stationNode that has as left child the subtree in which stationNode is inserted
    carNode* father = node->parent;
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
 * Returns the predecessor of a certain light-stationNode
 * @param node : light-stationNode whose predecessor is desired
 * @return pointer to predecessor light-stationNode, returns NULL if has no predecessor
 */
carNode* lightSearchPredecessor(carNode* node){

    //if has a right subtree => the maximum is the maximum valor of that subtree
    if(node->left->data != -1){
        return lightSearchMaximum(node->left);
    }

    //else the successor is the first parent stationNode that has as right child the subtree in which stationNode is inserted
    carNode* father = node->parent;
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
 * @return pointer to the light-stationNode with minimum integer of the light-tree
 */
carNode* lightSearchMinimum(carNode* root){
    carNode* current = root;

    while(current->left->data != -1){
        current = current->left;
    }
    return current;
}

/**
 * Search the maximum valor of a specified light-tree
 * @param root : root of the specified light-tree
 * @return pointer to the light-stationNode with maximum integer of the light-tree
 */
carNode* lightSearchMaximum(carNode* root){
    carNode* current = root;

    while(current->right->data != -1){
        current = current->right;
    }
    return current;
}

/**
 * Free the memory reserved for the specified light-tree
 * @param root : root of the light-tree to free
 */
void freeLightTree(carNode* root){
    if(root == lightNil)
        return;
    freeLightTree(root->left);
    freeLightTree(root->right);
    free(root);
}

//Neighbors-tree functions
//---------------------------------------------

/**
 * Creates a new red neighborNode, with parent, right and left child initialised to null.
 * @param data : integer saved into the neighborNode
 * @param generator : node that generated the new edge
 * @return pointer to the new neighborNode
 */
neighborNode* createNeighborNode(int data, neighborNode* generator){

    neighborNode * newNode = (neighborNode*)malloc(sizeof(struct neighborNode));
    newNode->data = data;
    newNode->color = red;
    newNode->generator = generator;
    newNode->parent = NULL;
    newNode->right = NULL;
    newNode->left = NULL;

    return newNode;
}

/**
 * Creates a Nil neighbor node for a neighbors-tree, initialised with black color and all pointers to NULL.
 */
neighborNode* createNilNeighborNode(){

    neighborNode* newNode = (neighborNode*)malloc(sizeof(struct neighborNode));
    newNode->data = -1;
    newNode->color = black;
    newNode->generator = NULL;
    newNode->parent = NULL;
    newNode->right = NULL;
    newNode->left = NULL;

    return newNode;
}

/**
 * Adds a new neighborNode to the specified tree.
 * @param root : root of the specified tree.
 * @param newNode : neighborNode to add to the data structure.
 * @return pointer to the new inserted node
 */
neighborNode* neighborRBInsert(neighborNode** root, neighborNode* generator, int data){
    neighborNode *newNode = NULL;

    //if the tree is empty => create a new root
    if((*root) == neighborNil){
        newNode = createNeighborNode(data, generator);
        newNode->parent = neighborNil;
        newNode->left = neighborNil;
        newNode->right = neighborNil;
        newNode->color = black;

        *root = newNode;
    }

        //search te correct position for the new stationNode
    else{
        neighborNode* currentNode = *root;
        neighborNode* lastValid = NULL;

        while( currentNode->data != -1){
            lastValid = currentNode;
            if(data < lastValid->data){
                currentNode = currentNode->left;
            }else{
                currentNode = currentNode->right;
            }
        }
        newNode = createNeighborNode(data, generator);
        newNode->parent = lastValid;
        newNode->right = neighborNil;
        newNode->left = neighborNil;

        if(newNode->data < lastValid->data){
            lastValid->left = newNode;
        }else{
            lastValid->right = newNode;
        }

        neighborRBInsertFixup(root, newNode);
    }
    return newNode;
}

/**
 * Fix the red and black tree after insertion of a red neighborNode.
 * @param root : root of the specified tree.
 * @param node : neighborNode just inserted to fix.
 */
void neighborRBInsertFixup(neighborNode** root, neighborNode* node){
    neighborNode* father = node->parent;
    neighborNode* grandfather = father->parent;

    //case 0 : stationNode is the root
    if(*root == node){
        node->color = black;
    }

        //case 1 : father is black
    else if(father->color == black){
        return;
    }

        //parent is grandfather's left child
    else if(father == grandfather->left){
        neighborNode* uncle = grandfather->right;

        //case 2 : uncle is red
        if(uncle->color == red){
            neighborChangeColor(father);
            neighborChangeColor(grandfather);
            neighborChangeColor(uncle);
            neighborRBInsertFixup(root, grandfather);
        }

            //case 3 : uncle is black, stationNode is right child
        else if(uncle->color == black && node == father->right){
            neighborLeftRotate(root, father);
            neighborRBInsertFixup(root, father);
        }

            //case 4 : uncle is black, stationNode is left child
        else{
            neighborChangeColor(father);
            neighborChangeColor(grandfather);
            neighborRightRotate(root, grandfather);
        }
    }

        //parent is grandfather's right child
    else{
        neighborNode* uncle = grandfather->left;

        //case 2 : uncle is red
        if(uncle->color == red){
            neighborChangeColor(father);
            neighborChangeColor(grandfather);
            neighborChangeColor(uncle);
            neighborRBInsertFixup(root, grandfather);
        }

            //case 3 : uncle is black, stationNode is left child
        else if(uncle->color == black && node == father->left){
            neighborRightRotate(root, father);
            neighborRBInsertFixup(root, father);
        }

            //case 4 : uncle is black, stationNode is right child
        else{
            neighborChangeColor(father);
            neighborChangeColor(grandfather);
            neighborLeftRotate(root, grandfather);
        }
    }
}

/**
 * Changes the color of the specified neighborNode to the opposite color.
 * @param node : neighborNode to modify
 */
void neighborChangeColor(neighborNode* node){
    if(node->color == red)
        node->color = black;
    else
        node->color = red;
}

/**
 * Performs a right rotation to the specified neighborNode.
 * @param root : root of neighborNode's tree
 * @param node : neighborNode to right-rotate
 */
void neighborRightRotate(neighborNode** root, neighborNode* node){
    neighborNode* y = node;
    neighborNode* x = node->left;
    neighborNode* b = x->right;
    neighborNode* d = node->parent;

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

    //if the rotated stationNode was the old root, then a new root is settled
    if(y == *root)
        *root = x;
}

/**
 * Performs a left rotation to the specified neighborNode.
 * @param root : root of neighborNode's tree
 * @param node : neighborNode to left-rotate
 */
void neighborLeftRotate(neighborNode** root, neighborNode* node){
    neighborNode* x = node;
    neighborNode* y = node->right;
    neighborNode* d = node->parent;
    neighborNode* b = y->left;

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


    //if the rotated stationNode was the old root, then a new root is settled
    if(x == *root)
        *root = y;
}

/**
 * Perfmorms a search of an element in the specified neighbor-tree
 * @param root : root of the specified tree
 * @param data : data to search in the tree
 * @return pointer to the neighbor node containing the data; if isn't found then returns null
 */
neighborNode* searchNeighborNode(neighborNode* root, int data){
    if (data == root->data)
        return root;
    else if( root->data == -1)
        return NULL;
    else if (data < root->data)
        return searchNeighborNode(root->left, data);
    else
        return searchNeighborNode(root->right, data);
}

void freeNeighborTree(neighborNode* root){
    if(root == neighborNil)
        return;
    freeNeighborTree(root->left);
    freeNeighborTree(root->right);
    free(root);
}

//queue functions
//---------------------------------------------

/**
 * Creates a new node for the border-queue, with next pointer initialised to null
 * @param data : integer to insert into the node
 * @return pointer to the new node
 */
borderNode* createBorderNode(int data){

    borderNode* newNode = (borderNode*)malloc(sizeof (struct borderNode));
    newNode->data = data;
    newNode->next = NULL;

    return newNode;
}

/**
 * Enqueue a borderNode in borders-queue
 * @param head : double pointer to the head of the queue
 * @param tail : double pointer to the tail of the queue
 * @param node : pointer to the new node to enqueue
 */
void enqueue(borderNode** head, borderNode** tail, borderNode* node){
    if(*head == NULL){
        *head = node;
        *tail = node;
    }else{
        (*tail)->next = node;
        *tail = node;
    }
}
/**
 * Dequeue a borderNode in borders-queue, whiel also freeing memory from the queue
 * @param head : head of the queue
 * @param tail : tail of the queue
 * @return integer contained in the old head of the queue; if is empty returns -1
 */
int dequeue(borderNode** head, borderNode** tail){
    borderNode* platypus = NULL;
    int result = -1;

    if(*head != NULL){
        platypus = *head;
        *head = (*head)->next;
        result = platypus->data;
        free(platypus);
    }

    return result;
}

//Stack functions
//---------------------------------------------

/**
 * Adds a new element on top of the stack with LIFO logic
 * @param head : head of the stack
 * @param data : data to insert
 */
void addToStack(borderNode** head, int data){
    borderNode* newNode = createBorderNode(data);

    if(*head == NULL){
        *head = newNode;
    }else{
        newNode->next = *head;
        *head = newNode;
    }
}

/**
 * Reads the saved route while also freeing the memory from the stack
 * @param head : head of the stack
 */
void readRoute(borderNode** head){
    borderNode* temp = NULL;

    while(*head != NULL) {
        printf("%d", (*head)->data);
        temp = *head;
        *head = (*head)->next;
        free(temp);
        if(*head != NULL)
            printf(" ");
    }
    printf("\n");
}



//Route functions
//---------------------------------------------

/* 1) inserisci start nella pila della frontiera e nell'albero dei visitati
 * 2) dequeue dalla frontiera il nodo e trova i vicini del nodo tenendo conto di goal e senza tornare indietro, poi inseriscili nei visitati con il puntatore al nodo generatore usando la visita in order
 * 3) itera fintanto che non trovi goal nei visitati o finisce frontiera
 * 4) se hai trovato goal stampa la strada dei generatori al contrario
 * 5) libera la memoria da tutto lo schifo usato
 */

/**
 * Calculate the best route from start to end stations
 * @param start : starting station
 * @param end : ending station
 */
void routePlanner(stationNode* stationsRoot, int start, int end){
    int currentBorder = -1, min = 0, max = 0;
    borderNode* head = NULL;
    borderNode* tail = NULL;
    borderNode* stackHead = NULL;
    neighborNode* candidatesNodesRoot = neighborNil;
    neighborNode* generator = NULL;
    neighborNode* lastStation = NULL;
    stationNode* vertix = NULL;

    //se vai da sx a dx => calcola il percorso andando a cercare a partire dall'inizio i vicini raggiungibili per ogni nodo tramite una visita in order delle stazioni
    if(start < end){
        generator = neighborRBInsert(&candidatesNodesRoot, NULL, start);
        currentBorder = start;
    }
        //se vai da dx a sx => calcola il percorso partendo dalla fine ed andando a cercare i nodi da cui è raggiunto tramite una visita in order delle stazioni
    else{
        generator = neighborRBInsert(&candidatesNodesRoot, NULL, end);
        currentBorder = end;
    }

    if(start == end)
        printf("%d\n", start);

    else{
        //itera per ogni nodo presente nella coda della frontiera, cioè per ogni vertice vai a calcolare i suoi archi
        while(currentBorder != -1 && lastStation == NULL){
            vertix = searchNode(stationsRoot, currentBorder);

            //calcola min e max cercando la stazione e prendendo la sua auto più grande
            if( start < end){
                min = currentBorder;
                max = currentBorder + vertix->biggestCar;
                if(max > end)
                    max = end;

                //calcola i candidati per il percorso minimo
                lastStation = RightToLeftRoute(stationsRoot, &candidatesNodesRoot, generator, &head, &tail, min, max, end, currentBorder);
            }else{
                min = currentBorder;
                max = start;

                lastStation = LeftToRightRoute(stationsRoot, &candidatesNodesRoot, generator, &head, &tail, min, max, start, currentBorder);
            }

            currentBorder = dequeue(&head, &tail);
            generator = searchNeighborNode(candidatesNodesRoot, currentBorder);
        }

        //se hai trovato il percorso minimo => stampalo
        if(lastStation != NULL && start < end){
            while(lastStation != NULL){
                addToStack(&stackHead, lastStation->data);
                lastStation = lastStation->generator;
            }
            readRoute(&stackHead);
        }
        else if(lastStation != NULL && start > end){
            while(lastStation != NULL){
                printf("%d", lastStation->data);
                lastStation = lastStation->generator;
                if(lastStation!=NULL)
                    printf(" ");
            }
            printf("\n");
        }
        else{
            printf("nessun percorso\n");
        }

        //libera la memoria dallo stack, coda e albero dei vicini
        freeNeighborTree(candidatesNodesRoot);
        while(currentBorder != -1)
            currentBorder = dequeue(&head, &tail);
    }
}

/**
 * Search the candidates nodes for the best route
 * @param stationsRoot : root of the stations tree
 * @param candidatesNodesRoot : root of the candidates tree
 * @param generator : previous node in the route
 * @param head : head of the border queue
 * @param tail : tail of the border queue
 * @param min : minimum distance used to search candidates
 * @param max : maximum distance used to search candidates
 * @param end : arrival station
 * @param currentBorder : current border whose candidates are searched
 * @return : pointer to the arrival station if is found in the cadidates; if isn't found and all possible candidates of currentBorder have been analised, returns null
 */
neighborNode* RightToLeftRoute(stationNode* stationsRoot, neighborNode** candidatesNodesRoot, neighborNode* generator, borderNode** head, borderNode** tail, int min, int max, int end, int currentBorder){
    neighborNode *result = NULL;

    if(stationsRoot == Nil)
        return NULL;

    if(stationsRoot->data > min){
        result = RightToLeftRoute(stationsRoot->left, candidatesNodesRoot, generator, head, tail, min, max, end,
                                  currentBorder);
        if(result != NULL && result->data == end)
            return result;
    }

    if(stationsRoot->data >= min && stationsRoot->data <= max && searchNeighborNode( *candidatesNodesRoot, stationsRoot->data)==NULL){
        result = neighborRBInsert(candidatesNodesRoot, generator, stationsRoot->data);
        if(result->data == end)
            return result;
        enqueue(head, tail, createBorderNode(stationsRoot->data));
    }

    if(stationsRoot->data < max){
        result = RightToLeftRoute(stationsRoot->right, candidatesNodesRoot, generator, head, tail, min, max, end,
                                  currentBorder);
    }

    //ritorna solo se effettivamente hai trovato end
    if(result != NULL && result->data == end)
        return result;
    else
        return NULL;
}

neighborNode* LeftToRightRoute(stationNode* stationsRoot, neighborNode** candidatesNodesRoot, neighborNode* generator, borderNode** head, borderNode** tail, int min, int max, int end, int currentBorder){
    neighborNode* result = NULL;

    if(stationsRoot == Nil)
        return NULL;

    if(stationsRoot->data > min)
        result = LeftToRightRoute(stationsRoot->left, candidatesNodesRoot, generator, head, tail, min, max, end, currentBorder);

    if(stationsRoot->data >= min &&
       stationsRoot->data <= max &&
       (stationsRoot->data - stationsRoot->biggestCar) <= currentBorder &&
       searchNeighborNode( *candidatesNodesRoot, stationsRoot->data)==NULL)
    {
        result = neighborRBInsert(candidatesNodesRoot, generator, stationsRoot->data);
        if(result->data == end)
            return result;
        enqueue(head, tail, createBorderNode(stationsRoot->data));
    }

    if(stationsRoot->data < max){
        result = LeftToRightRoute(stationsRoot->right, candidatesNodesRoot, generator, head, tail, min, max, end, currentBorder);
    }

    //ritorna solo se effettivamente hai trovato end
    if(result != NULL && result->data == end)
        return result;
    else
        return NULL;
}