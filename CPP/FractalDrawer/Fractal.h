/**
 * @file Fractal.h
 * @author  Omer Salman <omer.salman@mail.huji.ac.il>
 * @date 8 Jan 2020
 *
 * @brief header file for fractal, SierpinskiCarpet, SierpinskiTriangle, VicsekFractal and FractalFactory classes.
 */

#ifndef CPP_EX2_FRACTAL_H
#define CPP_EX2_FRACTAL_H

// ------------------------------ includes ------------------------------

#include <vector>
#include <cmath>

// ------------------------------ classes ------------------------------

/**
 * class of Fractal object.
 */
class Fractal
{
private:
    char _printingChar = '#';
    int _fractalLevel;
    int _fractalBaseDim; // size of rows and cols of level 1 output board (square matrix)
    int _fillingConditionDivisor;
    int _fractalDim;
    std::vector<std::vector<char>> _fractalOutputGrid;

    /**
     * pure virtual function that gets two coordinates in the fractal's grid and a divisor and returns true if the
     * suited square for this coordinates in the fractal's grid need to be filled with the fractal's printing char or
     * it should stay enpty (" "), this function used for filling the fractal's grid.
     * @param x int presents the x coordinate of the current square in the fractal's grid.
     * @param y int presents the y coordinate of the current square in the fractal's grid.
     * @param divisor int presents the divisor for deciding if the current square should be filled or not.
     * @return true if the current square in the fractal's grind need to be filled with the fractal's printing char,
     * false if not - it should stay empty.
     */
    virtual bool _coordinateFillingCheckFunc(int x, int y, int divisor) const = 0;

    /**
     * function fills the fractal's grid with the fractal's printing char.
     */
    void _fillFractalOutputGrid();

protected:
    /**
     * function initialize fractal's data (private fields).
     */
    void initializeFractalData();

public:
    /**
     * constructor for Fractal object.
     * @param fractalLevel int presents the fractal's level to be built and printed.
     * @param fractalBaseDim int presents the fractal's level 1 gird dimension (square matrix).
     * @param fillingConditionDivisor int presents the divisor for filling condition of the fractal's grid.
     */
    Fractal(const int &fractalLevel, const int &fractalBaseDim, const int &fillingConditionDivisor) : _fractalLevel(
            fractalLevel),
            _fractalBaseDim(fractalBaseDim), _fillingConditionDivisor(fillingConditionDivisor),
            _fractalDim((int) pow(this->_fractalBaseDim, this->_fractalLevel))
    {
    }

    /**
     * virtual destructor for fractal objects as needed due to inheritance and polymorphism.
     */
    virtual ~Fractal() = default;

    /**
     * function prints fractal's grid.
     */
    void printFractal() const;
};


/**
 * class of Sierpinski carpet fractal object.
 */
class SierpinskiCarpet : public Fractal
{
private:
    /**
    * override pure virtual function that gets two coordinates in the fractal's grid and a divisor and returns true if
    * suited square for this coordinates in the fractal's grid need to be filled with the fractal's printing char or
    * it should stay enpty (" "), this function used for filling the fractal's grid.
    * @param x int presents the x coordinate of the current square in the fractal's grid.
    * @param y int presents the y coordinate of the current square in the fractal's grid.
    * @param divisor int presents the divisor for deciding if the current square should be filled or not.
    * @return true if the current square in the fractal's grind need to be filled with the fractal's printing char,
    * false if not - it should stay empty.
    */
    bool _coordinateFillingCheckFunc(int x, int y, int divisor) const override;

public:
    /**
     * constructor for SierpinskiCarpet fractal object
     * @param fractalLevel int presents the fractal's level to be built and printed
     */
    explicit SierpinskiCarpet(const int &fractalLevel);
};


/**
 * class of Sierpinski triangle fractal object.
 */
class SierpinskiTriangle : public Fractal
{
private:
    /**
    * override pure virtual function that gets two coordinates in the fractal's grid and a divisor and returns true if
    * suited square for this coordinates in the fractal's grid need to be filled with the fractal's printing char or
    * it should stay enpty (" "), this function used for filling the fractal's grid.
    * @param x int presents the x coordinate of the current square in the fractal's grid.
    * @param y int presents the y coordinate of the current square in the fractal's grid.
    * @param divisor int presents the divisor for deciding if the current square should be filled or not.
    * @return true if the current square in the fractal's grind need to be filled with the fractal's printing char,
    * false if not - it should stay empty.
    */
    bool _coordinateFillingCheckFunc(int x, int y, int divisor) const override;

public:
    /**
     * constructor for Sierpinski triangle fractal object
     * @param fractalLevel int presents the fractal's level to be built and printed
     */
    explicit SierpinskiTriangle(const int &fractalLevel);
};


/**
 * class of Vicsek fractal object.
 */
class VicsekFractal : public Fractal
{
private:
    /**
    * override pure virtual function that gets two coordinates in the fractal's grid and a divisor and returns true if
    * suited square for this coordinates in the fractal's grid need to be filled with the fractal's printing char or
    * it should stay enpty (" "), this function used for filling the fractal's grid.
    * @param x int presents the x coordinate of the current square in the fractal's grid.
    * @param y int presents the y coordinate of the current square in the fractal's grid.
    * @param divisor int presents the divisor for deciding if the current square should be filled or not.
    * @return true if the current square in the fractal's grind need to be filled with the fractal's printing char,
    * false if not - it should stay empty.
    */
    bool _coordinateFillingCheckFunc(int x, int y, int divisor) const override;

public:
    /**
     * constructor for Vinsek fractal object.
     * @param fractalLevel int presents the fractal's level to be built and printed.
     */
    explicit VicsekFractal(const int &fractalLevel);
};


/**
 * class of fractal factory object.
 */
class FractalFactory
{
private:
    FractalFactory() = default; // constructor declared default because there is no need to enable the user to create
    // one - printing fractals is made through public static function below.
public:
    /**
     * static function to print suited fractals as their type and level described in two suited vectors.
     * @param fractalTypeNum vector of ints presents each fractal type number.
     * @param fractalLevelNum vector of ints presents each fractal level number.
     */
    static void printSuitedFractal(const int &fractalTypeNum, const int &fractalLevelNum);
};

#endif //CPP_EX2_FRACTAL_H
