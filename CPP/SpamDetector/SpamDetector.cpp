/**
 * @file SpamDetector.cpp
 * @author  Omer Salman <omer.salman@mail.huji.ac.il>
 * @version 1.0
 * @date 19 Jan 2020
 *
 * @brief System to identify spam messages, gets 3 system arguments:
 * 2 file path - one presents bad words data base and one presents the message, and a threshold.
 * in case program ended successfully: prints SPAM to stdout in case message is spam and NOT_SPAM in case it isn't.
 */

// ------------------------------ includes ------------------------------
#include <iostream>
#include "HashMap.hpp"
#include <boost/filesystem.hpp>
#include <boost/tokenizer.hpp>
#include <vector>

// -------------------------- const definitions -------------------------
#define VALID_NUMBER_OF_SYSTEM_ARGUMENTS 4
#define MAX_NUM_OF_SEPARATORS_IN_ONE_LINE 1


// ------------------------------ functions -----------------------------
/**
 * function checks if two files exists according to thier given pathes.
 * @param badWordsDataBaseFilePath first file path  - presents the bad words data base
 * @param messageFilePath second file path - presents the message
 * @return true if both of them exists, false otherwise.
 */
bool fileExists(const std::string &badWordsDataBaseFilePath, const std::string &messageFilePath);

/**
 * function checks if a threshold given as string is valid
 * @param thresholdStr string presents threshold
 * @return true if the threshold is valid, false otherwise.
 */
bool validThreshold(const std::string &thresholdStr);

/**
 * function checks if the number of separators in one line is valid
 * @param lineToAnalyze str presents line to analyze - check the number of separators in it
 * @return true if the number of separators in the line is valid, false otherwise.
 */
bool validNumOfSepInLine(const std::string &lineToAnalyze);

/**
 * function checks if a given string presents a valid number
 * @param str string to check
 * @return true if the string presents a valid number, false otherwise.
 */
bool strPresentsValidNumber(const std::string &str);

/**
 * function turns all chars of a source given string to uppercase and puts them in a destination given string.
 * @param sourceStr source given string
 * @param targetStr destination given string
 */
void strToUppercase(const std::string &sourceStr, std::string &targetStr);

/**
 * main function that runs the program.
 * @param argc number of system arguments given to the program
 * @param argv array of char* to acces each argumnet given to the program/
 * @return EXIT_SUCCESS in case program ended successfully, EXIT_FAILURE otherwise.
 */
int main(int argc, char *argv[])
{
    if (argc != VALID_NUMBER_OF_SYSTEM_ARGUMENTS)
    {
        std::cerr << "Usage: SpamDetector <database path> <message path> <threshold>" << std::endl;
        return EXIT_FAILURE;
    }
    std::string badWordsDataBaseFilePath = argv[1];
    std::string messageFilePath = argv[2];
    std::string thresholdStr = argv[3];
    if (!fileExists(badWordsDataBaseFilePath, messageFilePath))
    {
        return EXIT_FAILURE;
    }
    if (!validThreshold(thresholdStr))
    {
        return EXIT_FAILURE;
    }
    double threshold = std::stoi(thresholdStr);
    typedef boost::tokenizer<boost::char_separator<char>> tokenizer;
    boost::char_separator<char> sep{","};
    std::string lineToAnalyze = {};
    std::ifstream ifDataBaseStream(badWordsDataBaseFilePath);
    std::vector<std::string> badWordsVector;
    std::vector<int> valuesVector;
    while (std::getline(ifDataBaseStream, lineToAnalyze))
    {
        if (!validNumOfSepInLine(lineToAnalyze))
        {
            ifDataBaseStream.close();
            return EXIT_FAILURE;
        }
        tokenizer tok{lineToAnalyze, sep};
        int linePartsCounter = 0;
        std::string currentStr = {};
        std::string strToEnter = {};
        int valueToEnter = 0;
        for (auto beginIter = tok.begin(); beginIter != tok.end() && linePartsCounter < 2; beginIter++)
        {
            linePartsCounter++;
            currentStr = beginIter.current_token();
            if (linePartsCounter == 1)
            {
                strToEnter = currentStr;
            }
            else if (linePartsCounter == 2)
            {
                if (!strPresentsValidNumber(currentStr))
                {
                    ifDataBaseStream.close();
                    return EXIT_FAILURE;
                }
                valueToEnter = std::stoi(currentStr);
            }
        }
        if (linePartsCounter != 2)
        {
            std::cerr << "Invalid input" << std::endl;
            ifDataBaseStream.close();
            return EXIT_FAILURE;
        }
        std::string strToEnterUppercase = {};
        strToUppercase(strToEnter, strToEnterUppercase);
        badWordsVector.push_back(strToEnterUppercase);
        valuesVector.push_back(valueToEnter);
        lineToAnalyze = {};
    }
    ifDataBaseStream.close();
    std::ifstream ifMessageStream(messageFilePath);
    std::string message = {};
    std::string currentLine = {};
    while (std::getline(ifMessageStream, currentLine))
    {
        message += currentLine += "\n";
    }
    std::string messageUppercase = {};
    strToUppercase(message, messageUppercase);
    ifMessageStream.close();
    int badWordsSum = 0;
    HashMap<std::string, int> badWordsHashMap(badWordsVector, valuesVector);
    for (auto const &t : badWordsHashMap)
    {
        size_t subStrIndex = messageUppercase.find(t.first);
        while (subStrIndex != std::string::npos)
        {
            badWordsSum += t.second;
            subStrIndex = messageUppercase.find(t.first, subStrIndex + 1);
        }
    }
    if (badWordsSum >= threshold)
    {
        std::cout << "SPAM" << std::endl;
    }
    else
    {
        std::cout << "NOT_SPAM" << std::endl;
    }
    return EXIT_SUCCESS;
}

/**
 * function checks if two files exists according to thier given pathes.
 * @param badWordsDataBaseFilePath first file path  - presents the bad words data base
 * @param messageFilePath second file path - presents the message
 * @return true if both of them exists, false otherwise.
 */
bool fileExists(const std::string &badWordsDataBaseFilePath, const std::string &messageFilePath)
{
    if (!boost::filesystem::exists(badWordsDataBaseFilePath) || !boost::filesystem::exists(messageFilePath))
    {
        std::cerr << "Invalid input" << std::endl;
        return false;
    }
    return true;
}

/**
 * function checks if a threshold given as string is valid
 * @param thresholdStr string presents threshold
 * @return true if the threshold is valid, false otherwise.
 */
bool validThreshold(const std::string &thresholdStr)
{
    if (thresholdStr == "0")
    {
        std::cerr << "Invalid input" << std::endl;
        return false;
    }
    return strPresentsValidNumber(thresholdStr);
}

/**
 * function checks if the number of separators in one line is valid
 * @param lineToAnalyze str presents line to analyze - check the number of separators in it
 * @return true if the number of separators in the line is valid, false otherwise.
 */
bool validNumOfSepInLine(const std::string &lineToAnalyze)
{
    int sepCounter = 0;
    for (int i = 0; i < (int) lineToAnalyze.length() && sepCounter < 2; i++)
    {
        if (lineToAnalyze[i] == ',')
        {
            sepCounter++;
        }
    }
    if (sepCounter != MAX_NUM_OF_SEPARATORS_IN_ONE_LINE)
    {
        std::cerr << "Invalid input" << std::endl;
        return false;
    }
    return true;
}

/**
 * function checks if a given string presents a valid number
 * @param str string to check
 * @return true if the string presents a valid number, false otherwise.
 */
bool strPresentsValidNumber(const std::string &str)
{
    for (int i = 0; i < (int) str.length(); i++)
    {
        if (str[i] < '0' || str[i] > '9')
        {
            std::cerr << "Invalid input" << std::endl;
            return false;
        }
    }
    return true;
}

/**
 * function turns all chars of a source given string to uppercase and puts them in a destination given string.
 * @param sourceStr source given string
 * @param targetStr destination given string
 */
void strToUppercase(const std::string &sourceStr, std::string &targetStr)
{
    for (int i = 0; i < (int) sourceStr.length(); i++)
    {
        targetStr += toupper(sourceStr[i]);
    }
}