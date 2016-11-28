cmake_minimum_required(VERSION 3.6)
project(FuzzyClusteringNN-Limb)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp Network.h Neuron.cpp Neuron.h Network.cpp TrainingData.cpp TrainingData.h ClusterDataSet.cpp ClusterDataSet.h Cluster.cpp Cluster.h Signal.cpp Signal.h)
add_executable(FuzzyClusteringNN-Limb ${SOURCE_FILES})