#include "dataset.hpp"
#include "libs/encode_png.hpp"
#include <memory>
#include <cstring>
#include <string>
#include <fstream>
#include <iostream>

DataSample::DataSample() {
    this->label = 0;
    this->n_rows = 0;
    this->n_cols = 0;
}

DataSample::DataSample(int label, unsigned char *vals, int n_rows, int n_cols) {
    int n = n_rows*n_cols;
    this->label = label;
    this->n_rows = n_rows;
    this->n_cols = n_cols;
    vec = Matrix(n, 1, vals);
    original_data = std::make_unique<unsigned char[]>(n);
    std::memcpy(original_data.get(), vals, n);
}

void DataSample::print() const {
    printf("Datasample:\n");
    printf("Label: %d\n", label);
    printf("Data:\n");
    vec.print();
}


static void check_ifstream_die_if_error(std::ifstream &infile) {
    if (infile.fail()) {
        if (infile.eof()) {
            std::cerr << "EOF occured early\n";
            exit(EXIT_FAILURE);
        }
        std::perror("ifstream.read");
        exit(EXIT_FAILURE);
    }
}

static int32_t read_and_flip_int32(std::ifstream &infile) {
    int32_t number;
    infile.read(reinterpret_cast<char *>(&number), sizeof(number));
    check_ifstream_die_if_error(infile);
    number = __builtin_bswap32(number);
    return number;
}

static unsigned char read_uchar(std::ifstream &infile) {
    unsigned char number = 0;
    infile.read(reinterpret_cast<char *>(&number), 1);
    check_ifstream_die_if_error(infile);
    return number;
}

static void read_image(std::ifstream &infile, unsigned char *buffer, int n_pixels) {
    for(int i=0; i<n_pixels; i++) {
        unsigned char c = read_uchar(infile);
        buffer[i] = c;
    }
}

static void read_label(std::ifstream &infile, unsigned char &label) {
    infile.read(reinterpret_cast<char *>(&label), 1);
    check_ifstream_die_if_error(infile);
}

static void read_header_image_file(std::ifstream &image_file, int32_t &n_images, int32_t &n_rows, int32_t &n_cols) {
    int32_t magic_number = read_and_flip_int32(image_file);
    if (magic_number != 2051) {
        std::cerr << "The magic number of the image file is not correct. This means that the file is not parsed correctly or that it's magic number is not 2051\n";
        exit(EXIT_FAILURE);
    }
    n_images = read_and_flip_int32(image_file);
    n_rows = read_and_flip_int32(image_file);
    n_cols = read_and_flip_int32(image_file);

}

static void read_header_label_file(std::ifstream &label_file, int32_t &n_labels) {
    int32_t magic_number = read_and_flip_int32(label_file);
    if (magic_number != 2049) {
        std::cerr << "The magic number of the image file is not correct. This means that the file is not parsed correctly or that it's magic number is not 2051\n";
        exit(EXIT_FAILURE);
    }
    n_labels = read_and_flip_int32(label_file);
}


DataSet::DataSet() {
    sample_count = 0;
    sample_size = 0;
    data = std::make_unique<DataSample[]>(0);
}


DataSet::DataSet(int32_t number_of_samples, std::string filename_images, std::string filename_labels) {
    std::ifstream image_file(filename_images, std::ios::in | std::ios::binary);
    if(!image_file) {
        std::cerr << "Reading in image file did not work :/.('" << filename_images << "')\n";
        exit(EXIT_FAILURE);
    }
    std::ifstream label_file(filename_labels, std::ios::in | std::ios::binary);
    if(!label_file) {
        std::cerr << "Reading in label file did not work :/.('" << filename_labels << "')\n";
        exit(EXIT_FAILURE);
    }

    int32_t n_images, n_rows, n_cols;
    read_header_image_file(image_file, n_images, n_rows, n_cols);
    int32_t n_pixels_per_image = n_rows * n_cols;
    if(number_of_samples > n_images) {
        std::cerr << "NumberOfSamples is too large. The given file only contains " << n_images << " datapoints\n";
        exit(EXIT_FAILURE);
    }

    int32_t n_labels;
    read_header_label_file(label_file, n_labels);
    if (n_images != n_labels) {
        std::cerr << "Image file and label file have a different number of items.\n";
        std::cerr << "The files you are using are not made fo each other :(\n";
        exit(EXIT_FAILURE);
    }


    this->data = std::make_unique<DataSample[]>(number_of_samples);
    sample_count = number_of_samples;
    this->sample_size = n_pixels_per_image;

    // rading in the data
    std::unique_ptr<unsigned char[]> image_buffer = std::make_unique<unsigned char[]>(n_pixels_per_image);
    unsigned char label_buffer = 111;

    for(int32_t i = 0; i < number_of_samples; i++) {

        read_image(image_file, image_buffer.get(), n_pixels_per_image);
        read_label(label_file, label_buffer);
        data[i] = DataSample(label_buffer, image_buffer.get(), n_rows, n_cols);
    }

    image_file.close();
    label_file.close();
}

DataSample& DataSet::operator[](size_t index) {
    return data[index];
}
