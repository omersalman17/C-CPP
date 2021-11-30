//
// Created by omer on 12/23/2019.
//

/**
 * @file Dense.cpp
 * @author  Omer Salman <omer.salman@mail.huji.ac.il>
 * @date 26 Dec 2019
 *
 * @brief Dense object class
 */

// ------------------------------ includes ------------------------------

#include "Dense.h"

// ------------------------------ constructors -----------------------------

/**
 * constructor for Dense object: constructs a Dense object from given two matrices and an Activation object
 * @param w given matrix represents weights
 * @param bias given matrix (actually a vector) represents bias
 * @param actType ActivationType for Dense's Activation
 */
Dense::Dense(const Matrix &w, const Matrix &bias, ActivationType actType) : w(w), bias(bias), activation(actType)
{
}


// ------------------------------ public functions - part of the API -----------------------------

/**
 * getter for Dense's object weights matrix
 * @return Dense's object weights matrix
 */
const Matrix &Dense::getWeights() const
{
    return this->w;
}

/**
 * getter for Dense's object bias matrix (actually a vector)
 * @return Dense's object bias matrix (actually a vector)
 */
const Matrix &Dense::getBias() const
{
    return this->bias;
}

/**
 * getter for Dense's object Activation object
 * @return Dense's object Activation object
 */
const Activation &Dense::getActivation() const
{
    return this->activation;
}

/**
 * overloading operator "()" for Dense object: returns a new const Matrix made from a given Matrix after the Dense
 * calculation (according to the exercise guidelines) and it's activation function was made on it.
 * @param m given matrix
 * @return new const Matrix made from a given Matrix after the Dense calculation (according to the exercise guidelines)
 * and it's activation function was made on it.
 */
Matrix Dense::operator()(const Matrix &m) const
{
    const Matrix matrixToReturn = this->w * m + this->bias;
    return this->activation(matrixToReturn);
}

