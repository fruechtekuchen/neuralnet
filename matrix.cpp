#include "matrix.hpp"

#include <stdlib.h>
#include <memory>
#include <iostream>
#include <random>
#include <ctime>
#include <cstring>
#include <cassert>

Matrix::Matrix() {
    n = 0;
    m = 0;
    data = std::make_unique<double[]>(0);
}

Matrix::Matrix(int n, int m) {
    this->n = n;
    this->m = m;
    this->data = std::make_unique<double[]>(n*m);
}

double Matrix::getElementAt(int i, int j) const {
    return this->data[i*m+j];
}

void Matrix::setElementAt(int i, int j, double newVal) {
    data[i*m+j] = newVal;
}

void Matrix::print() const {
    for(int i=0; i<n; i++) {
        for(int j=0; j<m; j++) {
            std::cout << data[i*m+j] << ", ";
        }
        std::cout << "\n";
    }
}

void Matrix::fillRandom(double max) {
    std::srand(std::time(0));
    static std::uniform_real_distribution<double> unif(-max, max);
    static std::default_random_engine re;
 
    for(int i=0; i<n; i++) {
        for(int j=0; j<m; j++) {
            data[i*m+j] = unif(re);
        }
    }
}

void Matrix::deepCopy(Matrix& dest) const {
    if(dest.n == n || dest.m == m) {
        std::memcpy(dest.data.get(), data.get(), n*m*sizeof(double));
        return;
    }
    dest.n = n;
    dest.m = m;
    dest.data = std::make_unique<double[]>(n*m);
    std::memcpy(dest.data.get(), data.get(), n*m*sizeof(double));
}

void Matrix::applyFunction(double func(double)) const {
    for(int i=0; i<n; i++) {
        for(int j=0; j<m; j++) {
            data[i*m+j] = func(data[i*m+j]);
        }
    }
}

void Matrix::adjustSlightly(double relChange) {
    std::srand(std::time(0));
    static std::uniform_real_distribution<double> unif(1.-relChange, 1.+relChange);
    static std::default_random_engine re;
    for(int i=0; i<n; i++) {
        for(int j=0; j<m; j++) {
            data[i*m+j] = unif(re)*data[i*m+j];
        }
    }
}


// overloaded operators
double& Matrix::operator[](size_t index) {
    return this->data[index];
}

Matrix operator+(Matrix const& l, Matrix const& r) {
    //assert(l.n == r.n && l.m == r.m);
    if(l.n != r.n || l.m != r.m) {
        fprintf(stderr, "Dimension mismatch when adding matrices: %dx%d and %dx%d\n", l.n, l.m, r.n, r.m);
    }
    Matrix result(l.n, l.m);
    for(int i=0; i<l.n; i++) {
        for(int j=0; j<l.m; j++) {
            result.setElementAt(i, j, l.getElementAt(i,j) + r.getElementAt(i, j));
        }
    }
    return result;
}


Matrix operator*(const Matrix& l, const Matrix& r) {
    if(l.m != r.n) {
        fprintf(stderr, "Dimension mismatch when multiplying matrices: %dx%d and %dx%d\n", l.n, l.m, r.n, r.m);
        return Matrix(l.n, r.m);
    }
    Matrix result(l.n, r.m);
    for(int i=0; i<l.n; i++) {
        for(int j=0; j<r.m; j++) {
            double sum =0;
            for(int k=0; k<l.m; k++) {
                sum += l.getElementAt(i,k) * r.getElementAt(k, j);
            }
            result.setElementAt(i, j, sum);
        }
    }
    return result;
}
