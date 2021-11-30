/**
 * @file Fractal.cpp
 * @author  Omer Salman <omer.salman@mail.huji.ac.il>
 * @date 8 Jan 2020
 *
 * @brief .cpp file for fractal, SierpinskiCarpet, SierpinskiTriangle, VicsekFractal and FractalFactory classes.
 */

// ------------------------------ includes ------------------------------

#include "Fractal.h"
#include <iostream>

// ------------------------------ constructors -----------------------------

/**
 * constructor for SierpinskiCarpet fractal object.
 * @param fractalLevel int presents the fractal's level to be built and printed.
 */
SierpinskiCarpet::SierpinskiCarpet(const int &fractalLevel) : Fractal(fractalLevel, 3, 3)
{
    this->initializeFractalData();
}

/**
 * constructor for Sierpinski triangle fractal object.
 * @param fractalLevel int presents the fractal's level to be built and printed.
 */
SierpinskiTriangle::SierpinskiTriangle(const int &fractalLevel) : Fractal(fractalLevel, 2, 2)
{
    this->initializeFractalData();
}

/**
 * constructor for Vinsek fractal object.
 * @param fractalLevel int presents the fractal's level to be built and printed.
 */
VicsekFractal::VicsekFractal(const int &fractalLevel) : Fractal(fractalLevel, 3, 3)
{
    this->initializeFractalData();
}

// ------------------------------ functions -----------------------------

/**
 * function prints fractal's grid.
 */
void Fractal::printFractal() const
{
    for (int i = 0; i < this->_fractalDim; i++)
    {
        for (int j = 0; j < this->_fractalDim; j++)
        {
            std::cout << this->_fractalOutputGrid[i][j];
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

/**
* override pure virtual function that gets two coordinates in the fractal's grid and a divisor and returns true if the
* suited square for this coordinates in the fractal's grid need to be filled with the fractal's printing char or
* it should stay enpty (" "), this function used for filling the fractal's grid.
* @param x int presents the x coordinate of the current square in the fractal's grid.
* @param y int presents the y coordinate of the current square in the fractal's grid.
* @param divisor int presents the divisor for deciding if the current square should be filled or not.
* @return true if the current square in the fractal's grind need to be filled with the fractal's printing char,
* false if not - it should stay empty.
*/
bool SierpinskiCarpet::_coordinateFillingCheckFunc(int x, int y, const int divisor) const
{
    while (x > 0 || y > 0)
    {
        if (x % divisor == 1 && y % divisor == 1)
        {
            return false;
        }
        x /= divisor;
        y /= divisor;
    }
    return true;
}

/**
* override pure virtual function that gets two coordinates in the fractal's grid and a divisor and returns true if the
* suited square for this coordinates in the fractal's grid need to be filled with the fractal's printing char or
* it should stay enpty (" "), this function used for filling the fractal's grid.
* @param x int presents the x coordinate of the current square in the fractal's grid.
* @param y int presents the y coordinate of the current square in the fractal's grid.
* @param divisor int presents the divisor for deciding if the current square should be filled or not.
* @return true if the current square in the fractal's grind need to be filled with the fractal's printing char,
* false if not - it should stay empty.
*/
bool SierpinskiTriangle::_coordinateFillingCheckFunc(int x, int y, const int divisor) const
{
    while (x > 0 || y > 0)
    {
        if (x % divisor == 1 && y % divisor == 1)
        {
            return false;
        }
        x /= divisor;
        y /= divisor;
    }
    return true;
}

/**
* override pure virtual function that gets two coordinates in the fractal's grid and a divisor and returns true if the
* suited square for this coordinates in the fractal's grid need to be filled with the fractal's printing char or
* it should stay enpty (" "), this function used for filling the fractal's grid.
* @param x int presents the x coordinate of the current square in the fractal's grid.
* @param y int presents the y coordinate of the current square in the fractal's grid.
* @param divisor int presents the divisor for deciding if the current square should be filled or not.
* @return true if the current square in the fractal's grind need to be filled with the fractal's printing char,
* false if not - it should stay empty.
*/
bool VicsekFractal::_coordinateFillingCheckFunc(int x, int y, const int divisor) const
{
    while (x > 0 || y > 0)
    {
        if ((x % divisor == 1) ^ (y % divisor == 1))
        {
            return false;
        }
        x /= divisor;
        y /= divisor;
    }
    return true;
}

/**
 * function fills the fractal's grid with the fractal's printing char.
 */
void Fractal::_fillFractalOutputGrid()
{
    for (int i = 0; i < this->_fractalDim; i++)
    {
        for (int j = 0; j < this->_fractalDim; j++)
        {
            if (this->_coordinateFillingCheckFunc(i, j, this->_fillingConditionDivisor))
            {
                (this->_fractalOutputGrid[i])[j] = this->_printingChar;
            }
        }
    }
}

/**
 * function initialize fractal's data (private fields).
 */
void Fractal::initializeFractalData()
{
    std::vector<char> fractalGridLine(this->_fractalDim, ' ');
    this->_fractalOutputGrid = std::vector<std::vector<char>>(this->_fractalDim, fractalGridLine);
    this->_fillFractalOutputGrid();

}

/**
 * static function to print suited fractals as their type and level described in two suited vectors.
 * @param fractalTypeNum vector of ints presents each fractal type number.
 * @param fractalLevelNum vector of ints presents each fractal level number.
 */
void FractalFactory::printSuitedFractal(const int &fractalTypeNum, const int &fractalLevelNum)
{
    switch (fractalTypeNum)
    {
        case 1:
        {
            SierpinskiCarpet sierpinskiCarpet(fractalLevelNum);
            sierpinskiCarpet.printFractal();
            break;
        }
        case 2:
        {
            SierpinskiTriangle sierpinskiTriangle(fractalLevelNum);
            sierpinskiTriangle.printFractal();
            break;
        }
        case 3:
        {
            VicsekFractal vicsekFractal(fractalLevelNum);
            vicsekFractal.printFractal();
            break;
        }
        default:
        {
        }
    }
}