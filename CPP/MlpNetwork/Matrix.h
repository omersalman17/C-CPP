// Matrix.h
/**
 * @file Matrix.h
 * @author  Omer Salman <omer.salman@mail.huji.ac.il>
 * @date 26 Dec 2019
 *
 * @brief Matrix object class
 */
#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>

/**
 * @struct MatrixDims
 * @brief Matrix dimensions container
 */
typedef struct MatrixDims
{
    int rows, cols;
} MatrixDims;

/**
 * class of Matrix object
 */
class Matrix
{
private:
    float *matrix;
    MatrixDims matrixDims;

public:
    /**
     * constructor for matrix object with given number of rows and columns
     * @param rows matrix's number of rows
     * @param cols matrix's number of columns
     */
    Matrix(int rows, int cols);

    /**
     * default constructor for matrix objects, constructs a matrix with 1 row and 1 column
     */
    Matrix();

    /**
     * copy constructor for matrix object
     * @param m matrix to construct a new matrix from with identical values
     */
    Matrix(const Matrix &m);

    /**
     * destructor for matrix object
     */
    ~Matrix();

    /**
     * getter for matrix's number of rows
     * @return matrix's number of rows
     */
    int getRows() const;

    /**
     * getter for matrix's number of columns
     * @return matrix's number of columns
     */
    int getCols() const;

    /**
     * function turns the matrix object to vector (1 column) without loosing any of the matrix's values
     * @return
     */
    Matrix &vectorize();

    /**
     * plain prints the matrix's values
     */
    void plainPrint() const;

    /**
     * overloading operator "=" for matrix objects : function copy given matrix's values to this matrix and returns a
     * reference to this matrix in order to enable concatenation as expected form operator "="
     * @param m given matrix to copy values from
     * @return reference to this matrix after all m matrix's values were copied to it
     */
    Matrix &operator=(const Matrix &m);

    /**
     * overloading operator "*" for matrix object : function multiply this matrix's values with another given matrix's
     * values and returns a new matrix containing the multiplied values (returns a new matrix made form matrices
     * multiplication of this matrix and another given matrix).
     * @param b given matrix to multiply with this matrix.
     * @return new matrix made form multiplication of this matrix and b given matrix
     */
    Matrix operator*(const Matrix &m) const;

    /**
     * overloading operator "*" for matrix object: matrix's scalar multiplication from the right
     * @param scalar float to multiply matrix's values with
     * @param matrixToMultiply given matrix to multiply
     * @return new matrix made from scalar multiplication of this matrix and scalar
     */
    Matrix operator*(float scalar) const;

    /**
     * overloading operator "*" for matrix object: matrix's scalar multiplication from the left
     * @param scalar float to multiply matrix's values with
     * @param matrixToMultiply given matrix to multiply
     * @return new matrix made from scalar multiplication of this matrix and scalar
     */
    friend Matrix operator*(float scalar, const Matrix &matrixToMultiply);

    /**
     * overloading operator "+" for matrix object: addition of this matrix and another given matrix
     * @param m given matrix to add it's values to this matrix's values
     * @return new matrix whose values are made from addition of this matrix's values and another given matrix's values
     */
    Matrix operator+(const Matrix &m) const;

    /**
     * overloading operator "+=" for matrix object: addition of this matrix and another given matrix and assignment
     * the result matrix to this matrix - function returns a reference to this matrix in order to enable concatenation
     * as expected from operator "+=".
     * @param m given matrix to add it's values to this matrix's values
     * @return reference to this matrix after the given matrix's (m) values were add to it's values
     * (after matrixes addition)
     */
    Matrix &operator+=(const Matrix &m);

    /**
     * overloading operator "()" for matrix object: returns reference to the matrix's value at index (i,j) in order to
     * enable change the value
     * @param i row index
     * @param j column index
     * @return reference to matrix's value at index (i,j) in order to enable change the value
     */
    float &operator()(int i, int j);

    /**
     * overloading operator "()" for matrix object: returns the matrix's value at index (i,j) as const in order to
     * get the value without ant intention to change it
     * @param i row index
     * @param j column index
     * @return returns the matrix's value at index (i,j) as const in order to get the value without any intention to
     * change it.
     */
    float operator()(int i, int j) const;

    /**
     * overloading operator "[]" for matrix object: returns reference to the matrix's value at index i in order to
     * enable change the value
     * @param i value index in matrix's data
     * @return reference to matrix's value at index i in order to enable change the value
     */
    float &operator[](int i);

    /**
     * overloading operator "[]" for matrix object: returns the matrix's value at index i as const in order to
     * get the value without ant intention to change it
     * @param i value index in matrix's data
     * @return returns the matrix's value at index i as const in order to get the value without any intention to
     * change it.
     */
    float operator[](int i) const;

    /**
     * overloading operator ">>" for matrix object: reads values from istream object and puts them in a suitable
     * index of given matrix
     * @param istream istream object to read values from
     * @param m given matrix to fill with values red from istream object
     * @return reference to istream object we red values from
     */
    friend std::istream &operator>>(std::istream &is, Matrix &m);

    /**
     * overloading operator "<<" for matrix object: ostream object contains pretty export of the matrix as described
     * in the exercise guidelines
     * @param ostream object to fill with pretty export of the matrix as described in the exercise guideline
     * @param m given matrix to output
     * @return refernece to ostream object contains pretty export of the matrix as described in the exercise guidelines
     */
    friend std::ostream &operator<<(std::ostream &os, Matrix &m);
};

#endif //MATRIX_H
