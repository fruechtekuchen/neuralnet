#include "neuralnetwork.hpp"

#include "matrix.hpp"
#include "dataset.hpp"
#include <stdlib.h>
#include <memory>
#include <iostream>
#include <cmath>

#include "timer.hpp"

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

// optimize this
Matrix Network::apply(const Matrix& in_vec) {
    if(in_vec.n != layer_sizes[0] || in_vec.m != 1) {
        std::cout << "error: Network::apply: dimension mismatch\n";
        exit(1);
    }
    Matrix result;
    Timer timer = Timer(); double time_mult = 0.;
    in_vec.deepCopy(result);
    
    for(int layer=0; layer<transition_count; layer++) {
        timer.reset();
        result = layer_weights[layer] * result;
        time_mult += timer.elapsed();

        result = result + layer_biases[layer];
        result.applyFunction(activation_func);
    }
    //printf("mult: %f\n", time_mult);

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

// optimize this
double Network::getAverageError(DataSet &dataset) {
    double err = 0;
    for(int i=0; i<dataset.sample_count; i++) {
        DataSample &sample = dataset[i];
        Matrix vec_out = this->apply(sample.vec);
        vec_out[sample.label] -= 1;
        err += vec_out.getVectorNorm()/(double)dataset.sample_count;
    }

    return err;
}

Matrix Network::getAverageErrorVector(DataSet &dataset) {
    Matrix err = Matrix(layer_sizes[layer_sizes.size()-1], 1);
    err.setAllElementsTo(0.);
    for(int i=0; i<dataset.sample_count; i++) {
        DataSample &sample = dataset[i];
        Matrix vec_out = this->apply(sample.vec);
        vec_out[sample.label] -= 1;
        // TODO: optimize with "Matrix += Matrix"
        err = err + (1./(double)dataset.sample_count * vec_out);
    }

    return err;
}