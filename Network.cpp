//
// Created by Nick Lofton on 11/13/16.
//

#include "Network.h"

void Net::getResults(std::vector<double> &resultVals) const {
    resultVals.clear();
    for (unsigned neuronNum = 0; neuronNum < m_layers.back().size() - num_bias; neuronNum++) {
        resultVals.push_back(m_layers.back()[neuronNum].getOutputVal());
    }

};

void Net::backProp(const std::vector<double> &targetVals) {
    // calculate overall net error (RMS of output neuron errors)
    Layer &outputLayer = m_layers.back();
    m_error = 0.0;

    // cost function
    for (unsigned neuronNum = 0; neuronNum < outputLayer.size() - num_bias; neuronNum++) {
        double delta = targetVals[neuronNum] - outputLayer[neuronNum].getOutputVal();
        m_error += delta * delta;
    }

    m_error /= (outputLayer.size());
    m_error = sqrt(m_error);

    m_recentAverageError = (m_recentAverageError * m_recentAverageSmoothingFactor + m_error)
                           / (m_recentAverageSmoothingFactor + 1.0);

    // calculate output layer gradients
    for (unsigned neuronNum = 0; neuronNum < outputLayer.size() - num_bias; neuronNum++) {
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

        for (unsigned neuronNum = 0; neuronNum < layer.size() - num_bias; neuronNum++) {
            layer[neuronNum].updateInputWeights(prevLayer);
        }
    }
};

void Net::feedForward(const std::vector<double> &inputVals) {
    assert(inputVals.size() == m_layers[0].size() - num_bias);

    // Assign (latch)
    for (unsigned index = 0; index < inputVals.size(); index++) {
        m_layers[0][index].setOutputVal(inputVals[index]);
    }

    // Forward Propogation
    for (unsigned layerNum = 1; layerNum < m_layers.size(); layerNum++) {
        Layer &prevLayer = m_layers[layerNum - 1];
        for (unsigned neuronNum = 0; neuronNum < m_layers[layerNum].size() - num_bias; neuronNum++) {
            m_layers[layerNum][neuronNum].feedForward(prevLayer);
        }
    }
};


Net::Net(const std::vector<unsigned> &topology, double learningRate, double momentum) {
    unsigned numLayers = topology.size();
    for (unsigned layerNum = 0; layerNum < numLayers; layerNum++) {
        m_layers.push_back(Layer());
        unsigned numOutputs = (layerNum == numLayers - 1) ? 0 : topology[layerNum + 1];

        // We have made a new layer, now fill it with neurons
        for (unsigned neuronNum = 0; neuronNum < topology[layerNum]; neuronNum++) {
            m_layers.back().push_back(Neuron(numOutputs, neuronNum));
            m_layers.back()[neuronNum].setMomentum(momentum);
            m_layers.back()[neuronNum].setLearningRate(learningRate);
        }

        for (int neuronNum = topology[layerNum]; neuronNum < topology[layerNum] + num_bias; neuronNum++) {
            m_layers.back().push_back(Neuron(numOutputs, neuronNum));
            m_layers.back()[neuronNum].setMomentum(momentum);
            m_layers.back()[neuronNum].setLearningRate(learningRate);
            // Force the bias neuron to have a constant output to 1.0.
            m_layers.back().back().setOutputVal(1.0);
        }
    }
}
