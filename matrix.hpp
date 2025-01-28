#pragma once

#include <stdlib.h>
#include <memory>



class Matrix {
public:
    int n, m;
    std::unique_ptr<double[]> data;

    Matrix();
    Matrix(int n, int m);
    Matrix(int n, int m, unsigned char *vals);


    double getElementAt(int i, int j) const;
    void setElementAt(int i, int j, double newVal);
    void setAllElementsTo(double val);
    void print() const;
    void fillRandom(double max);
    void deepCopy(Matrix& dest) const;
    void applyFunction(double func(double)) const;
    void adjustRandomSlightly(double relChange);
    double getVectorNorm() const;

    double& operator[](size_t index);
    friend Matrix operator+(Matrix const& l, Matrix const& r);
    friend Matrix operator*(Matrix const& l, Matrix const& r);
    
};

