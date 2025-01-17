#pragma once

#include "matrix.hpp"
#include <memory>

class DataSample{
public:
    Matrix vec;
    int label;
    int n_rows, n_cols;
    // original_data is only used to write it out as an image later. It can be removed if this isn't necessary.
    std::unique_ptr<unsigned char[]> original_data;

    // this constructor does not return a valid data sample. This only exists so that an array of DataPoints can be created.
    DataSample();
    DataSample(int label, unsigned char *vals, int32_t n_rows, int32_t n_cols);

    void print() const;
};


class DataSet {
public:
    int sample_count;
    std::unique_ptr<DataSample[]> data;
    int sample_size;

    DataSet();
    // creates dataset from a file in the format specified by https://yann.lecun.com/exdb/mnist/
    DataSet(int32_t number_of_samples, std::string filename_images, std::string filename_labels);
    DataSample& operator[](size_t index);
};

