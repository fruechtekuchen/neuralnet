#include "neuralnetwork.hpp"
#include <vector>

class NetworkTrainer{

public:

    Network network;

    NetworkTrainer(std::vector<int> layer_sizes);

};