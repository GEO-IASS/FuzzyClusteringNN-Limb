//
// Created by Nick Lofton on 11/13/16.
//

#ifndef NEURALNETWORK_NET_H
#define NEURALNETWORK_NET_H

#include <vector>
#include <iostream>
#include "Neuron.h"

class Net {
public:
    Net(const std::vector<unsigned> &topology);
    void feedForward(const std::vector<double> &inputVals);
    void backProp(const std::vector<double> &targetVals);
    void getResults(std::vector<double> &resultVals) const;
    double getRecentAverageError(void) const { return m_recentAverageError; };
private:
    std::vector<Layer> m_layers; // m_layers[layerNum][neuronNum]
    double m_error;
    double m_recentAverageError;
    double m_recentAverageSmoothingFactor;
};

#endif //NEURALNETWORK_NET_H
