//
// Created by Nick Lofton on 11/13/16.
//

#ifndef NEURALNETWORK_TRAININGDATA_H
#define NEURALNETWORK_TRAININGDATA_H

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

class TrainingData {
public:
    TrainingData(const std::string filename);
    bool isEof(void) { return m_trainingDataFile.eof(); }
    void getTopology(std::vector<unsigned> &topology);
    unsigned getNextInputs(std::vector<double> &inputVals);
    unsigned getTargetOutputs(std::vector<double> &targetOutputVals);
    void showVectorVals(std::string label, std::vector<double> vector);
    double getFuzzifier(void);
    double getErrorMargin(void);
    double getIterLimit(void);
    double getLearningRate(void);
    double getMomentum(void);
    void resetEOF(void) { m_trainingDataFile.clear(); m_trainingDataFile.seekg(0, std::ios::beg);}
private:
    std::ifstream m_trainingDataFile;
};

#endif //NEURALNETWORK_TRAININGDATA_H
