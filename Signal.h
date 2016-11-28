//
// Created by Nick Lofton on 11/16/16.
//

#ifndef NEURALNETWORK_ARMODEL_H
#define NEURALNETWORK_ARMODEL_H


#include <vector>

class Signal {
public:
    Signal(std::vector<double> signal);
    Signal();
    std::vector<double> getSignal(void);
    void setSignal(std::vector<double> signal);
    void setClusterIndex(double clusterIndex);
    double getClusterIndex(void);
    std::string toString() const;
private:
    std::vector<double> m_signal;
    double m_clusterIndex;
};


#endif //NEURALNETWORK_ARMODEL_H
