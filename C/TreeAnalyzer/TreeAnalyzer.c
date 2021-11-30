/**
 * @file TreeAnalyzerc
 * @author  Omer Salman <omer.salman@mail.huji.ac.il>
 * @version 1.0
 * @date 27 November 2019
 *
 * @brief System to analyze a file describing a tree, computing a few algorithms on it and printing results.
 *
 * @section DESCRIPTION
 * System to analyze a file describing a tree, computing a few algorithms on it and printing results.
 * Input  : file describing a tree, and two vertex on that tree.
 * Process: analyze the file and check it's data validity, if it's valid: building the tree, compute a few
 * algorithms on it and print the results, if it's not valid: print an "invalid input" message to the user
 * and finish running the program.
 * Output : in case of valid input: printing to stdout the tree's:
 * Root Vertex, Vertices Count, Edges Count, Length of Minimal Branch, Length of Maximal Branch, Diameter Length,
 * Shortest Path Between two vertexes input gotten as system arguments/
 * in case of invalid input or incorrect usage of th program: prints the user a suitable message/
 */
// ------------------------------ includes ------------------------------
#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include <string.h>

// -------------------------- const definitions -------------------------
/**
 * @def NUMBER_OF_SYS_ARG 4
 * @brief A macro that sets the number of system arguments required for the program to run.
 */
#define NUMBER_OF_SYS_ARG 4

/**
 * @def LINE_CHARS_LENGTH 1025
 * @brief A macro that sets the maximal chars number components in a line.
 */
#define LINE_CHARS_LENGTH 1025

/**
 * @def DEFAULT_INITIALIZING_ZERO_VALUE 0
 * @brief A macro that sets the default initializing value of variables to zero.
 */
#define DEFAULT_INITIALIZING_ZERO_VALUE 0

/**
 * @def SUCCESS_INDICATOR 0
 * @brief A macro that sets the success indicator.
 */
#define SUCCESS_INDICATOR 0

/**
 * @def ERR0R_INDICATOR -1
 * @brief A macro that sets the error indicator.
 */
#define ERR0R_INDICATOR -1
// -------------------------- structs -------------------------
/**
 * a struct describing a vertex in a tree.
 */
typedef struct Vertex
{
    int key;
    struct Vertex **neighborVertexesPointer; //pointer to memory location contains neighbor vertexes
    int numberOfNeighbors;
    struct Vertex *fatherVertex;
    int dist;
    struct Vertex *prev;
} Vertex;

// ------------------------------ functions -----------------------------
int invalidInputMessage(FILE *fptr);

void buildVertexes(int numberOfVertexes, Vertex *treeVertexesPointer, char currentLine[], FILE *fptr);

void initiatingTreeVertexes(int numberOfVertexes, Vertex *treeVertexesPointer);

void BFS(Vertex *treeVertexesPointer, int startingVertexKey, int numberOfVertexes);

void resetVertexesDistAndPrev(Vertex *treeVertexesPointer, int numberOfVertexes);

void firstLineValidityCheck(char currentLine[], char inputStrToCmp[], FILE *fptr, int *invalidInputIndicatorPtr);

void
dataLinesValidityCheck(char currentLine[], char inputStrToCmp[], FILE *fptr, int linesCounter, int numberOfVertexes,
                       int *neighborsNumberPointer, int neighborsKeys[], int *invalidInputIndicatorPtr);

void neighborsVertexValidityCheck(char currentLine[], char inputStrToCmp[], int linesCounter, int neighborsKeys[],
                                  int *neighborsNumberPointer, int *invalidInputIndicatorPtr, int numberOfVertexes);

void vertexSystemArgumentsValidityCheck(char vertex1Key[], char vertex2Key[], int numberOfVertexes,
                                        int *invalidInputIndicatorPtr);

void computeAndPrintOutput(Vertex *treeVertexesPointer, int numberOfVertexes);

void computeAndPrintMinMaxDia(Vertex *treeVertexesPointer, Vertex rootVertex, int numberOfVertexes);

void printShortestPathBtwSysVertexes(Vertex *treeVertexesPointer, int endSysVertexKey, int startSysVertexKey,
                                     int numberOfVertexes);

void freeMemoryAllocatedInsideEachVertex(Vertex *treeVertexesPointer, int numberOfVertexes);

void
neighborsKeysValidityCheck(int neighborsNumber, int const *neighborsKeys, int numberOfVertexes,
                           int *invalidInputIndicatorPtr);

/**
 * @brief main function that runs the program.
 * @param argc number of system arguments gotten.
 * @param argv an array of pointers, aeach pointer presents a system argument gotten.
 * @return -1 in case of invalid input, 0 in case of finished running successfully.
 */
int main(int argc, char *argv[])
{
//-----------------number of arguments validity check-----------------------
    if (argc != NUMBER_OF_SYS_ARG)
    {
        fprintf(stderr, "Usage: TreeAnalyzer <Graph File Path> <First Vertex> <Second Vertex>\n");
        return EXIT_FAILURE;
    }
//-----------------empty file check-----------------------
    FILE *fptr = NULL;
    fptr = fopen(argv[1], "r");
    if (fptr == NULL)
    {
        return invalidInputMessage(fptr);
    }
    char currentLine[LINE_CHARS_LENGTH] = {DEFAULT_INITIALIZING_ZERO_VALUE};
    char inputStrToCmp[LINE_CHARS_LENGTH] = {DEFAULT_INITIALIZING_ZERO_VALUE};
    int linesCounter = -1; // linesCounter will present the current vertex we are analyzing according to the line
    // we analyze on the file/
    int numberOfVertexes = 0;
    int invalidInputIndicator = DEFAULT_INITIALIZING_ZERO_VALUE;
    int *invalidInputIndicatorPtr = &invalidInputIndicator;
//-----------------first line validity check-----------------------
    firstLineValidityCheck(currentLine, inputStrToCmp, fptr, invalidInputIndicatorPtr);
    if (*invalidInputIndicatorPtr == ERR0R_INDICATOR)
    {
        return invalidInputMessage(fptr);
    }
    else
    {
        numberOfVertexes = (int) strtol(inputStrToCmp, NULL, 10);
        linesCounter++;
    }
//------------------data lines validity check------------------------------
    int neighborsNumber = 0;
    int *neighborsNumberPointer = &neighborsNumber;
    int *neighborsKeys = (int *) malloc(numberOfVertexes * sizeof(int));
    if (neighborsKeys == NULL)
    {
        exit(1);
    }
    dataLinesValidityCheck(currentLine, inputStrToCmp, fptr, linesCounter, numberOfVertexes, neighborsNumberPointer,
                           neighborsKeys, invalidInputIndicatorPtr);
    if (*invalidInputIndicatorPtr == ERR0R_INDICATOR)
    {
        free(neighborsKeys);
        neighborsKeys = NULL;
        return invalidInputMessage(fptr);
    }
//------------------neighbors validity check------------------------------
    neighborsKeysValidityCheck(neighborsNumber, neighborsKeys, numberOfVertexes, invalidInputIndicatorPtr);
    if (*invalidInputIndicatorPtr == ERR0R_INDICATOR)
    {
        free(neighborsKeys);
        neighborsKeys = NULL;
        return invalidInputMessage(fptr);
    }
    free(neighborsKeys);
    neighborsKeys = NULL;
//------------------system arguments vertexes validity check------------------------------
    char strVertex1KeyFromSys[LINE_CHARS_LENGTH] = {DEFAULT_INITIALIZING_ZERO_VALUE};
    char strVertex2KeyFromSys[LINE_CHARS_LENGTH] = {DEFAULT_INITIALIZING_ZERO_VALUE};
    strcpy(strVertex1KeyFromSys, argv[2]);
    strcpy(strVertex2KeyFromSys, argv[3]);
    vertexSystemArgumentsValidityCheck(strVertex1KeyFromSys, strVertex2KeyFromSys, numberOfVertexes,
                                       invalidInputIndicatorPtr);
    if (*invalidInputIndicatorPtr == ERR0R_INDICATOR)
    {
        return invalidInputMessage(fptr);
    }
    int startSysVertexKey = (int) strtol(strVertex1KeyFromSys, NULL, 10);
    int endSysVertexKey = (int) strtol(strVertex2KeyFromSys, NULL, 10);
    rewind(fptr);
//    ---------------------building vertexes--------------------
    Vertex *treeVertexesPointer = NULL;
    treeVertexesPointer = (Vertex *) malloc(numberOfVertexes * sizeof(Vertex));
    if (treeVertexesPointer == NULL)
    {
        exit(1);
    }
    buildVertexes(numberOfVertexes, treeVertexesPointer, currentLine, fptr);
    fclose(fptr);
//    ---------------------computing and printing output--------------------
    computeAndPrintOutput(treeVertexesPointer, numberOfVertexes);
    printShortestPathBtwSysVertexes(treeVertexesPointer, endSysVertexKey, startSysVertexKey, numberOfVertexes);
//    ---------------------free memory allocated--------------------
    freeMemoryAllocatedInsideEachVertex(treeVertexesPointer, numberOfVertexes);
    free(treeVertexesPointer);
    treeVertexesPointer = NULL;
    return SUCCESS_INDICATOR;
}

/**
 *
 * @param fptr
 * @return
 */
int invalidInputMessage(FILE *fptr)
{
    fclose(fptr);
    fprintf(stderr, "Invalid input\n");
    return EXIT_FAILURE;
}

/**
 *
 * @param currentLine
 * @param inputStrToCmp
 * @param fptr
 * @param invalidInputIndicatorPtr
 */
void firstLineValidityCheck(char currentLine[], char inputStrToCmp[], FILE *fptr, int *invalidInputIndicatorPtr)
{
    if (fgets(currentLine, LINE_CHARS_LENGTH, fptr) != NULL)
    {
        sscanf(currentLine, "%[^\r\n]", currentLine);
        sscanf(currentLine, "%[0-9]", inputStrToCmp);
        if (strcmp(currentLine, inputStrToCmp) != SUCCESS_INDICATOR || currentLine[0] == '0')
        {
            *invalidInputIndicatorPtr = ERR0R_INDICATOR;
        }
    }
}

/**
 *
 * @param currentLine
 * @param inputStrToCmp
 * @param fptr
 * @param linesCounter
 * @param numberOfVertexes
 * @param neighborsNumberPointer
 * @param neighborsKeys
 * @param invalidInputIndicatorPtr
 */
void
dataLinesValidityCheck(char currentLine[], char inputStrToCmp[], FILE *fptr, int linesCounter, int numberOfVertexes,
                       int *neighborsNumberPointer, int neighborsKeys[], int *invalidInputIndicatorPtr)
{
    while (fgets(currentLine, LINE_CHARS_LENGTH, fptr) != NULL)
    {
        if (currentLine[0] == '-')
        {
            char lineCopy[LINE_CHARS_LENGTH] = {DEFAULT_INITIALIZING_ZERO_VALUE};
            strcpy(lineCopy, currentLine);
            sscanf(lineCopy, "%[^\r\n]", lineCopy);
            if (strcmp(lineCopy, "-") != SUCCESS_INDICATOR)
            {
                *invalidInputIndicatorPtr = ERR0R_INDICATOR;
                return;
            }
        }
        else
        {
            neighborsVertexValidityCheck(currentLine, inputStrToCmp, linesCounter, neighborsKeys,
                                         neighborsNumberPointer, invalidInputIndicatorPtr, numberOfVertexes);
            if (*invalidInputIndicatorPtr == ERR0R_INDICATOR)
            {
                return;
            }
        }
        linesCounter++;
    }
    if (numberOfVertexes != linesCounter)
    {
        *invalidInputIndicatorPtr = ERR0R_INDICATOR;
        return;
    }
}

/**
 *
 * @param currentLine
 * @param inputStrToCmp
 * @param linesCounter
 * @param neighborsKeys
 * @param neighborsNumberPointer
 * @param invalidInputIndicatorPtr
 * @param numberOfVertexes
 */
void neighborsVertexValidityCheck(char currentLine[], char inputStrToCmp[], int linesCounter, int neighborsKeys[],
                                  int *neighborsNumberPointer, int *invalidInputIndicatorPtr, int numberOfVertexes)
{
    sscanf(currentLine, "%[^\r\n]", currentLine);
    sscanf(currentLine, "%[0-9 ]", inputStrToCmp);
    if (strcmp(currentLine, inputStrToCmp) != SUCCESS_INDICATOR)
    {
        *invalidInputIndicatorPtr = ERR0R_INDICATOR;
        return;
    }
    else
    {
        char currentLineToAnalyze[LINE_CHARS_LENGTH] = {DEFAULT_INITIALIZING_ZERO_VALUE};
        strcpy(currentLineToAnalyze, currentLine);
        char *partOfCurrentLine = NULL;
        partOfCurrentLine = strtok(currentLineToAnalyze, " ");
        while (partOfCurrentLine != NULL)
        {
            int intPartOfCurrentLine = DEFAULT_INITIALIZING_ZERO_VALUE;
            intPartOfCurrentLine = (int) strtol(partOfCurrentLine, NULL, 10);
            (*neighborsNumberPointer)++;
            if (intPartOfCurrentLine == linesCounter || *neighborsNumberPointer >= numberOfVertexes)
            {
                *invalidInputIndicatorPtr = ERR0R_INDICATOR;
                return;
            }
            neighborsKeys[(*neighborsNumberPointer) - 1] = intPartOfCurrentLine;
            partOfCurrentLine = strtok(NULL, " ");
        }
    }
}

/**
 *
 * @param vertex1Key
 * @param vertex2Key
 * @param numberOfVertexes
 * @param invalidInputIndicatorPtr
 */
void vertexSystemArgumentsValidityCheck(char vertex1Key[], char vertex2Key[], int numberOfVertexes,
                                        int *invalidInputIndicatorPtr)
{
    char scannedV1Key[LINE_CHARS_LENGTH] = {DEFAULT_INITIALIZING_ZERO_VALUE};
    char scannedV2Key[LINE_CHARS_LENGTH] = {DEFAULT_INITIALIZING_ZERO_VALUE};
    sscanf(vertex1Key, "%[0-9]", scannedV1Key);
    sscanf(vertex2Key, "%[0-9]", scannedV2Key);
    if (strcmp(scannedV1Key, vertex1Key) != SUCCESS_INDICATOR)
    {
        *invalidInputIndicatorPtr = ERR0R_INDICATOR;
        return;
    }
    if (strcmp(scannedV2Key, vertex2Key) != SUCCESS_INDICATOR)
    {
        *invalidInputIndicatorPtr = ERR0R_INDICATOR;
        return;
    }
    int v1KeyInt = (int) strtol(vertex1Key, NULL, 10);
    int v2KeyInt = (int) strtol(vertex2Key, NULL, 10);
    if (v1KeyInt >= numberOfVertexes || v1KeyInt < 0)
    {
        *invalidInputIndicatorPtr = ERR0R_INDICATOR;
        return;
    }
    if (v2KeyInt >= numberOfVertexes || v2KeyInt < 0)
    {
        *invalidInputIndicatorPtr = ERR0R_INDICATOR;
        return;
    }
}

/**
 *
 * @param numberOfVertexes
 * @param treeVertexesPointer
 * @param currentLine
 * @param fptr
 */
void buildVertexes(int numberOfVertexes, Vertex *treeVertexesPointer, char currentLine[], FILE *fptr)
{
    initiatingTreeVertexes(numberOfVertexes, treeVertexesPointer);
    fgets(currentLine, LINE_CHARS_LENGTH, fptr); // skipping numberOfVertexes declaration line
    int currentVertex = 0; // starting from vertex number 0 (due to skipped line)
    while (fgets(currentLine, LINE_CHARS_LENGTH, fptr) != NULL)
    {
        if (currentLine[0] == '-')
        {
            currentVertex++;
            continue;
        }
        Vertex **neighborsPointer = (Vertex **) malloc(sizeof(Vertex *));
        if (neighborsPointer == NULL)
        {
            exit(1);
        }
        int numberOfNeighbors = 0;
        int neighborKey = DEFAULT_INITIALIZING_ZERO_VALUE;
        char *partOfCurrentLine = NULL;
        partOfCurrentLine = strtok(currentLine, " ");
        while (partOfCurrentLine != NULL)
        {
            neighborKey = (int) strtol(partOfCurrentLine, NULL, 10);
            neighborsPointer = (Vertex **) realloc(neighborsPointer, (numberOfNeighbors + 1) * sizeof(Vertex *));
            if (neighborsPointer == NULL)
            {
                exit(1);
            }
            neighborsPointer[numberOfNeighbors] = &treeVertexesPointer[neighborKey];
            treeVertexesPointer[neighborKey].fatherVertex = &treeVertexesPointer[currentVertex];
            numberOfNeighbors++;
            partOfCurrentLine = strtok(NULL, " ");
        }
        treeVertexesPointer[currentVertex].neighborVertexesPointer = neighborsPointer;
        treeVertexesPointer[currentVertex].numberOfNeighbors = numberOfNeighbors;
        currentVertex++;
    }
}

/**
 *
 * @param numberOfVertexes
 * @param treeVertexesPointer
 */
void initiatingTreeVertexes(int numberOfVertexes, Vertex *treeVertexesPointer)
{
    for (int i = 0; i < numberOfVertexes; i++)
    {
        treeVertexesPointer[i].key = i;
        treeVertexesPointer[i].neighborVertexesPointer = NULL;
        treeVertexesPointer[i].fatherVertex = NULL;
        treeVertexesPointer[i].numberOfNeighbors = 0;
        treeVertexesPointer[i].prev = NULL;
        treeVertexesPointer[i].dist = 0;
    }
}

/**
 *
 * @param treeVertexesPointer
 * @param startingVertexKey
 * @param numberOfVertexes
 */
void BFS(Vertex *treeVertexesPointer, int startingVertexKey, int numberOfVertexes)
{
    resetVertexesDistAndPrev(treeVertexesPointer, numberOfVertexes);
    treeVertexesPointer[startingVertexKey].dist = 0;
    treeVertexesPointer[startingVertexKey].prev = NULL;
    Queue *queuePointer = NULL;
    queuePointer = allocQueue();
    enqueue(queuePointer, startingVertexKey);
    unsigned int currentQueueNodeValue = DEFAULT_INITIALIZING_ZERO_VALUE; // current vertex key
    while (queueIsEmpty(queuePointer) != 1)
    {
        currentQueueNodeValue = dequeue(queuePointer);
        Vertex currentVertex = treeVertexesPointer[currentQueueNodeValue];
        for (int i = 0; i < currentVertex.numberOfNeighbors; i++)
        {
            if (currentVertex.neighborVertexesPointer[i]->dist == -1)
            {
                enqueue(queuePointer, currentVertex.neighborVertexesPointer[i]->key);
                currentVertex.neighborVertexesPointer[i]->prev = &treeVertexesPointer[currentQueueNodeValue];
                // we need the address so we will use the original vertex and not currentVertex
                currentVertex.neighborVertexesPointer[i]->dist = currentVertex.dist + 1;
            }
        }
        if (currentVertex.fatherVertex != NULL && currentVertex.fatherVertex->dist == -1)
        {
            enqueue(queuePointer, currentVertex.fatherVertex->key);
            currentVertex.fatherVertex->prev = &treeVertexesPointer[currentQueueNodeValue];
            currentVertex.fatherVertex->dist = currentVertex.dist + 1;
        }
    }
    freeQueue(&queuePointer);
}

/**
 *
 * @param treeVertexesPointer
 * @param numberOfVertexes
 */
void resetVertexesDistAndPrev(Vertex *treeVertexesPointer, int numberOfVertexes)
{
    for (int i = 0; i < numberOfVertexes; i++)
    {
        treeVertexesPointer[i].dist = -1;
        treeVertexesPointer[i].prev = NULL;
    }
}

/**
 *
 * @param treeVertexesPointer
 * @param numberOfVertexes
 */
void computeAndPrintOutput(Vertex *treeVertexesPointer, int numberOfVertexes)
{
    Vertex rootVertex = {0, NULL, 0, NULL, 0, NULL};
    for (int i = 0; i < numberOfVertexes; i++)
    {
        if (treeVertexesPointer[i].fatherVertex == NULL)
        {
            rootVertex = treeVertexesPointer[i];
            break;
        }
    }
    printf("Root Vertex: %d\n", rootVertex.key);
    printf("Vertices Count: %d\n", numberOfVertexes);
    printf("Edges Count: %d\n", numberOfVertexes - 1);
    computeAndPrintMinMaxDia(treeVertexesPointer, rootVertex, numberOfVertexes);
}

/**
 *
 * @param treeVertexesPointer
 * @param rootVertex
 * @param numberOfVertexes
 */
void computeAndPrintMinMaxDia(Vertex *treeVertexesPointer, Vertex rootVertex, int numberOfVertexes)
{
    BFS(treeVertexesPointer, rootVertex.key, numberOfVertexes);
    int lengthOfMinimalBranch = numberOfVertexes - 1; // longest route available in a tree
    int lengthOfMaximalBranch = 0; // shortest route available in a tree
    Vertex deepestLeaf = {0, NULL, 0, NULL, 0, NULL};
    for (int i = 0; i < numberOfVertexes; i++)
    {
        if (treeVertexesPointer[i].dist < lengthOfMinimalBranch &&
            treeVertexesPointer[i].neighborVertexesPointer == NULL)
        {
            lengthOfMinimalBranch = treeVertexesPointer[i].dist;
        }
        if (treeVertexesPointer[i].dist > lengthOfMaximalBranch)
        {
            lengthOfMaximalBranch = treeVertexesPointer[i].dist;
            deepestLeaf = treeVertexesPointer[i];
        }
    }
    printf("Length of Minimal Branch: %d\n", lengthOfMinimalBranch);
    printf("Length of Maximal Branch: %d\n", lengthOfMaximalBranch);
    int diameterLength = 0; // minimal possible diameter in a tree
    BFS(treeVertexesPointer, deepestLeaf.key, numberOfVertexes);
    for (int i = 0; i < numberOfVertexes; i++)
    {
        if (treeVertexesPointer[i].dist > diameterLength)
        {
            diameterLength = treeVertexesPointer[i].dist;
        }
    }
    printf("Diameter Length: %d\n", diameterLength);
}

/**
 *
 * @param treeVertexesPointer
 * @param endSysVertexKey
 * @param startSysVertexKey
 * @param numberOfVertexes
 */
void printShortestPathBtwSysVertexes(Vertex *treeVertexesPointer, int endSysVertexKey, int startSysVertexKey,
                                     int numberOfVertexes)
{
    BFS(treeVertexesPointer, endSysVertexKey, numberOfVertexes);
    Vertex *currentVertexPointer = NULL;
    currentVertexPointer = &treeVertexesPointer[startSysVertexKey];
    printf("Shortest Path Between %d and %d: ", startSysVertexKey, endSysVertexKey);
    while (currentVertexPointer->key != endSysVertexKey)
    {
        printf("%d ", currentVertexPointer->key);
        currentVertexPointer = currentVertexPointer->prev;
    }
    printf("%d\n", endSysVertexKey);
}

/**
 *
 * @param treeVertexesPointer
 * @param numberOfVertexes
 */
void freeMemoryAllocatedInsideEachVertex(Vertex *treeVertexesPointer, int numberOfVertexes)
{
    for (int i = 0; i < numberOfVertexes; i++)
    {
        if (treeVertexesPointer[i].neighborVertexesPointer != NULL)
        {
            free(treeVertexesPointer[i].neighborVertexesPointer);
            treeVertexesPointer[i].neighborVertexesPointer = NULL;
        }
    }
}

/**
 *
 * @param neighborsNumber
 * @param neighborsKeys
 * @param numberOfVertexes
 * @param invalidInputIndicatorPtr
 * @param fptr
 */
void
neighborsKeysValidityCheck(int neighborsNumber, int const *neighborsKeys, int numberOfVertexes,
                           int *invalidInputIndicatorPtr)
{
    for (int i = 0; i < neighborsNumber; i++)
    {
        if (neighborsKeys[i] > numberOfVertexes)
        {
            *invalidInputIndicatorPtr = ERR0R_INDICATOR;
            return;
        }
        else
        {
            for (int j = 0; j < neighborsNumber; j++)
            {
                if (neighborsKeys[i] == neighborsKeys[j] && i != j)
                {
                    *invalidInputIndicatorPtr = ERR0R_INDICATOR;
                    return;
                }
            }
        }
    }
}