//
// Created by Nick Lofton on 11/15/16.
//

#ifndef NEURALNETWORK_CLUSTER_H
#define NEURALNETWORK_CLUSTER_H


#include <vector>
#include <cstdlib>
#include "Signal.h"

class Cluster {
public:
    Cluster(Signal initialCentriod);
    Cluster(void);
    Signal getCentriod(void) const;
    void setCentriod(Signal newCentriod);
private:
    double randomPoint(void) const {return rand() / double(RAND_MAX);};
    std::vector<Signal> m_signals; // m_signals[signalValue]
    Signal m_centriod;
};


#endif //NEURALNETWORK_CLUSTER_H
