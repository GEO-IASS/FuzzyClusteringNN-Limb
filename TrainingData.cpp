//
// Created by Nick Lofton on 11/13/16.
//
#include "TrainingData.h"

void TrainingData::getTopology(std::vector<unsigned> &topology) {
    std::string line;
    std::string label;

    m_trainingDataFile.clear();
    m_trainingDataFile.seekg(0, std::ios::beg);

    std::getline(m_trainingDataFile, line);
    std::stringstream stringStream(line);
    stringStream >> label;
    if (this->isEof() || label.compare("topology:") != 0) {
        std::abort();
    }

    while (!stringStream.eof()) {
        unsigned number;
        stringStream >> number;
        topology.push_back(number);
    }

    std::cout << "Topology ";
    for (unsigned index = 0; index < topology.size(); index++) {
        std::cout << topology[index] << " ";
    }

    std::cout << std::endl;

    return;
}

TrainingData::TrainingData(const std::string filename) {
    m_trainingDataFile.open(filename.c_str());
}

unsigned TrainingData::getNextInputs(std::vector<double> &inputVals) {
    inputVals.clear();

    std::string line;
    std::getline(m_trainingDataFile, line);
    std::stringstream stringStream (line);

    std::string label;
    stringStream >> label;

    if (label.compare("in:") == 0) {
        double oneValue;
        while (stringStream >> oneValue) {
            inputVals.push_back(oneValue);
        }
    }

    return inputVals.size();
}

unsigned TrainingData::getTargetOutputs(std::vector<double> &targetOutputVals){
    targetOutputVals.clear();

    std::string line;
    std::getline(m_trainingDataFile, line);
    std::stringstream stringStream (line);

    std::string label;
    stringStream >> label;

    if (label.compare("out:") == 0) {
        double oneValue;
        while (stringStream >> oneValue) {
            targetOutputVals.push_back(oneValue);
        }
    }

    return targetOutputVals.size();
}

void TrainingData::showVectorVals(std::string label, std::vector<double> vector) {
    std::cout << label << " ";
    for (unsigned index = 0; index < vector.size(); index++) {
        std::cout << vector[index] << " ";
    }

    std::cout << std::endl;
}
