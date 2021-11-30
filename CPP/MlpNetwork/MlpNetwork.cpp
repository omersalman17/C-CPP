//
// Created by omer on 12/23/2019.
//

/**
 * @file MlpNetwork.cpp
 * @author  Omer Salman <omer.salman@mail.huji.ac.il>
 * @date 26 Dec 2019
 *
 * @brief MlpNetwork object class
 */

// ------------------------------ includes ------------------------------

#include "MlpNetwork.h"

// ------------------------------ constructors -----------------------------

/**
 * constructor for MlpNetwork object : constructs an MlpNetwork object from a given weight representing matrices
 * and bias representing matrices (actually vectors).
 * @param weights array of matrices representing weights
 * @param biases array of matrices representing biases
 */
MlpNetwork::MlpNetwork(Matrix weights[], Matrix biases[]) :
        dense0(weights[0], biases[0], Relu), dense1(weights[1], biases[1], Relu),
        dense2(weights[2], biases[2], Relu), dense3(weights[3], biases[3], Softmax)
{
    for (int i = 0; i < MLP_SIZE; i++)
    {
        if (weights[i].getRows() != weightsDims[i].rows || weights[i].getCols() != weightsDims[i].cols)
        {
            std::cerr << "Error: weights matrix has invalid rows or cols number" << std::endl;
            exit(1);
        }
        if (biases[i].getRows() != biasDims[i].rows || biases[i].getCols() != biasDims[i].cols)
        {
            std::cerr << "Error: biases vector has invalid rows or cols number" << std::endl;
            exit(1);
        }
    }
}

// ------------------------------ public functions - part of the API -----------------------------

/**
 * overloading operator "()" for MlpNetwork object: returns a Digit object presents what digit is described on given
 * matrix presenting an image and at what probability.
 * @param img given matrix presents an image describing a digit
 * @return Digit object presents what digit is described on given matrix presenting an image and at what probability.
 */
Digit MlpNetwork::operator()(const Matrix &img) const
{
    Matrix r1 = dense0(img);
    r1 = dense1(r1);
    r1 = dense2(r1);
    r1 = dense3(r1);
    unsigned int digit = 0;
    float prob = 0;
    for (int i = 0; i < 10; i++)
    {
        if (r1[i] > prob)
        {
            digit = i;
            prob = r1[i];
        }
    }
    Digit digitObjToReturn = {digit, prob};
    return digitObjToReturn;
}