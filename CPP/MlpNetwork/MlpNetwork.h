//MlpNetwork.h
/**
 * @file MlpNetwork.h
 * @author  Omer Salman <omer.salman@mail.huji.ac.il>
 * @date 26 Dec 2019
 *
 * @brief MlpNetwork object class
 */

#ifndef MLPNETWORK_H
#define MLPNETWORK_H

#include "Matrix.h"
#include "Digit.h"
#include "Dense.h"

#define MLP_SIZE 4

const MatrixDims imgDims = {28, 28};
const MatrixDims weightsDims[] = {{128, 784},
                                  {64,  128},
                                  {20,  64},
                                  {10,  20}};
const MatrixDims biasDims[] = {{128, 1},
                               {64,  1},
                               {20,  1},
                               {10,  1}};

/**
 * class of MlpNetwork object
 */
class MlpNetwork
{
private:
    Dense dense0;
    Dense dense1;
    Dense dense2;
    Dense dense3;
public:
    /**
     * constructor for MlpNetwork object : constructs an MlpNetwork object from a given weight representing matrices
     * and bias representing matrices (actually vectors).
     * @param weights array of matrices representing weights
     * @param biases array of matrices representing biases
     */
    MlpNetwork(Matrix weights[], Matrix biases[]);

    /**
     * overloading operator "()" for MlpNetwork object: returns a Digit object presents what digit is described on
     * given matrix presenting an image and at what probability.
     * @param img given matrix presents an image describing a digit
     * @return Digit object presents what digit is described on given matrix presenting an image and at what
     * probability.
     */
    Digit operator()(const Matrix &img) const;
};

#endif // MLPNETWORK_H
