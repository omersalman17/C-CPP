//
// Created by omer on 12/23/2019.
//
/**
 * @file Dense.h
 * @author  Omer Salman <omer.salman@mail.huji.ac.il>
 * @date 26 Dec 2019
 *
 * @brief Dense object class
 */
#ifndef CPP_EX1_DENSE_H
#define CPP_EX1_DENSE_H

#include "Matrix.h"
#include "Activation.h"

/**
 * class of Dense object
 */
class Dense
{
private:
    Matrix w, bias;
    const Activation activation;

public:
    /**
     * constructor for Dense object: constructs a Dense object from given two matrices and an Activation object
     * @param w given matrix represents weights
     * @param bias given matrix (actually a vector) represents bias
     * @param actType ActivationType for Dense's Activation
     */
    Dense(const Matrix &w, const Matrix &bias, ActivationType actType);

    /**
     * getter for Dense's object weights matrix
     * @return Dense's object weights matrix
     */
    const Matrix &getWeights() const;

    /**
     * getter for Dense's object bias matrix (actually a vector)
     * @return Dense's object bias matrix (actually a vector)
     */
    const Matrix &getBias() const;

    /**
     * getter for Dense's object Activation object
     * @return Dense's object Activation object
     */
    const Activation &getActivation() const;

    /**
     * overloading operator "()" for Dense object: returns a new const Matrix made from a given Matrix after the Dense
     * calculation (according to the exercise guidelines) and it's activation function was made on it.
     * @param m given matrix
     * @return new const Matrix made from a given Matrix after the Dense calculation (according to the exercise
     * guidelines) and it's activation function was made on it.
     */
    Matrix operator()(const Matrix &m) const;
};


#endif //CPP_EX1_DENSE_H
