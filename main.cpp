#include "neuralnetwork.hpp"
#include "matrix.hpp"

#include <iostream>


/* TODO:
- read in data
- make trainer to train the network
- make function to modify by aqdjustable value
*/

int main() {

   std::vector<int> layer_sizes = {2, 1};
   Network net1(layer_sizes);
   net1.print();

   net1.adjustSlightly(0.05);
   net1.print();

   std::cout << ":)\n";
}