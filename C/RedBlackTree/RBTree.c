/**
 * @file RBTree.c
 * @author  Omer Salman <omer.salman@mail.huji.ac.il>
 * @version 1.0
 * @date 11 Dec 2019
 *
 * @brief functions of a generic red-black tree data structure.
 *
 *
 * @section DESCRIPTION
 * functions of a generic red-black tree data structure.
 */

// ------------------------------ includes ------------------------------
#include <stdio.h>
#include "RBTree.h"
#include <stdlib.h>


// -------------------------- const definitions -------------------------
/**
 * @def FAILURE_INDICATOR 0
 * @brief A macro that sets an int failure indicator
 */
#define FAILURE_INDICATOR 0

/**
 * @def SUCCESS_INDICATOR 1
 * @brief A macro that sets an int success indicator
 */
#define SUCCESS_INDICATOR 1

/**
 * @enum Direction
 * @brief An enum for direction of node's child - to indicate if a node is left child or right child.
 */
typedef enum Direction
{
    Right,
    Left,
    NoDirection
} Direction;

// ------------------------------ functions -----------------------------
void fixTree(Node *justEnteredNode, RBTree *tree);

void goingThroughTreeNodes(Node *node, forEachFunc func, void *args);

int firstRotation(Node *justEnteredNode, Node *enteredNodeParent, Node *enteredNodeGrandfather);

void secondRotation(Node *justEnteredNode, Node *enteredNodeParent, Node *enteredNodeGrandfather, RBTree *tree);

int noRotationsAndNullNodesCheck(Node *justEnteredNode);

void freeTreeNode(Node *node, FreeFunc freeFunc);


/**
 * constructs a new RBTree with the given CompareFunc.
 * comp: a function two compare two variables.
 */
RBTree *newRBTree(CompareFunc compFunc, FreeFunc freeFunc)
{
    RBTree *rbTreePtr = (RBTree *) malloc(1 * sizeof(RBTree));
    if (rbTreePtr == NULL)
    {
        return NULL;
    }
    rbTreePtr->root = NULL;
    rbTreePtr->compFunc = compFunc;
    rbTreePtr->freeFunc = freeFunc;
    rbTreePtr->size = 0;
    return rbTreePtr;
}

/**
 * add an item to the tree
 * @param tree: the tree to add an item to.
 * @param data: item to add to the tree.
 * @return: 0 on failure, other on success. (if the item is already in the tree - failure).
 */
int addToRBTree(RBTree *tree, void *data)
{
    if (tree == NULL)
    {
        return FAILURE_INDICATOR;
    }
    Node *currentNode = tree->root;
    Node *prevNode = tree->root;
    Direction direction = NoDirection;
    while (currentNode != NULL)
    {
        int dataComparisonResult = (tree->compFunc)(data, currentNode->data);
        prevNode = currentNode;
        if (dataComparisonResult > 0)
        {
            currentNode = currentNode->right;
            direction = Right;
        }
        else if (dataComparisonResult < 0)
        {
            currentNode = currentNode->left;
            direction = Left;
        }
        else
        {
            return FAILURE_INDICATOR;
        }
    }
    Node *newNodeToEnter = (Node *) malloc(1 * sizeof(Node));
    if (newNodeToEnter == NULL)
    {
        return FAILURE_INDICATOR;
    }
    newNodeToEnter->right = NULL;
    newNodeToEnter->left = NULL;
    newNodeToEnter->data = data;
    newNodeToEnter->color = RED;
    newNodeToEnter->parent = prevNode;
    if (prevNode == NULL)
    { // tree root is NULL - tree is empty
        tree->root = newNodeToEnter;
        newNodeToEnter->color = BLACK;
        (tree->size)++;
        return SUCCESS_INDICATOR;
    }
    else if (direction == Right)
    {
        prevNode->right = newNodeToEnter;
    }
    else if (direction == Left)
    {
        prevNode->left = newNodeToEnter;
    }
    (tree->size)++;
    fixTree(newNodeToEnter, tree);
    return SUCCESS_INDICATOR;
}

/**
 * check whether the tree contains this item.
 * @param tree: the tree to add an item to.
 * @param data: item to check.
 * @return: 0 if the item is not in the tree, other if it is.
 */
int containsRBTree(RBTree *tree, void *data)
{
    Node *currentNode = tree->root;
    while (currentNode != NULL)
    {
        int dataComparisonResult = (tree->compFunc)(data, currentNode->data);
        if (dataComparisonResult > 0)
        {
            currentNode = currentNode->right;
        }
        else if (dataComparisonResult < 0)
        {
            currentNode = currentNode->left;
        }
        else
        {
            return 1; // data exists in tree
        }
    }
    return 0; // data doesnt exist in tree
}

/**
 * Activate a function on each item of the tree. the order is an ascending order. if one of the activations of the
 * function returns 0, the process stops.
 * @param tree: the tree with all the items.
 * @param func: the function to activate on all items.
 * @param args: more optional arguments to the function (may be null if the given function support it).
 * @return: 0 on failure, other on success.
 */
int forEachRBTree(RBTree *tree, forEachFunc func, void *args)
{
    if (tree == NULL)
    {
        return FAILURE_INDICATOR;
    }
    else if (func == NULL)
    {
        return FAILURE_INDICATOR;
    }
    else
    {
        goingThroughTreeNodes(tree->root, func, args);
        return SUCCESS_INDICATOR;
    }
}

/**
 * a recursive function to execute a given function on all tree's nodes.
 * @param node node to execute function on and go to his child recursively
 * @param func function to execute on every node's data
 * @param args optional parameter for the function execute
 */
void goingThroughTreeNodes(Node *node, forEachFunc func, void *args)
{ // going through tree nodes in increasing order according to their data
    if (node == NULL)
    {
        return;
    }
    goingThroughTreeNodes(node->left, func, args);
    func(node->data, args);
    goingThroughTreeNodes(node->right, func, args);
}

/**
 * function to fix the tree starting a given node so it will become a legal red black tree
 * @param justEnteredNode node to start fixing the tree from
 * @param tree tree to fix
 */
void fixTree(Node *justEnteredNode, RBTree *tree)
{
    if (noRotationsAndNullNodesCheck(justEnteredNode) == FAILURE_INDICATOR)
    {
        return;
    }
    Node *enteredNodeParent = justEnteredNode->parent;
    Node *enteredNodeGrandfather = enteredNodeParent->parent;
    Node *enteredNodeUncle = enteredNodeGrandfather->left;
    if (enteredNodeUncle == enteredNodeParent)
    {
        enteredNodeUncle = enteredNodeGrandfather->right;
    }
    if (enteredNodeParent->color == RED)
    {
        if (enteredNodeUncle == NULL || enteredNodeUncle->color == BLACK)
        {
            int firstRotationIndicator = firstRotation(justEnteredNode, enteredNodeParent, enteredNodeGrandfather);
            if (firstRotationIndicator == SUCCESS_INDICATOR)
            {
                secondRotation(enteredNodeParent, justEnteredNode, enteredNodeGrandfather, tree);
            }
            else
            {
                secondRotation(justEnteredNode, enteredNodeParent, enteredNodeGrandfather, tree);
            }
        }
        else if (enteredNodeUncle->color == RED)
        {
            enteredNodeParent->color = BLACK;
            enteredNodeUncle->color = BLACK;
            enteredNodeGrandfather->color = RED;
            fixTree(enteredNodeGrandfather, tree);
        }
    }
}

/**
 * free all memory of the data structure.
 * @param tree: the tree to free.
 */
void freeRBTree(RBTree *tree)
{
    freeTreeNode(tree->root, tree->freeFunc);
    free(tree);
}

/**
 * recursive function to free all tree node's and their data
 * @param node node to free and continue freeing it's children and so on recursively
 * @param freeFunc suited free function for a node's data
 */
void freeTreeNode(Node *node, FreeFunc freeFunc)
{
    if (node == NULL)
    {
        return;
    }
    freeTreeNode(node->left, freeFunc);
    freeTreeNode(node->right, freeFunc);
    freeFunc(node->data);
    free(node);
}

/**
 * function to execute first rotation on given nodes if needed in order to keep the tree a legal red-black tree
 * @param justEnteredNode node that just added to the tree so a rotation is needed in order to keep
 * the tree a legal red black tree.
 * @param enteredNodeParent justEnteredNode's parent
 * @param enteredNodeGrandfather justEnteredNode's parent's parent
 * @return 1 in case the rotation executed successfully, 0 otherwise.
 */
int firstRotation(Node *justEnteredNode, Node *enteredNodeParent, Node *enteredNodeGrandfather)
{
    if (enteredNodeGrandfather->left != NULL && enteredNodeGrandfather->left->right != NULL &&
        justEnteredNode == enteredNodeGrandfather->left->right)
    {
        enteredNodeGrandfather->left = justEnteredNode;
        enteredNodeParent->right = justEnteredNode->left;
        if (justEnteredNode->left != NULL)
        {
            justEnteredNode->left->parent = enteredNodeParent;
        }
        justEnteredNode->left = enteredNodeParent;

    }
    else if (enteredNodeGrandfather->right != NULL && enteredNodeGrandfather->right->left != NULL &&
             justEnteredNode == enteredNodeGrandfather->right->left)
    {
        enteredNodeGrandfather->right = justEnteredNode;
        enteredNodeParent->left = justEnteredNode->right;
        if (justEnteredNode->right != NULL)
        {
            justEnteredNode->right->parent = enteredNodeParent;
        }
        justEnteredNode->right = enteredNodeParent;
    }
    else
    {
        return FAILURE_INDICATOR;
    }
    justEnteredNode->parent = enteredNodeGrandfather;
    enteredNodeParent->parent = justEnteredNode;
    return SUCCESS_INDICATOR;
}

/**
 * function to execute second rotation on given nodes if needed in order to keep the tree a legal red-black tree
 * @param justEnteredNode node that just added to the tree so a rotation is needed in order to keep
 * the tree a legal red black tree.
 * @param enteredNodeParent justEnteredNode's parent
 * @param enteredNodeGrandfather justEnteredNode's parent's parent
 * @param tree tree to execute rotation on
 */
void secondRotation(Node *justEnteredNode, Node *enteredNodeParent, Node *enteredNodeGrandfather, RBTree *tree)
{
    Node *parentSuitableChild = NULL;
    if (enteredNodeGrandfather->left != NULL && enteredNodeGrandfather->left->left != NULL &&
        justEnteredNode == enteredNodeGrandfather->left->left)
    {
        parentSuitableChild = enteredNodeParent->right;
        enteredNodeParent->right = enteredNodeGrandfather;
        enteredNodeGrandfather->left = parentSuitableChild;
    }
    else if (enteredNodeGrandfather->right != NULL && enteredNodeGrandfather->right->right != NULL &&
             justEnteredNode == enteredNodeGrandfather->right->right)
    {
        parentSuitableChild = enteredNodeParent->left;
        enteredNodeParent->left = enteredNodeGrandfather;
        enteredNodeGrandfather->right = parentSuitableChild;
    }
    else
    {
        return;
    }
    enteredNodeParent->parent = enteredNodeGrandfather->parent;
    if (enteredNodeGrandfather->parent != NULL)
    {
        if (enteredNodeGrandfather->parent->left == enteredNodeGrandfather)
        {
            enteredNodeGrandfather->parent->left = enteredNodeParent;
        }
        else if (enteredNodeGrandfather->parent->right == enteredNodeGrandfather)
        {
            enteredNodeGrandfather->parent->right = enteredNodeParent;
        }
    }
    else
    {
        tree->root = enteredNodeParent;
    }
    enteredNodeGrandfather->parent = enteredNodeParent;
    if (parentSuitableChild != NULL)
    {
        parentSuitableChild->parent = enteredNodeGrandfather;
    }
    enteredNodeParent->color = BLACK;
    enteredNodeGrandfather->color = RED;
}

/**
 * function checks if a rotation is needed on a given tree in order to keep it a legal red-black tree
 * @param justEnteredNode node that just added to the tree so a rotation is might be needed in order to keep
 * the tree a legal red black tree.
 * @return 1 if a rotation is need, 0 otherwise.
 */
int noRotationsAndNullNodesCheck(Node *justEnteredNode)
{
    if (justEnteredNode == NULL)
    {
        return FAILURE_INDICATOR;
    }
    else if (justEnteredNode->parent == NULL)
    {
        justEnteredNode->color = BLACK;
        return FAILURE_INDICATOR;
    }
    Node *enteredNodeParent = justEnteredNode->parent;
    if (enteredNodeParent == NULL || enteredNodeParent->color == BLACK)
    {
        return FAILURE_INDICATOR;
    }
    Node *enteredNodeGrandfather = enteredNodeParent->parent;
    if (enteredNodeGrandfather == NULL)
    {
        return FAILURE_INDICATOR;
    }
    return SUCCESS_INDICATOR;
}