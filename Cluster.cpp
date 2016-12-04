//
// Created by Nick Lofton on 11/15/16.
//

#include "Cluster.h"
#include "Signal.h"

Signal Cluster::getCentriod(void) const {
    return m_centriod;
}
void Cluster::setCentriod(Signal signal) {
    m_centriod = signal;
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
