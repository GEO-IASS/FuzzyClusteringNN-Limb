//
// Created by Nick Lofton on 11/15/16.
//

#include "Cluster.h"
#include "Signal.h"

void Cluster::addToCluster(Signal signal) {
    m_signals.push_back(signal);
}
Signal Cluster::getCentriod(void) const {
    return m_centriod;
}
void Cluster::setCentriod(Signal signal) {
    m_centriod = signal;
}

void Cluster::calculateNewCentriod() {

}

Cluster::Cluster() {
    std::vector<double> randomValues;
    for (int i = 0; i < 5; i++) {
        randomValues.push_back(randomPoint());
    }
    m_centriod = Signal(randomValues);
}

Cluster::Cluster(Signal initialCentriod) {
    m_centriod = initialCentriod;
}
