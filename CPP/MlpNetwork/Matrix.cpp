//
// Created by omer on 12/18/2019.
//

/**
 * @file Matrix.cpp
 * @author  Omer Salman <omer.salman@mail.huji.ac.il>
 * @date 26 Dec 2019
 *
 * @brief Matrix object class
 */

// ------------------------------ includes ------------------------------

#include <new>
#include <cstdlib>
#include <iostream>
#include "Matrix.h"

// -------------------------- const definitions -------------------------

#define EXIT_CODE 1

// ------------------------------ private functions - not part of the API -----------------------------

/**
 * function initializes all given matrix elements to 0
 * @param matrix given matrix
 * @param rows matrix's number of rows
 * @param cols matrix's number of columns
 */
void initMatrixElemsTo0(float *matrix, const int &rows, const int &cols)
{
    for (int i = 0; i < rows * cols; i++)
    {
        matrix[i] = 0;
    }
}

// ------------------------------ constructors and destructors -----------------------------

/**
 * constructor for matrix object with given number of rows and columns
 * @param rows matrix's number of rows
 * @param cols matrix's number of columns
 */
Matrix::Matrix(const int rows, const int cols) : matrixDims({rows, cols})
{
    if (rows <= 0 || cols <= 0)
    {
        std::cerr << "Error: cant build matrix with negative number of rows and columns" << std::endl;
        exit(EXIT_CODE);
    }
    matrix = new(std::nothrow) float[rows * cols];
    if (matrix == nullptr)
    {
        std::cerr << "Error: couldn't allocate needed memory" << std::endl;
        exit(EXIT_CODE);
    }
    initMatrixElemsTo0(matrix, rows, cols);
}

/**
 * default constructor for matrix objects, constructs a matrix with 1 row and 1 column
 */
Matrix::Matrix() : Matrix(1, 1)
{
}

/**
 * copy constructor for matrix object
 * @param m matrix to construct a new matrix from with identical values
 */
Matrix::Matrix(const Matrix &m) : matrixDims({m.getRows(), m.getCols()})
{
    matrix = new(std::nothrow) float[m.getRows() * m.getCols()];
    if (matrix == nullptr)
    {
        std::cerr << "Error: couldn't allocate needed memory" << std::endl;
        exit(EXIT_CODE);
    }
    for (int i = 0; i < matrixDims.rows * matrixDims.cols; i++)
    {
        matrix[i] = m.matrix[i];
    }
}

/**
 * destructor for matrix object
 */
Matrix::~Matrix()
{
    delete[]matrix;
}

// ------------------------------ public functions - part of the API -----------------------------

/**
 * getter for matrix's number of rows
 * @return matrix's number of rows
 */
int Matrix::getRows() const
{
    return matrixDims.rows;
}

/**
 * getter for matrix's number of columns
 * @return matrix's number of columns
 */
int Matrix::getCols() const
{
    return matrixDims.cols;
}

/**
 * function turns the matrix object to vector (1 column) without loosing any of the matrix's values
 * @return
 */
Matrix &Matrix::vectorize()
{
    matrixDims.rows = matrixDims.rows * matrixDims.cols;
    matrixDims.cols = 1;
    return *this;
}

/**
 * plain prints the matrix's values
 */
void Matrix::plainPrint() const
{
    for (int i = 0; i < this->matrixDims.rows; i++)
    {
        for (int j = 0; j < this->matrixDims.cols; j++)
        {
            std::cout << this->matrix[(i * matrixDims.cols) + j] << " ";
        }
        std::cout << std::endl;
    }
}

/**
 * overloading operator "=" for matrix objects : function copy given matrix's values to this matrix and returns a
 * reference to this matrix in order to enable concatenation as expected form operator "="
 * @param m given matrix to copy values from
 * @return reference to this matrix after all m matrix's values were copied to it
 */
Matrix &Matrix::operator=(const Matrix &m)
{
    if (&m == this)
    {
        return *this;
    }
    this->matrixDims.rows = m.getRows();
    this->matrixDims.cols = m.getCols();
    delete[] matrix;
    this->matrix = new(std::nothrow) float[m.getRows() * m.getCols()];
    if (matrix == nullptr)
    {
        std::cerr << "Error: couldn't allocate needed memory" << std::endl;
        exit(EXIT_CODE);
    }
    for (int i = 0; i < this->getRows() * this->getCols(); i++)
    {
        this->matrix[i] = m.matrix[i];
    }
    return *this;
}

/**
 * overloading operator "*" for matrix object : function multiply this matrix's values with another given matrix's
 * values and returns a new matrix containing the multiplied values (returns a new matrix made form matrices
 * multiplication of this matrix and another given matrix).
 * @param b given matrix to multiply with this matrix.
 * @return new matrix made form multiplication of this matrix and b given matrix
 */
Matrix Matrix::operator*(const Matrix &b) const
{
    if (this->matrixDims.cols != b.matrixDims.rows)
    {
        std::cerr << "Error: operator ""*"" cannot multiply matrices - unsuited number of rows or cols " << std::endl;
        exit(EXIT_CODE);
    }
    int matrixElemIndex = 0;
    Matrix matrixToReturn(this->getRows(), b.getCols());
    for (int i = 0; i < this->matrixDims.rows; i++)
    {
        for (int j = 0; j < b.matrixDims.cols; j++)
        {
            float matrixElement = 0;
            for (int k = 0; k < this->matrixDims.cols; k++)
            {
                matrixElement += this->matrix[i * this->matrixDims.cols + k] * b.matrix[j + k * b.matrixDims.cols];
            }
            matrixToReturn.matrix[matrixElemIndex] = matrixElement;
            matrixElemIndex += 1;
        }
    }
    return matrixToReturn;
}

/**
 * overloading operator "*" for matrix object: matrix's scalar multiplication from the left
 * @param scalar float to multiply matrix's values with
 * @param matrixToMultiply given matrix to multiply
 * @return new matrix made from scalar multiplication of this matrix and scalar
 */
Matrix operator*(const float scalar, const Matrix &matrixToMultiply)
{
    Matrix matrixToReturn(matrixToMultiply.getRows(), matrixToMultiply.getCols());
    for (int i = 0; i < matrixToMultiply.getRows(); i++)
    {
        for (int j = 0; j < matrixToMultiply.getCols(); j++)
        {
            matrixToReturn.matrix[i * matrixToMultiply.getCols() + j] = scalar *
                                                                        matrixToMultiply.matrix[
                                                                                i * matrixToMultiply.getCols() + j];
        }
    }
    return matrixToReturn;
}

/**
 * overloading operator "*" for matrix object: matrix's scalar multiplication from the right
 * @param scalar float to multiply matrix's values with
 * @param matrixToMultiply given matrix to multiply
 * @return new matrix made from scalar multiplication of this matrix and scalar
 */
Matrix Matrix::operator*(const float scalar) const
{
    return (scalar) * (*this);
}

/**
 * overloading operator "+" for matrix object: addition of this matrix and another given matrix
 * @param m given matrix to add it's values to this matrix's values
 * @return new matrix whose values are made from addition of this matrix's values and another given matrix's values (m)
 */
Matrix Matrix::operator+(const Matrix &m) const
{
    if (this->matrixDims.cols != m.matrixDims.cols || this->matrixDims.rows != m.matrixDims.rows)
    {
        std::cerr << "Error: operator ""+"" cannot add matrices with different number of rows or cols " << std::endl;
        exit(EXIT_CODE);
    }
    Matrix matrixToReturn(this->getRows(), this->getCols());
    for (int i = 0; i < this->getRows(); i++)
    {
        for (int j = 0; j < this->getCols(); j++)
        {
            matrixToReturn.matrix[i * this->getCols() + j] = this->matrix[i * this->getCols() + j] +
                                                             m.matrix[i * m.getCols() + j];
        }
    }
    return matrixToReturn;
}

/**
 * overloading operator "+=" for matrix object: addition of this matrix and another given matrix and assignment
 * the result matrix to this matrix - function returns a reference to this matrix in order to enable concatenation
 * as expected from operator "+=".
 * @param m given matrix to add it's values to this matrix's values
 * @return reference to this matrix after the given matrix's (m) values were add to it's values
 * (after matrixes addition)
 */
Matrix &Matrix::operator+=(const Matrix &m)
{
    *this = (*this + m);
    return *this;
}

/**
 * overloading operator "()" for matrix object: returns reference to the matrix's value at index (i,j) in order to
 * enable change the value
 * @param i row index
 * @param j column index
 * @return reference to matrix's value at index (i,j) in order to enable change the value
 */
float &Matrix::operator()(const int i, const int j)
{
    if (i < 0 || j < 0 || i >= this->getRows() || j >= this->getCols())
    {
        std::cerr << "Error: operator ""()"" cannot return a matrix element with negative given index" << std::endl;
        exit(EXIT_CODE);
    }
    int suitedIndex = i * this->matrixDims.cols + j;
    return this->matrix[suitedIndex];
}

/**
 * overloading operator "()" for matrix object: returns the matrix's value at index (i,j) as const in order to
 * get the value without ant intention to change it
 * @param i row index
 * @param j column index
 * @return returns the matrix's value at index (i,j) as const in order to get the value without any intention to
 * change it.
 */
float Matrix::operator()(const int i, const int j) const
{
    if (i < 0 || j < 0 || i >= this->getRows() || j >= this->getCols())
    {
        std::cerr << "Error: operator ""()"" cannot return a matrix element with negative given index" << std::endl;
        exit(EXIT_CODE);
    }
    int suitedIndex = i * this->matrixDims.cols + j;
    return this->matrix[suitedIndex];
}

/**
 * overloading operator "[]" for matrix object: returns reference to the matrix's value at index i in order to
 * enable change the value
 * @param i value index in matrix's data
 * @return reference to matrix's value at index i in order to enable change the value
 */
float &Matrix::operator[](const int i)
{
    if (i < 0 || i >= this->matrixDims.rows * this->matrixDims.cols)
    {
        std::cerr << "Error: operator ""[]"" cannot return a matrix value with unsuitable given index" << std::endl;
        exit(EXIT_CODE);
    }
    return this->matrix[i];
}

/**
 * overloading operator "[]" for matrix object: returns the matrix's value at index i as const in order to
 * get the value without ant intention to change it
 * @param i value index in matrix's data
 * @return returns the matrix's value at index i as const in order to get the value without any intention to
 * change it.
 */
float Matrix::operator[](const int i) const
{
    if (i < 0 || i >= this->matrixDims.rows * this->matrixDims.cols)
    {
        std::cerr << "Error: operator ""[]"" cannot return a matrix value with  unsuitable given index" << std::endl;
        exit(EXIT_CODE);
    }
    return this->matrix[i];
}

/**
 * overloading operator ">>" for matrix object: reads values from istream object and puts them in a suitable index
 * of given matrix
 * @param istream istream object to read values from
 * @param m given matrix to fill with values red from istream object
 * @return reference to istream object we red values from
 */
std::istream &operator>>(std::istream &istream, Matrix &m)
{
    for (int i = 0; i < m.getRows() && istream.good(); i++)
    {
        for (int j = 0; j < m.getCols() && istream.good(); j++)
        {
            float num = 0;
            istream.read((char *) &num, sizeof(float));
            m.matrix[i * m.getCols() + j] = num;
        }
    }
    std::string str;
    istream.read((char *) &str, sizeof(float));
    if (istream.eof())
    {
        return istream;
    }
    else
    {
        std::cerr << "Error: operator "">>"" failed reading the whole file" << std::endl;
        exit(EXIT_CODE);
    }
}

/**
 * overloading operator "<<" for matrix object: ostream object contains pretty export of the matrix as described
 * in the exercise guidelines
 * @param ostream object to fill with pretty export of the matrix as described in the exercise guideline
 * @param m given matrix to output
 * @return refernece to ostream object contains pretty export of the matrix as described in the exercise guidelines
 */
std::ostream &operator<<(std::ostream &ostream, Matrix &m)
{
    for (int i = 0; i < m.matrixDims.rows; i++)
    {
        for (int j = 0; j < m.matrixDims.cols; j++)
        {
            if (m(i, j) <= 0.1f)
            {
                ostream << "  ";
            }
            else
            {
                ostream << "**";
            }
        }
        ostream << std::endl;
    }
    return ostream;
}
