//
// Created by Nick Lofton on 11/13/16.
//

#include "Network.h"

void Net::getResults(std::vector<double> &resultVals) const {
    resultVals.clear();
    for (unsigned neuronNum = 0; neuronNum < m_layers.back().size() - 1; neuronNum++) {
        resultVals.push_back(m_layers.back()[neuronNum].getOutputVal());
    }

};

void Net::backProp(const std::vector<double> &targetVals) {
    // calculate overall net error (RMS of output neuron errors)
    Layer &outputLayer = m_layers.back();
    m_error = 0.0;

    // cost function
    for (unsigned neuronNum = 0; neuronNum < outputLayer.size() - 1; neuronNum++) {
        double delta = targetVals[neuronNum] - outputLayer[neuronNum].getOutputVal();
        m_error += delta * delta;
    }

    m_error /= 2;
    //m_error = sqrt(m_error);

    m_recentAverageError = (m_recentAverageError * m_recentAverageSmoothingFactor + m_error)
                           / (m_recentAverageSmoothingFactor + 1.0);

    // calculate output layer gradients
    for (unsigned neuronNum = 0; neuronNum < outputLayer.size() - 1; neuronNum++) {
        outputLayer[neuronNum].calcOutputGradients(targetVals[neuronNum]);
    }

    // calculate gradients on hidden layers
    for (unsigned layerNum = m_layers.size() - 2; layerNum > 0; layerNum--) {
        Layer &hiddenLayer = m_layers[layerNum];
        Layer &nextLayer = m_layers[layerNum + 1];

        for (unsigned neuronNum = 0; neuronNum < hiddenLayer.size(); neuronNum++) {
            hiddenLayer[neuronNum].calcHiddenGradients(nextLayer);
        }
    }

    // For all layers from outputs to first hidden layer, update connection weights
    for (unsigned layerNum = m_layers.size() - 1; layerNum > 0; layerNum--) {
        Layer &layer = m_layers[layerNum];
        Layer &prevLayer = m_layers[layerNum - 1];

        for (unsigned neuronNum = 0; neuronNum < layer.size() - 1; neuronNum++) {
            layer[neuronNum].updateInputWeights(prevLayer);
        }
    }
};

void Net::feedForward(const std::vector<double> &inputVals) {
    assert(inputVals.size() == m_layers[0].size() - 1);

    // Assign (latch)
    for (unsigned index = 0; index < inputVals.size(); index++) {
        m_layers[0][index].setOutputVal(inputVals[index]);
    }

    // Forward Propogation
    for (unsigned layerNum = 1; layerNum < m_layers.size(); layerNum++) {
        Layer &prevLayer = m_layers[layerNum - 1];
        for (unsigned neuronNum = 0; neuronNum < m_layers[layerNum].size() - 1; neuronNum++) {
            m_layers[layerNum][neuronNum].feedForward(prevLayer);
        }
    }
};


Net::Net(const std::vector<unsigned> &topology) {
    unsigned numLayers = topology.size();
    for (unsigned layerNum = 0; layerNum < numLayers; layerNum++) {
        m_layers.push_back(Layer());
        unsigned numOutputs = layerNum == topology.size() - 1 ? 0 : topology[layerNum + 1];

        // std::cout << "Added Layer: " << layerNum + 1 << std::endl;
        // We have made a new layer, now fill it with neurons
        // Add a bias neuron to the layer
        for (unsigned neuronNum = 0; neuronNum <= topology[layerNum]; neuronNum++) {
            m_layers.back().push_back(Neuron(numOutputs, neuronNum));
            // std::cout << "Made a Neuron" << std::endl;
        }

        // Force the bias neuron to have a constant output to 1.0.
        m_layers.back().back().setOutputVal(1.0);
    }
}
