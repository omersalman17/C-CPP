/**
 * @file manageStudents.c
 * @author  Omer Salman <omer.salman@mail.huji.ac.il>
 * @version 1.0
 * @date 10 Nov 2019
 *
 * @brief System to manage university students details.
 *
 * @section DESCRIPTION
 * The system manage university students details and enables the user to execute a few
 * sorting functions.
 * Input  : student details.
 * Process: Sorting the students list at the end of the input.
 * Output : Sorted students list according to a specified parameter or outputing the best
 * student according to
 * age and grade.
 */

// ------------------------------ includes ------------------------------
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

// -------------------------- const definitions -------------------------
/**
 * @def MAXIMUM_NUMBER_OF_STUDENTS 5000
 * @brief A macro that sets the maximal input students number allowed for the system.
 */
#define MAXIMUM_NUMBER_OF_STUDENTS 5000

/**
 * @def HALF_MAXIMUM_NUMBER_OF_STUDENTS 2500
 * @brief A macro that sets half of the maximal input students number allowed for the system.
 */
#define HALF_MAXIMUM_NUMBER_OF_STUDENTS 2500

/**
 * @def LINE_LENGTH 151
 * @brief A macro that sets the maximal chars number for student's input information line.
 */
#define LINE_LENGTH 151

/**
 * @def FIELD_LENGTH  151
 * @brief A macro that sets the maximal chars number for any input field.
 */
#define FIELD_LENGTH 41

/**
 * @def TRUE
 * @brief A macro that defines 0 int as "true" word for better code understanding.
 */
#define TRUE 0

/**
 * @def FALSE
 * @brief A macro that defines 1 int as "false" word for better code understanding.
 */
#define FALSE 1

/**
 * @def NUMBER_OF_SYSTEM_ARGUMENTS
 * @brief A macro that defines the number of system arguments allowed for the
 * program to receive in order to run.
 */
#define  NUMBER_OF_SYSTEM_ARGUMENTS 2

/**
 * @def MINIMAL_GRADE
 * @brief A macro that defines the minimal grade for a student.
 */
#define MINIMAL_GRADE 0

/**
 * @def MAXIMAL_GRADE
 * @brief A macro that defines the maximal grade for a student.
 */
#define MAXIMAL_GRADE 100

/**
 * @def MINIMAL_AGE
 * @brief A macro that defines the minimal age for a student.
 */
#define MINIMAL_AGE 18

/**
 * @def MAXIMAL_AGE
 * @brief A macro that defines the maximal age for a student.
 */
#define MAXIMAL_AGE 120

/**
 * @def LEGAL_ID_DIGITS_NUMBER
 * @brief A macro that defines the legal digits number in a student's ID (it's length).
 */
#define LEGAL_ID_DIGITS_NUMBER 10

/**
 * @def LEGAL_NUMERIC_FIELD_DIGITS_NUMBER
 * @brief A macro that defines the legal digits number in a numeric parameter of
 * a student such as grade or age
 * (both of them can contain only 3 digits number).
 */
#define LEGAL_NUMERIC_FIELD_DIGITS_NUMBER 3

/**
 * @typedef struct presents a student.
 * @brief A struct that presents a student with various details relevant for university
 * students management program.
 * @param ID an array of chars (string) presents student's ID.
 * @param Name an array of chars (string) presents student's name.
 * @param Grade an int presents student's grade.
 * @param Age an int presents student's age.
 * @param Country an array of chars (string) presents student's country.
 * @param City an array of chars (string) presents student's city.
 * @param Talent a double presents the student's talent, calculated from: grade / age .
 */
typedef struct Student
{
    char ID[FIELD_LENGTH];
    char Name[FIELD_LENGTH];
    int Grade;
    int Age;
    char Country[FIELD_LENGTH];
    char City[FIELD_LENGTH];
    double Talent;
} Student;

int idValidityCheck(char id[]);

int nameValidityCheck(char name[]);

int gradeValidityCheck(char grade[]);

int ageValidityCheck(char age[]);

int countryAndCityValidityCheck(char countryOrCity[]);

int inputValidityCheck(char id[], char name[], char grade[], char age[], char country[],
                       char city[], int linesCounter);

void quickSort(Student studentsArray[], int leftIndex, int rightIndex);

int partition(Student studentsArray[], int leftIndex, int rightIndex);

void swapStudents(Student studentsArray[], int firstIndex, int secondIndex);

void initializeNewStudent(Student studentsArr[], int studentsCounter, char id[], char name[],
                          char grade[], char age[], char country[], char city[]);

void initiatingTwoSubArrays(Student studentsArray[], Student leftArray[], int leftArraySize,
                            Student rightArray[], int rightArraySize, int left, int middle);

void assignRemainingValues(Student studentsArray[], Student rightArray[], int rightArraySize,
                           Student leftArray[], int leftArraySize, int i, int m, int j);

int argsAreNotValid(int argc, int commandIsBest, int commandIsMerge, int commandIsQuick);

void printSortedStudentsArray(Student studentsArr[], int studentsCounter);

void merge(Student studentsArray[], int left, int middle, int right);

void mergeSort(Student studentsArray[], int left, int right);

int executingCommand(Student studentsArr[], int studentsCounter, int commandIsBest,
                     int commandIsQuick, int commandIsMerge);


// ------------------------------ functions -----------------------------
/**
 * @brief The main function, runs the program using various sub-functions.
 * @return 0, to tell the system execution ended without errors, 1 if the system arguments
 * are not valid.
 */
int main(int argc, char **argv)
{
    Student studentsArr[MAXIMUM_NUMBER_OF_STUDENTS];
    char studentInfo[LINE_LENGTH];
    char id[FIELD_LENGTH], name[FIELD_LENGTH], grade[FIELD_LENGTH], age[FIELD_LENGTH],
            country[FIELD_LENGTH], city[FIELD_LENGTH];
    char command[10];
    strcpy(command, argv[1]);
    int commandIsBest, commandIsQuick, commandIsMerge;
    commandIsBest = strcmp(command, "best\0");
    commandIsQuick = strcmp(command, "quick\0");
    commandIsMerge = strcmp(command, "merge\0");
    if (argsAreNotValid(argc, commandIsBest, commandIsMerge, commandIsQuick))
    {
        return FALSE;
    }
    int studentsCounter = 0;
    int linesCounter = -1;
    do
    {
        printf("Enter student info. To exit press q, then enter\n");
        fgets(studentInfo, LINE_LENGTH, stdin);
        sscanf(studentInfo, "%[^\t]\t%[^\t]\t%[^\t]\t%[^\t]\t%[^\t]\t%[^\t]\t", id, name,
               grade, age, country, city);
        linesCounter += 1;
        if (strcmp(studentInfo, "q\r\n") == TRUE || strcmp(studentInfo, "q\n") == TRUE)
        {
            break;
        }
        else if (inputValidityCheck(id, name, grade, age, country, city, linesCounter) == TRUE)
        {
            initializeNewStudent(studentsArr, studentsCounter, id, name, grade, age,
                                 country, city);
            studentsCounter += 1;
        }
    } while (studentsCounter <= MAXIMUM_NUMBER_OF_STUDENTS);
    return executingCommand(studentsArr, studentsCounter, commandIsBest,
                            commandIsQuick, commandIsMerge);
}

/**
 * @brief executing command given as a system argument after the user finished entering
 * the students details
 * to the system
 * @param studentsArr array of student structs.
 * @param studentsCounter int presents students number in the students array.
 * @param commandIsBest int presents if the command is "best", 0 if it is and other
 * number if it's not.
 * @param commandIsQuick int presents if the command is "quick", 0 if it is and other
 * number if it's not.
 * @param commandIsMerge int presents if the command is "merge", 0 if it is and other
 * number if it's not.
 * @return 0 (indicator for the program finished successfully).
 */
int executingCommand(Student studentsArr[], int studentsCounter, int commandIsBest,
                     int commandIsQuick, int commandIsMerge)
{
    if (commandIsBest == TRUE && studentsCounter >= 1)
    {
        Student bestStudent = studentsArr[0];
        for (int i = 1; i < studentsCounter; i++)
        {
            if (bestStudent.Talent < studentsArr[i].Talent)
            {
                bestStudent = studentsArr[i];
            }
        }
        printf("best student info is: %s\t%s\t%d\t%d\t%s\t%s\t\n", bestStudent.ID,
               bestStudent.Name, bestStudent.Grade, bestStudent.Age, bestStudent.Country,
               bestStudent.City);

        return TRUE;
    }
    else if (commandIsQuick == TRUE)
    {
        quickSort(studentsArr, 0, studentsCounter - 1);
    }
    else if (commandIsMerge == TRUE)
    {
        mergeSort(studentsArr, 0, studentsCounter - 1);
    }
    printSortedStudentsArray(studentsArr, studentsCounter);
    return TRUE;
}

/**
 * @brief returns an int indicator for the validity of the system arguments given to the program.
 * @param argc int presents number of system arguments that were given to the program.
 * @param commandIsBest int presents if the command is "best", 0 if it is and other number
 * if it's not.
 * @param commandIsQuick int presents if the command is "quick", 0 if it is and other number
 * if it's not.
 * @param commandIsMerge int presents if the command is "merge", 0 if it is and other number
 * if it's not.
 * @return 0 if the args are valid, 1 otherwise.
 */
int argsAreNotValid(int argc, int commandIsBest, int commandIsMerge, int commandIsQuick)
{
    if (argc == NUMBER_OF_SYSTEM_ARGUMENTS)
    {
        if (commandIsBest == TRUE || commandIsMerge == TRUE || commandIsQuick == TRUE)
        {
            return TRUE;
        }
    }
    printf("USAGE: please enter best or quick or merge as system argument\n");
    return FALSE;
}

/**
 * @brief Print the students array after the sorting methods were executed according to the
 * system arguments that
 * were given.
 * @param studentsArr students structs array.
 * @param studentsCounter int presents the students number in the array.
 */
void printSortedStudentsArray(Student studentsArr[], int studentsCounter)
{
    for (int i = 0; i < studentsCounter; i++)
    {
        printf("%s\t%s\t%d\t%d\t%s\t%s\t\n", studentsArr[i].ID, studentsArr[i].Name,
               studentsArr[i].Grade, studentsArr[i].Age, studentsArr[i].Country,
               studentsArr[i].City);
    }
}

/**
 * @brief initialize new student and adding it's details for the students array;
 * @param studentsArr students structs array
 * @param studentsCounter int presents the students number in the array.
 * @param id array of chars (string) presents the id of a student.
 * @param name array of chars (string) presents the name of a student.
 * @param grade array of chars (string) presents the grade of a student.
 * @param age array of chars (string) presents the age of a student.
 * @param country array of chars (string) presents the country of a student.
 * @param city array of chars (string) presents the city of a student.
 */
void initializeNewStudent(Student studentsArr[], int studentsCounter, char id[], char name[],
                          char grade[], char age[], char country[], char city[])
{
    int gradeInt = (int) strtol(grade, NULL, 10);
    int ageInt = (int) strtol(age, NULL, 10);
    strcpy(studentsArr[studentsCounter].ID, id);
    strcpy(studentsArr[studentsCounter].Name, name);
    strcpy(studentsArr[studentsCounter].Country, country);
    strcpy(studentsArr[studentsCounter].City, city);
    studentsArr[studentsCounter].Grade = gradeInt;
    studentsArr[studentsCounter].Age = ageInt;
    studentsArr[studentsCounter].Talent = (double) gradeInt / ageInt;
}

/**
 * @brief checks if the student's details the user entered are valid, if they are, returns 0,
 * otherwise returns 1.
 * @param id array of chars (string) presents the id of a student.
 * @param name array of chars (string) presents the name of a student.
 * @param grade array of chars (string) presents the grade of a student.
 * @param age array of chars (string) presents the age of a student.
 * @param country array of chars (string) presents the country of a student.
 * @param city array of chars (string) presents the city of a student.
 * @param linesCounter int presents the user's input current line.
 * @return 0 if the students details the user entered are valid, otherwise returns 1.
 */
int inputValidityCheck(char id[], char name[], char grade[], char age[], char country[],
                       char city[], int linesCounter)
{
    if (idValidityCheck(id) != TRUE)
    {
        printf("in line %d\n", linesCounter);
        return FALSE;
    }
    else if (nameValidityCheck(name) != TRUE)
    {
        printf("in line %d\n", linesCounter);
        return FALSE;
    }
    else if (gradeValidityCheck(grade) != TRUE)
    {
        printf("in line %d\n", linesCounter);
        return FALSE;
    }
    else if (ageValidityCheck(age) != TRUE)
    {
        printf("in line %d\n", linesCounter);
        return FALSE;
    }
    else if (countryAndCityValidityCheck(country) != TRUE)
    {
        printf("in line %d\n", linesCounter);
        return FALSE;
    }
    else if (countryAndCityValidityCheck(city) != TRUE)
    {
        printf("in line %d\n", linesCounter);
        return FALSE;
    }
    return TRUE;
}

/**
 * @brief checks if the student's id the user entered is valid, if it is, returns 0,
 * otherwise prints an informative error message and returns 1.
 * @param id array of chars (string) presents the id of a student.
 * @return 0 if the students id the user entered is valid, otherwise returns 1.
 */
int idValidityCheck(char id[])
{
    char legalIdNumber[FIELD_LENGTH];
    if (id[0] != '0')
    {
        sscanf(id, "%[0-9]", legalIdNumber);
        if (strcmp(legalIdNumber, id) == TRUE)
        {
            if (strlen(legalIdNumber) == LEGAL_ID_DIGITS_NUMBER)
            {
                return TRUE;
            }
        }
    }
    printf("ERROR: ID is only 10 digit number not starting with 0\n");
    return FALSE;
}

/**
 * @brief checks if the student's name the user entered is valid, if it is, returns 0,
 * otherwise prints an informative
 * error message and returns 1.
 * @param name array of chars (string) presents the id of a student.
 * @return 0 if the students name the user entered is valid, otherwise returns 1.
 */
int nameValidityCheck(char name[])
{
    char legalName[FIELD_LENGTH];
    sscanf(name, "%[a-zA-Z -]", legalName);
    if (strcmp(legalName, name) == TRUE)
    {
        return TRUE;
    }
    printf("ERROR: Name can only contain english letters, whitespaces or ""-""\n");
    return FALSE;
}

/**
 * @brief checks if an input the user entered for a numeric field (such as age and grade)
 * contains only digits, if it is returns the number converted from array of chars (string)
 * to int type, otherwise returns -1.
 * @param num array of chars (string) presents a number.
 * @return in case num contains only digits, returns num as int type, otherwise returns -1.
 */
int numericInputValidityCheck(char num[])
{
    char onlyNumbers[FIELD_LENGTH];
    if (strlen(num) <= LEGAL_NUMERIC_FIELD_DIGITS_NUMBER)
    {
        sscanf(num, "%[0-9]", onlyNumbers);
        if (strcmp(onlyNumbers, num) == TRUE)
        {
            int numInt;
            numInt = (int) strtol(num, NULL, 10);
            return numInt;
        }
    }
    return -1; // -1 is an indicator for error
}

/**
 * @brief checks if the student's grade the user entered is valid, if it is, returns 0,
 * otherwise prints an informative error message and returns 1.
 * @param grade array of chars (string) presents the grade of a student.
 * @return 0 if the students garde the user entered is valid, otherwise returns 1.
 */
int gradeValidityCheck(char grade[])
{
    int gradeInt = numericInputValidityCheck(grade);
    if (gradeInt <= MAXIMAL_GRADE && gradeInt >= MINIMAL_GRADE)
    {
        return TRUE;
    }
    printf("ERROR: Grade is an integer between 0 and 100 (includes)\n");
    return FALSE;
}

/**
 * @brief checks if the student's age the user entered is valid, if it is, returns 0,
 * otherwise prints an informative error message and returns 1.
 * @param age array of chars (string) presents the age of a student.
 * @return 0 if the students age the user entered is valid, otherwise returns 1.
 */
int ageValidityCheck(char age[])
{
    int ageInt = numericInputValidityCheck(age);
    if (ageInt <= MAXIMAL_AGE && ageInt >= MINIMAL_AGE)
    {
        return TRUE;
    }
    printf("ERROR: Age is an integer between 18 and 120 (includes)\n");
    return FALSE;
}

/**
 * @brief checks if the student's country and city the user entered is valid, if it is,
 * returns 0, otherwise prints an informative error message and returns 1.
 * @param countryOrCity array of chars (string) presents the country of city of a student.
 * @return 0 if the students country and city the user entered is valid, otherwise returns 1.
 */
int countryAndCityValidityCheck(char countryOrCity[])
{
    char legalCityOrCountry[FIELD_LENGTH];
    sscanf(countryOrCity, "%[a-zA-Z-]", legalCityOrCountry);
    if (strcmp(countryOrCity, legalCityOrCountry) == TRUE)
    {
        return TRUE;
    }
    printf("ERROR: Country/City can contain only letters and ""-""\n");
    return FALSE;
}

/**
 * @brief quicksorting a given students array according to lexicographic order of their names.
 * @param studentsArr students structs array.
 * @param leftIndex of the array for quicksorting.
 * @param rightIndex of the array for quicksorting.
 */
void quickSort(Student studentsArray[], int leftIndex, int rightIndex)
{
    int m;
    if (leftIndex < rightIndex)
    {
        m = partition(studentsArray, leftIndex, rightIndex);
        quickSort(studentsArray, leftIndex, m - 1);
        quickSort(studentsArray, m + 1, rightIndex);
    }
}

/**
 * @brief partition algorithm for quicks-orting a given students array according to
 * lexicographic order of their names.
 * @param studentsArr students structs array.
 * @param leftIndex of the array for quick-sorting.
 * @param rightIndex of the array for quick-sorting.
 * @return partition index as part of quick sort.
 */
int partition(Student studentsArray[], int leftIndex, int rightIndex)
{
    char pivot[FIELD_LENGTH];
    strcpy(pivot, studentsArray[rightIndex].Name);
    int i = leftIndex;
    int j = rightIndex - 1;
    while (1)
    {
        while (strcmp(studentsArray[i].Name, pivot) < 0)
        {
            i++;
        }
        while (strcmp(studentsArray[j].Name, pivot) > 0)
        {
            j--;
        }
        if (i >= j)
        {
            break;
        }
        else
        {
            swapStudents(studentsArray, i, j);
        }
    }
    swapStudents(studentsArray, i, rightIndex);
    return i;
}

/**
 * @brief swapping two students indexes in the students array.
 * @param studentsArr students structs array.
 * @param firstIndex of student in the array.
 * @param secondIndex of student in the array.
 */
void swapStudents(Student studentsArray[], int firstIndex, int secondIndex)
{
    Student tmpStudent;
    tmpStudent = studentsArray[firstIndex];
    studentsArray[firstIndex] = studentsArray[secondIndex];
    studentsArray[secondIndex] = tmpStudent;
}

/**
 * @brief mergesorting a students structs array according to a given comparison parameter.
 * @param studentsArr students structs array.
 * @param left left index for the merge sort algorithm.
 * @param right right index for the merge sort algorithm.
 */
void mergeSort(Student studentsArray[], int left, int right)
{
    if (left < right)
    {
        int middle = left + (right - left) / 2;
        mergeSort(studentsArray, left, middle);
        mergeSort(studentsArray, middle + 1, right);
        merge(studentsArray, left, middle, right);
    }
}

/**
 * @brief merge two students structs arrays, used in merge sort algorithm .
 * @param studentsArr students structs array.
 * @param left left index for the merge sort algorithm.
 * @param middle middle index for the merge sort algorithm.
 * @param right right index for the merge sort algorithm.
 */
void merge(Student studentsArray[], int left, int middle, int right)
{
    int i = 0;
    int j = 0;
    int m = left;
    int leftArraySize = middle - left + 1;
    int rightArraySize = right - middle;
    Student rightArray[HALF_MAXIMUM_NUMBER_OF_STUDENTS],
            leftArray[HALF_MAXIMUM_NUMBER_OF_STUDENTS];
    initiatingTwoSubArrays(studentsArray, leftArray, leftArraySize, rightArray,
                           rightArraySize, left, middle);
    while (i < leftArraySize && j < rightArraySize)
    {
        if (leftArray[i].Grade <= rightArray[j].Grade)
        {
            studentsArray[m] = leftArray[i];
            i++;
        }
        else
        {
            studentsArray[m] = rightArray[j];
            j++;
        }
        m++;
    }
    assignRemainingValues(studentsArray, rightArray, rightArraySize, leftArray,
                          leftArraySize, i, m, j);
}

/**
 * @brief initiating students structs details for two sub-arrays of the main
 * students array, used for
 * merge sorting the main students array.
 * @param studentsArr students structs main array.
 * @param leftArray students left sub-array of the main students array.
 * @param leftArraySize int presents students left sub-array size.
 * @param rightArray students right sub-array of the main students array.
 * @param rightArraySize int presents students right sub-array size..
 * @param left left index of the merge sort algorithm.
 * @param middle middle index of the merge sort algorithm.
 */
void initiatingTwoSubArrays(Student studentsArray[], Student leftArray[], int leftArraySize,
                            Student rightArray[], int rightArraySize, int left, int middle)
{
    for (int i = 0; i < leftArraySize; i++)
    {
        leftArray[i] = studentsArray[left + i];
    }
    for (int j = 0; j < rightArraySize; j++)
    {
        rightArray[j] = studentsArray[middle + 1 + j];
    }
}

/**
 * @brief assigning remaining students from two sub-arrays to the main array.
 * @param studentsArr students structs main array.
 * @param rightArray students right sub-array of the main students array.
 * @param rightArraySize int presents students right sub-array size.
 * @param leftArray students left sub-array of the main students array.
 * @param leftArraySize int presents students left sub-array size.
 * @param i i index of the merge sort algorithm.
 * @param m m index of the merge sort algorithm.
 * @param j j index of the merge sort algorithm.
 */
void assignRemainingValues(Student studentsArray[], Student rightArray[], int rightArraySize,
                           Student leftArray[], int leftArraySize, int i, int m, int j)
{
    while (i < leftArraySize)
    {
        studentsArray[m] = leftArray[i];
        i++;
        m++;
    }
    while (j < rightArraySize)
    {
        studentsArray[m] = rightArray[j];
        j++;
        m++;
    }
}