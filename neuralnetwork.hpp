#pragma once

#include "matrix.hpp"
#include "dataset.hpp"
#include <stdlib.h>
#include <vector>
#include <memory>
#include <string>



class Network {

public:
    Network();
    Network(std::vector<int> layer_sizes);
    Network(std::string filename);
    void deepCopy(Network& dest) const;
    Matrix apply(const Matrix& in_vec);
    void print() const;
    void adjustRandomSlightly(double relChange);
    double getAverageError(DataSet &dataset);
    void saveAsFile(std::string filename) const;

private:
    std::unique_ptr<Matrix[]> layer_weights;
    std::unique_ptr<Matrix[]> layer_biases;
    std::vector<int> layer_sizes;
    int layer_count;
    int transition_count;
};

