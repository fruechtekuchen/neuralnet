#include "neuralnetwork.hpp"
#include "networktrainer.hpp"
#include "libs/encode_png.hpp"

#include <iostream>



/* TODO:
- make trainer to train the mainNetwork
*/

static NetworkTrainer createTrainer(int n_samples);


int main(int argc, char *argv[]) {

   int n_samples = 50;
   if(argc > 1) n_samples = atoi(argv[1]);
   printf("Using %d samples.\n", n_samples);
   NetworkTrainer trainer = createTrainer(n_samples);

   double currentError = trainer.getAverageError();
   printf("Err start: %.17gf\n", currentError);
   trainer.trainDarwin(0.001, 10);
   currentError = trainer.getAverageError();
   printf("Err end: %.17gf\n", currentError);


   std::cout << ":)\n";
}


static NetworkTrainer createTrainer(int sample_count) {
   int sample_size = 784; // number of pixels per image
   int output_size = 10;

   std::vector<int> layer_sizes = {sample_size, sample_size, sample_size/10, output_size};

   return NetworkTrainer(layer_sizes, sample_count, "data/train-images-idx3-ubyte", "data/train-labels-idx1-ubyte");
}


