#pragma once

#include "matrix.hpp"
#include <stdlib.h>
#include <vector>
#include <memory>



class Network {

public:
    Network();
    Network(std::vector<int> layer_sizes);
    void deepCopy(Network& dest) const;
    Matrix apply(Matrix& in_vec);
    void print() const;
    void adjustSlightly(double relChange);


private:
    std::unique_ptr<Matrix[]> layer_weights;
    std::unique_ptr<Matrix[]> layer_biases;
    std::vector<int> layer_sizes;
    int layer_count;
    int transition_count;
};

