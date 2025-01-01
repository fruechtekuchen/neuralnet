#include "networktrainer.hpp"

#include "neuralnetwork.hpp"
#include <vector>



NetworkTrainer::NetworkTrainer(std::vector<int> layer_sizes) {
    network = Network(layer_sizes);
    
}
