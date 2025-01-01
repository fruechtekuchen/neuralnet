cmake_minimum_required(VERSION 3.5.0)
project(neuralnetwork VERSION 0.1.0 LANGUAGES C CXX)

add_executable(nn main.cpp neuralnetwork.cpp matrix.cpp)

