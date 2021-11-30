//
// Created by omer on 12/23/2019.
//

/**
 * @file Activation.cpp
 * @author  Omer Salman <omer.salman@mail.huji.ac.il>
 * @date 26 Dec 2019
 *
 * @brief Activation object class
 */

// ------------------------------ includes ------------------------------

#include "Activation.h"
#include <cmath>

// ------------------------------ constructors -----------------------------

/**
 * constructor for Activation object -  constructs an Activation object with given ActivationType
 * @param actType given ActivationType to constructs Activation object with
 */
Activation::Activation(ActivationType actType) : activationType(actType)
{
}


// ------------------------------ private functions - not part of the API -----------------------------

/**
 * Relu function to activate on given vector
 * @param vectorToReturn reference to given vector to activate Relu function on return reference to it after
 * activation was made.
 * @return  reference to given vector after Relu function was activated on it
 */
Matrix &relu(Matrix &vectorToReturn)
{
    if (vectorToReturn.getCols() != 1)
    {
        std::cerr << "Error: Relu function can only be applied on vectors" << std::endl;
        exit(1);
    }
    for (int i = 0; i < vectorToReturn.getRows(); i++)
    {
        if (vectorToReturn[i] < 0)
        {
            vectorToReturn[i] = 0;
        }
    }
    return vectorToReturn;
}

/**
 * Softmax function to activate on given vector
 * @param vectorToReturn reference to given vector to activate Softmax function on return reference to it after
 * activation was made.
 * @return  reference to given vector after Softmax function was activated on it
 */
Matrix &softmax(Matrix &vectorToReturn)
{
    if (vectorToReturn.getCols() != 1)
    {
        std::cerr << "Error: Softmax function can only be applied on vectors" << std::endl;
        exit(1);
    }
    float eSum = 0;
    for (int i = 0; i < vectorToReturn.getRows(); i++)
    {
        eSum += std::exp(vectorToReturn[i]);
    }
    for (int i = 0; i < vectorToReturn.getRows(); i++)
    {
        vectorToReturn[i] = (1 / eSum) * std::exp(vectorToReturn[i]);
    }
    return vectorToReturn;
}

// ------------------------------ public functions - part of the API -----------------------------

/**
 * getter for this Activation ActivationType
 * @return this Activation ActivationType
 */
ActivationType Activation::getActivationType()
{
    return this->activationType;
}

/**
 * overloading operator "()" for Activation object: returns a new Matrix (actually a vector) made form given Matrix
 * after it's ActivationType function was activated on it.
 * @param m given matrix
 * @return new Matrix (actually a vector) made form given Matrix
 * after it's ActivationType function was activated on it.
 */
Matrix Activation::operator()(const Matrix &m) const
{
    Matrix vectorToReturn(m);
    if (this->activationType == Relu)
    {
        return relu(vectorToReturn);
    }
    else
    {
        return softmax(vectorToReturn);
    }
}

