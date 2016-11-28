//
// Created by Nick Lofton on 11/16/16.
//

#include <string>
#include <sstream>
#include "Signal.h"
Signal::Signal(std::vector<double> signal) {
    setSignal(signal);
}

void Signal::setSignal(std::vector<double> signal) {
    m_signal = signal;
}

std::vector<double> Signal::getSignal() {
    return m_signal;
}

void Signal::setClusterIndex(double clusterIndex) {
    m_clusterIndex = clusterIndex;
}

double Signal::getClusterIndex() {
    return m_clusterIndex;
}

std::string Signal::toString()  const {
    std::ostringstream returnVal;

    if (!m_signal.empty())
    {
        // Convert all but the last element to avoid a trailing ","
        std::copy(m_signal.begin(), m_signal.end() - 1, std::ostream_iterator<double>(returnVal, ", "));

        // Now add the last element with no delimiter
        returnVal << m_signal.back();
    }

    return returnVal.str();
}

Signal::Signal() {}
