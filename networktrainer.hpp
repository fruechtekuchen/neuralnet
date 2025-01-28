#pragma once

#include "neuralnetwork.hpp"
#include "dataset.hpp"

#include <vector>

class NetworkTrainer{

public:

    Network mainNetwork;
    DataSet dataset;


    NetworkTrainer(std::vector<int> &layer_sizes, int n_samples, std::string filename_images, std::string filename_labels);
    

    void trainOneIterationDarwin(double relChange);
    void trainDarwin(double relChangeToError, int n_iterations);
    void trainOneIterationBackprop(double relChange);
    void trainBackprop(double relChangeToError, int n_iterations);
    double getAverageError();
    double getAverageError(Network &mainNetwork);


private:
    Network otherNetwork;


};