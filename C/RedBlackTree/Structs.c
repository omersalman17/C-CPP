/**
 * @file Structs.c
 * @author  Omer Salman <omer.salman@mail.huji.ac.il>
 * @version 1.0
 * @date 11 Dec 2019
 *
 * @brief functions of a generic vector struct.
 *
 *
 * @section DESCRIPTION
 * functions of a generic vector struct..
 */

// ------------------------------ includes ------------------------------
#include <stdio.h>
#include "Structs.h"
#include <stdlib.h>
#include <string.h>

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

// ------------------------------ functions -----------------------------
void copyVecAToVecB(Vector *vecA, Vector *vecB);

double getVecNormPow2(Vector *vec);

/**
 * CompFunc for strings (assumes strings end with "\0")
 * @param a - char* pointer
 * @param b - char* pointer
 * @return equal to 0 iff a == b. lower than 0 if a < b. Greater than 0 iff b < a. (lexicographic
 * order)
 */
int stringCompare(const void *a, const void *b)
{
    if (a == NULL || b == NULL)
    {
        return -1;
    }
    char *stringA = (char *) (a);
    char *stringB = (char *) (b);
    return strcmp(stringA, stringB);
}

/**
 * ForEach function that concatenates the given word to pConcatenated. pConcatenated is already allocated with
 * enough space.
 * @param word - char* to add to pConcatenated
 * @param pConcatenated - char*
 * @return 0 on failure, other on success
 */
int concatenate(const void *word, void *pConcatenated)
{
    if (word == NULL || pConcatenated == NULL)
    {
        return FAILURE_INDICATOR;
    }
    char *wordStr = (char *) word;
    char *pConcatenatedStr = (char *) pConcatenated;
    strcat(pConcatenatedStr, wordStr);
    strcat(pConcatenatedStr, "\n");
    return SUCCESS_INDICATOR;
}

/**
 * FreeFunc for strings
 */
void freeString(void *s)
{
    if (s == NULL)
    {
        return;
    }
    free(s);
}

/**
 * CompFunc for Vectors, compares element by element, the vector that has the first larger
 * element is considered larger. If vectors are of different lengths and identify for the length
 * of the shorter vector, the shorter vector is considered smaller.
 * @param a - first vector
 * @param b - second vector
 * @return equal to 0 iff a == b. lower than 0 if a < b. Greater than 0 iff b < a.
 */
int vectorCompare1By1(const void *a, const void *b)
{
    Vector *vectorA = (Vector *) (a);
    Vector *vectorB = (Vector *) (b);
    int commonLength = vectorA->len;
    if (vectorA->len > vectorB->len)
    {
        commonLength = vectorB->len;
    }
    double *vectorAData = vectorA->vector;
    double *vectorBData = vectorB->vector;
    for (int i = 0; i < commonLength; i++)
    {
        if (vectorAData[i] > vectorBData[i])
        {
            return 1;
        }
        else if (vectorAData[i] < vectorBData[i])
        {
            return -1;
        }
    }
    if (vectorA->len > vectorB->len)
    {
        return 1;
    }
    else if (vectorA->len < vectorB->len)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

/**
 * FreeFunc for vectors
 */
void freeVector(void *pVector)
{
    if (pVector == NULL)
    {
        return;
    }
    Vector *pVec = (Vector *) (pVector);
    free(pVec->vector);
    free(pVec);
}

/**
 * copy pVector to pMaxVector if : 1. The norm of pVector is greater then the norm of pMaxVector.
 * 								   2. pMaxVector == NULL.
 * @param pVector pointer to Vector
 * @param pMaxVector pointer to Vector
 * @return 1 on success, 0 on failure (if pVector == NULL: failure).
 */
int copyIfNormIsLarger(const void *pVector, void *pMaxVector)
{
    if (pVector == NULL)
    {
        return FAILURE_INDICATOR;
    }
    Vector *pVec = (Vector *) (pVector);
    if (pMaxVector == NULL)
    {
        pMaxVector = (Vector *) malloc(1 * sizeof(Vector));

        if (pMaxVector == NULL)
        {
            return FAILURE_INDICATOR;
        }
        copyVecAToVecB(pVec, pMaxVector);
        return SUCCESS_INDICATOR;
    }
    else
    {
        Vector *pMaxVec = (Vector *) (pMaxVector);
        double pVecNormPow2 = getVecNormPow2(pVec);
        double pMaxVecNormPow2 = getVecNormPow2(pMaxVec);
        if (pVecNormPow2 > pMaxVecNormPow2)
        {
            copyVecAToVecB(pVec, pMaxVec);
            return SUCCESS_INDICATOR;
        }
    }
    return FAILURE_INDICATOR;
}

/**
 * @param tree a pointer to a tree of Vectors
 * @return pointer to a *copy* of the vector that has the largest norm (L2 Norm).
 */
Vector *findMaxNormVectorInTree(RBTree *tree)
{
    if (tree == NULL)
    {
        return NULL;
    }
    Vector *maxNormVec = (Vector *) malloc(1 * sizeof(Vector));
    maxNormVec->len = 0;
    maxNormVec->vector = NULL;
    forEachRBTree(tree, copyIfNormIsLarger, maxNormVec);
    return maxNormVec;
}

/**
 * function copies data from a given vector to another given vector
 * @param vecA vector to copy data from
 * @param vecB vector to copy data to
 */
void copyVecAToVecB(Vector *vecA, Vector *vecB)
{
    if (vecA == NULL)
    {
        return;
    }
    if (vecA->vector == NULL)
    {
        return;
    }
    vecB->vector = (double *) realloc(vecB->vector, (vecA->len * sizeof(double)));
    vecB->len = vecA->len;
    for (int i = 0; i < vecB->len; i++)
    {
        vecB->vector[i] = vecA->vector[i];
    }
}

/**
 * function computes a given vector's norm pow 2 and returns it, if the vector's data is empty: returns 0/
 * @param vec a given vector
 * @return the vector's norm, if the vector's data is empty: returns 0.
 */
double getVecNormPow2(Vector *vec)
{
    if (vec == NULL)
    {
        return -1;
    }
    double *vecData = vec->vector;
    double vecNormPow2 = 0;
    if (vecData == NULL)
    {
        return 0;
    }
    for (int i = 0; i < (vec->len); i++)
    {
        vecNormPow2 += (vecData[i] * vecData[i]);
    }
    return vecNormPow2;
}