#include "networktrainer.hpp"
#include "neuralnetwork.hpp"
#include "dataset.hpp"
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


void NetworkTrainer::trainOneIteration(double relChange) {
    otherNetwork.adjustRandomSlightly(relChange);
    double errMain = mainNetwork.getAverageError(dataset);
    double errOther = otherNetwork.getAverageError(dataset);
    if(errOther < errMain) {
        otherNetwork.deepCopy(mainNetwork);
    }
}

double NetworkTrainer::getAverageError() {
    return mainNetwork.getAverageError(dataset);
}

