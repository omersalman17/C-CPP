//Activation.h
/**
 * @file Activation.h
 * @author  Omer Salman <omer.salman@mail.huji.ac.il>
 * @date 26 Dec 2019
 *
 * @brief Activation object class
 */

#ifndef ACTIVATION_H
#define ACTIVATION_H

#include "Matrix.h"

/**
 * @enum ActivationType
 * @brief Indicator of activation function.
 */
enum ActivationType
{
    Relu,
    Softmax
};

/**
 * class of Activation object
 */
class Activation
{
private:
    ActivationType activationType;
public:
    /**
    * constructor for Activation object -  constructs an Activation object with given ActivationType
    * @param actType given ActivationType to constructs Activation object with
    */
    Activation(ActivationType actType);


    /**
     * getter for this Activation ActivationType
     * @return this Activation ActivationType
     */
    ActivationType getActivationType();

    /**
     * overloading operator "()" for Activation object: returns a new Matrix (actually a vector) made form given Matrix
     * after it's ActivationType function was activated on it.
     * @param m given matrix
     * @return new Matrix (actually a vector) made form given Matrix
     * after it's ActivationType function was activated on it.
     */
    Matrix operator()(const Matrix &m) const;
};

#endif //ACTIVATION_H
