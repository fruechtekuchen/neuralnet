#include "networktrainer.hpp"
#include "neuralnetwork.hpp"
#include "dataset.hpp"
#include <vector>
#include <string>
#include <iostream>



NetworkTrainer::NetworkTrainer(std::vector<int> &layer_sizes, int n_samples, std::string filename_images, std::string filename_labels) {
    mainNetwork = Network(layer_sizes);
    otherNetwork = Network(layer_sizes);
    mainNetwork.deepCopy(otherNetwork);
    dataset = DataSet(n_samples, filename_images, filename_labels);
}


void NetworkTrainer::trainOneIteration(double relChange) {
    otherNetwork.adjustRandomSlightly(relChange);
    double errMain = getAverageError(mainNetwork);
    double errOther = getAverageError(otherNetwork);
    if(errOther < errMain) {
        otherNetwork.deepCopy(mainNetwork);
    }
}

double NetworkTrainer::getAverageError() {
    return getAverageError(mainNetwork);
}

double NetworkTrainer::getAverageError(Network &mainNetwork) {
    double err = 0;
    for(int i=0; i<dataset.sample_count; i++) {
        DataSample &sample = dataset[i];
        Matrix vec_out = mainNetwork.apply(sample.vec);
        vec_out[sample.label] -= 1;
        err += vec_out.getVectorNorm()/(double)dataset.sample_count;
    }

    return err;
}