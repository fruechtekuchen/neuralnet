#include "networktrainer.hpp"
#include "neuralnetwork.hpp"
#include "dataset.hpp"
#include "matrix.hpp"
#include <vector>
#include <string>
#include <iostream>

#include "timer.hpp"



NetworkTrainer::NetworkTrainer(std::vector<int> &layer_sizes, int n_samples, std::string filename_images, std::string filename_labels) {
    mainNetwork = Network(layer_sizes);
    otherNetwork = Network(layer_sizes);
    mainNetwork.deepCopy(otherNetwork);
    dataset = DataSet(n_samples, filename_images, filename_labels);
}


void NetworkTrainer::trainOneIterationDarwin(double relChange) {
    otherNetwork.adjustRandomSlightly(relChange);
    double errMain = mainNetwork.getAverageError(dataset);
    double errOther = otherNetwork.getAverageError(dataset);
    if(errOther < errMain) {
        otherNetwork.deepCopy(mainNetwork);
    }
}

void NetworkTrainer::trainDarwin(double relChangeToError, int n_iterations) {
    double currentError = mainNetwork.getAverageError(dataset);
    for(int i=0; i<n_iterations; i++) {
        printf("IT\n");
        trainOneIterationDarwin(relChangeToError * currentError);
        currentError = mainNetwork.getAverageError(dataset);
    }
}

void NetworkTrainer::trainOneIterationBackprop(double relChange) {
    // TODO
    
}
void NetworkTrainer::trainBackprop(double relChange, int n_iterations) {
    for(int i=0; i<n_iterations; i++) {
        printf("IT\n");
        trainOneIterationBackprop(relChange);
    }
}


double NetworkTrainer::getAverageError() {
    return mainNetwork.getAverageError(dataset);
}

