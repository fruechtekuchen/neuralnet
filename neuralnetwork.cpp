#include "neuralnetwork.hpp"

#include "matrix.hpp"
#include <stdlib.h>
#include <memory>
#include <iostream>
#include <cmath>


double activation_func(double d) {
    return 1./(1.+std::exp(-d));
}

Network::Network(){}

Network::Network(std::vector<int> layer_sizes) {
    if(layer_sizes.size() <= 1) return;
    layer_weights = std::make_unique<Matrix[]>(layer_sizes.size()-1);
    layer_biases = std::make_unique<Matrix[]>(layer_sizes.size()-1);
    this->layer_sizes = layer_sizes;
    layer_count = layer_sizes.size();
    transition_count = layer_count-1;

    // init the layer transitions
    for(int layer = 0; layer<transition_count; layer++) {
        layer_weights[layer] = Matrix(layer_sizes[layer+1], layer_sizes[layer]);
        layer_weights[layer].fillRandom(10.);
        
        layer_biases[layer] = Matrix(layer_sizes[layer+1], 1);
        layer_biases[layer].fillRandom(1.);
    }
    
}

void Network::deepCopy(Network& dest) const {
    if(layer_sizes.size() != dest.layer_sizes.size()) {
        fprintf(stderr, "Dimension mismatch when copying mainNetwork\n");
    }
    for(int layer = 0; layer<transition_count; layer++) {
        dest.layer_sizes[layer] = layer_sizes[layer];
        layer_weights[layer].deepCopy(dest.layer_weights[layer]);
        layer_biases[layer].deepCopy(dest.layer_biases[layer]);
    }
}

Matrix Network::apply(const Matrix& in_vec) {
    if(in_vec.n != layer_sizes[0] || in_vec.m != 1) {
        std::cout << "error: Network::apply: dimension mismatch\n";
        exit(1);
    }
    Matrix result;
    in_vec.deepCopy(result);

    for(int layer=0; layer<transition_count; layer++) {
        result = layer_weights[layer] * result;
        result = result + layer_biases[layer];
        result.applyFunction(activation_func);
    }

    return result;
}

void Network::print() const {
    for(int layer = 0; layer<transition_count; layer++) {
        std::printf("Layers size: %d\n", layer_sizes[layer]);
        std::printf("Weights:\n");
        layer_weights[layer].print();
        std::printf("Biases:\n");
        layer_biases[layer].print();
    }
    std::printf("\n");

}

void Network::adjustRandomSlightly(double relChange) {
    for(int layer = 0; layer<transition_count; layer++) {
        layer_weights[layer].adjustRandomSlightly(relChange);
        layer_biases[layer].adjustRandomSlightly(relChange);
    }
}
