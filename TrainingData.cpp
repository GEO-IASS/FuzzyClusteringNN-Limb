//
// Created by Nick Lofton on 11/13/16.
//
#include "TrainingData.h"

void TrainingData::getTopology(std::vector<unsigned> &topology) {
    std::string line;
    std::string label;

    std::getline(m_trainingDataFile, line);
    std::stringstream stringStream(line);
    stringStream >> label;

    if (label.compare("topology:") == 0) {
        unsigned oneValue;
        while (stringStream >> oneValue) {
            topology.push_back(oneValue);
        }
    }

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

double TrainingData::getFuzzifier(void) {

    std::string line;
    std::getline(m_trainingDataFile, line);
    std::stringstream stringStream (line);

    std::string label;
    stringStream >> label;
    double fuzzifier = 2;
    if (label.compare("fuzzifier:") == 0) {
        double oneValue;
        while (stringStream >> oneValue) {
            fuzzifier = oneValue;
        }
    }

    return fuzzifier;
};
double TrainingData::getErrorMargin(void) {

    std::string line;
    std::getline(m_trainingDataFile, line);
    std::stringstream stringStream (line);

    std::string label;
    stringStream >> label;

    double errorMargin = 0.15;
    if (label.compare("errorMargin:") == 0) {
        double oneValue;
        while (stringStream >> oneValue) {
            errorMargin = oneValue;
        }
    }

    return errorMargin;

};
double TrainingData::getIterLimit(void) {

    std::string line;
    std::getline(m_trainingDataFile, line);
    std::stringstream stringStream (line);

    std::string label;
    stringStream >> label;

    double iterationLimit = 1000;
    if (label.compare("iterationLimit:") == 0) {
        double oneValue;
        while (stringStream >> oneValue) {
            iterationLimit = oneValue;
        }
    }

    return iterationLimit;
};
double TrainingData::getLearningRate(void) {

    std::string line;
    std::getline(m_trainingDataFile, line);
    std::stringstream stringStream (line);

    std::string label;
    stringStream >> label;

    double learningRate = 2;
    if (label.compare("learningRate:") == 0) {
        double oneValue;
        while (stringStream >> oneValue) {
            learningRate = oneValue;
        }
    }

    return learningRate;

};
double TrainingData::getMomentum(void) {

    std::string line;
    std::getline(m_trainingDataFile, line);
    std::stringstream stringStream (line);

    std::string label;
    stringStream >> label;

    double momentum = 0.1;
    if (label.compare("momentum:") == 0) {
        double oneValue;
        while (stringStream >> oneValue) {
            momentum = oneValue;
        }
    }

    return momentum;

};
