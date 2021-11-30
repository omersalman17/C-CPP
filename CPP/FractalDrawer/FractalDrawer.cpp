/**
 * @file FractalDrawer.cpp
 * @author  Omer Salman <omer.salman@mail.huji.ac.il>
 * @date 8 Jan 2020
 *
 * @brief program to read valid fractal description from a file and print it.
 */

// ------------------------------ includes ------------------------------

#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/tokenizer.hpp>
#include <vector>
#include "Fractal.h"

// -------------------------- const definitions -------------------------

#define FRACTAL_MINIMAL_TYPE_NUM 1
#define FRACTAL_MAXIMAL_TYPE_NUM 3
#define FRACTAL_MINIMAL_LEVEL_NUM 1
#define FRACTAL_MAXIMAL_LEVEL_NUM 6
#define MAXIMAL_LINE_LENGTH 3
#define MAXIMAL_SEPARATORS_NUM 3
#define VALID_NUMBER_OF_SYSTEM_ARGUMENTS 2

// ------------------------------ functions -----------------------------

void invalidInputErrorActions(std::ifstream &inStreamForFile);

void invalidInputErrorMsg();

void numOfSysArgCheck(const int &argc);

void fileExistsAndExtensionCheck(const std::string &filePath);

void tooLongLineCheck(const std::string &lineToAnalyze, std::ifstream &ifStream);

void tooManySeparatorsCheck(const int &linePartsCounter, std::ifstream &ifStream);

void lineDataValidityCheck(const std::string &lineToAnalyze, std::ifstream &ifStream);

void insertDataToVectors(const std::string &lineToAnalyze, std::vector<int> &fractalTypeVec,
                         std::vector<int> &fractalLevelVec);

void printFractals(const std::vector<int> &fractalTypeVec, const std::vector<int> &fractalLevelVec);

/**
 * main function that runs the program.
 * @param argc number of system arguments given to the program.
 * @param argv pointer to an array of strings presenting the system arguments given to the program.
 * @return 0 in case program ended successfully, EXIT_FAILURE code otherwise.
 */
int main(int argc, char *argv[])
{
    numOfSysArgCheck(argc);
    std::string filePath = argv[1];
    fileExistsAndExtensionCheck(filePath);
    std::ifstream ifStream(filePath);
    std::vector<int> fractalTypeVec, fractalLevelVec;
    typedef boost::tokenizer<boost::char_separator<char>> tokenizer;
    boost::char_separator<char> sep{","};
    std::string lineToAnalyze = {};
    while (std::getline(ifStream, lineToAnalyze))
    {
        if (lineToAnalyze.empty() && ifStream.eof())
        { // actions taken in case of last line in file is empty
            break;
        }
        tooLongLineCheck(lineToAnalyze, ifStream);
        tokenizer tok{lineToAnalyze, sep};
        int linePartsCounter = 0;
        for (auto beginIter = tok.begin(); beginIter != tok.end(); beginIter++)
        {
            linePartsCounter++;
            tooManySeparatorsCheck(linePartsCounter, ifStream);
        }
        lineDataValidityCheck(lineToAnalyze, ifStream);
        insertDataToVectors(lineToAnalyze, fractalTypeVec, fractalLevelVec);
        lineToAnalyze = {};
    }
    ifStream.close();
    printFractals(fractalTypeVec, fractalLevelVec);
    return 0;
}

/**
 * function checks if number of system argument given to the program is valid, if not: ending program and
 * returns EXIT_FAILURE code.
 * @param argc number of system argument given to the program.
 */
void numOfSysArgCheck(const int &argc)
{
    if (argc != VALID_NUMBER_OF_SYSTEM_ARGUMENTS)
    {
        std::cerr << "Usage: FractalDrawer <file path>" << std::endl;
        exit(EXIT_FAILURE);
    }
}

/**
 * function checks if file exists and it's extension is ".csv" according to given string presents file path,
 * if not: ending program and returns EXIT_FAILURE code.
 * @param filePath string presents file.
 */
void fileExistsAndExtensionCheck(const std::string &filePath)
{
    if (!boost::filesystem::exists(filePath))
    {
        invalidInputErrorMsg();
    }
    if (boost::filesystem::extension(filePath) != ".csv")
    {
        invalidInputErrorMsg();
    }
}

/**
 * function execute actions needed in case invalid data in file (close ifstream connected to file and executing
 * invalidInputErrorMsg func).
 * @param inStreamForFile ifstream object connected to file.
 */
void invalidInputErrorActions(std::ifstream &inStreamForFile)
{
    inStreamForFile.close();
    invalidInputErrorMsg();
}

/**
 * function prints "Invalid input" to cerr and exits the program with EXIT_FAILURE code as described in the
 * exercise guidelines.
 */
void invalidInputErrorMsg()
{
    std::cerr << "Invalid input" << std::endl;
    exit(EXIT_FAILURE);
}

/**
 * function checks if a line red from the file in too long so its considered invalid and execute suited actions.
 * @param lineToAnalyze string presents one line of data red from the file.
 * @param ifStream ifstream connected to the file.
 */
void tooLongLineCheck(const std::string &lineToAnalyze, std::ifstream &ifStream)
{
    if (lineToAnalyze.length() > MAXIMAL_LINE_LENGTH)
    {
        invalidInputErrorActions(ifStream);
    }
}

/**
 * functions checks if in a line red from the file there are too many separators (like ",") so its considered invalid
 * and execute suited actions.
 * @param linePartsCounter int presents counter for how many parts of one line separated by the defines separator.
 * @param ifStream ifstream connected to the file.
 */
void tooManySeparatorsCheck(const int &linePartsCounter, std::ifstream &ifStream)
{
    if (linePartsCounter >= MAXIMAL_SEPARATORS_NUM)
    {
        invalidInputErrorActions(ifStream);
    }
}

/**
 * function checks if a line's (red from the file) data is invalid and execute suited actions.
 * @param lineToAnalyze string presents one line of data red from the file.
 * @param ifStream ifstream connected to the file.
 */
void lineDataValidityCheck(const std::string &lineToAnalyze, std::ifstream &ifStream)
{
    char fractalType = lineToAnalyze[0];
    char separator = lineToAnalyze[1];
    char fractalLevel = lineToAnalyze[2];
    int fractalTypeNum = fractalType - '0'; // made using ASCII
    int fractalLevelNum = fractalLevel - '0'; // made using ASCII
    if (separator != ',')
    {
        invalidInputErrorActions(ifStream);
    }
    if (fractalTypeNum < FRACTAL_MINIMAL_TYPE_NUM || fractalTypeNum > FRACTAL_MAXIMAL_TYPE_NUM)
    {
        invalidInputErrorActions(ifStream);
    }
    if (fractalLevelNum < FRACTAL_MINIMAL_LEVEL_NUM || fractalLevelNum > FRACTAL_MAXIMAL_LEVEL_NUM)
    {
        invalidInputErrorActions(ifStream);
    }
}

/**
 * function insert data red from a file's line presents fractal type number and fractal level number each one to
 * it's suited vector.
 * @param lineToAnalyze string presents one line of data red from the file.
 * @param fractalTypeVec vector of ints presents each fractal type number.
 * @param fractalLevelVec vector of ints presents each fractal level number.
 */
void insertDataToVectors(const std::string &lineToAnalyze, std::vector<int> &fractalTypeVec,
                         std::vector<int> &fractalLevelVec)
{
    char fractalType = lineToAnalyze[0];
    char fractalLevel = lineToAnalyze[2];
    int fractalTypeNum = fractalType - '0'; // made using ASCII
    int fractalLevelNum = fractalLevel - '0'; // made using ASCII
    fractalTypeVec.push_back(fractalTypeNum);
    fractalLevelVec.push_back(fractalLevelNum);
}

/**
 * function prints fractal's according to their description red from the file.
 * @param fractalTypeVec vector of ints presents each fractal type number.
 * @param fractalLevelVec vector of ints presents each fractal level number.
 */
void printFractals(const std::vector<int> &fractalTypeVec, const std::vector<int> &fractalLevelVec)
{
    for (int i = (int) fractalTypeVec.size() - 1; i >= 0; i--)
    {
        FractalFactory::printSuitedFractal(fractalTypeVec[i], fractalLevelVec[i]);
    }
}