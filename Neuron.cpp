//
// Created by Nick Lofton on 11/13/16.
//

#include "Neuron.h"

double Neuron::eta = 2; // overall net learning rate
double Neuron::alpha = 0.1; // momentum, multiplier of last deltaWeight [

void Neuron::updateInputWeights(Layer &prevLayer) {
    // The weights to be updated are in the Connection container in the neurons in the preceding layer
    for (unsigned neuronNum = 0; neuronNum < prevLayer.size(); neuronNum++) {
        Neuron &neuron = prevLayer[neuronNum];
        double oldDeltaWeight = neuron.m_outputWeights[m_myIndex].deltaWeight;

        double newDeltaWeight =
                // Individual input, magnified by the gradient and training rate
                (eta * neuron.getOutputVal() * m_gradient)
                // Also add the momentum * a fraction of the previous delta weight
                + (alpha * oldDeltaWeight);

        neuron.m_outputWeights[m_myIndex].deltaWeight = newDeltaWeight;
        neuron.m_outputWeights[m_myIndex].weight += newDeltaWeight;
    }
}


double Neuron::sumDeltaOfWeights(const Layer &nextLayer) const {
    double sum = 0.0;

    // Sum our contributions of the errors at the nodes we feed
    for (unsigned neuronNum = 0; neuronNum < nextLayer.size() - 1; neuronNum++) {
        sum += m_outputWeights[neuronNum].weight * nextLayer[neuronNum].m_gradient;
    }

    return sum;
}

void Neuron::calcHiddenGradients(const Layer &nextLayer) {
    double deltaOfWeights = sumDeltaOfWeights(nextLayer);
    m_gradient = deltaOfWeights * Neuron::activationFunctionDerivative(m_outputVal);
}

void Neuron::calcOutputGradients(double targetVal) {
    double delta = targetVal - m_outputVal;
    m_gradient = delta * Neuron::activationFunctionDerivative(m_outputVal);
}
double Neuron::activationFunction(double x) {
    // tanh - output range [-1.0...1.0]
    return tanh(x);
}

double Neuron::activationFunctionDerivative(double x){
    return 1.0 - (x * x);
}

void Neuron::feedForward(Layer &prevLayer) {
    double sum = 0.0;

    // Sum the previous layer's outputs (which are our inputs)
    // Include the bias node from the previous layer
    for (unsigned neuronNum = 0; neuronNum < prevLayer.size(); neuronNum++) {
        sum += prevLayer[neuronNum].getOutputVal() * prevLayer[neuronNum].m_outputWeights[m_myIndex].weight;
    }

    m_outputVal = Neuron::activationFunction(sum);
}

Neuron::Neuron(unsigned numOutputs, unsigned myIndex) {
    for (unsigned connections = 0; connections < numOutputs; connections++) {
        m_outputWeights.push_back(Connection());
        m_outputWeights.back().weight = randomWeight();
    }

    m_myIndex = myIndex;
}