//
// Created by Nick Lofton on 11/13/16.
//

#ifndef NEURALNETWORK_NEURON_H
#define NEURALNETWORK_NEURON_H

#include <vector>
#include <cstdlib>
#include <cassert>
#include <cmath>

struct Connection {
    double weight;
    double deltaWeight;
};

class Neuron;

typedef std::vector<Neuron> Layer;

// Class Neuron
class Neuron {
public:
    Neuron(unsigned numOutputs, unsigned myIndex);
    void setOutputVal(double val) { m_outputVal = val; }
    double getOutputVal(void) const { return m_outputVal; }
    void feedForward(Layer & prevLayer);
    void calcOutputGradients(double targetVal);
    void calcHiddenGradients(const Layer &nextLayer);
    void updateInputWeights(Layer &prevLayer);
    void setLearningRate(double learningRate) { eta = learningRate; };
    void setMomentum(double momentum) { alpha = momentum; };
private:
    static double activationFunction(double x);
    static double activationFunctionDerivative(double x);
    static double randomWeight(void) { return rand() / double(RAND_MAX); }
    double sumDeltaOfWeights (const Layer &nextLayer) const;
    double m_outputVal;
    std::vector<Connection> m_outputWeights;
    unsigned m_myIndex;
    double m_gradient;
    double eta;   // [0.0...1.0] overall net training weight
    double alpha; // [0.0...n] multiplier of the last weight change (momentum)
};

#endif //NEURALNETWORK_NEURON_H
