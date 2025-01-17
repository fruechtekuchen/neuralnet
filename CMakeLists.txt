cmake_minimum_required(VERSION 3.5.0)
project(neuralnetwork VERSION 0.1.0 LANGUAGES C CXX)

add_executable(train main.cpp neuralnetwork.cpp networktrainer.cpp dataset.cpp matrix.cpp libs/encode_png.cpp libs/lodepng.cpp)

