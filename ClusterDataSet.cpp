//
// Created by Nick Lofton on 11/15/16.
//

#include "ClusterDataSet.h"
#include <cmath>
#include <iostream>

void ClusterDataSet::setInitialClusters(std::vector<Signal> &signalData) {

    m_membership.resize(signalData.size());
    for (int i = 0; i < signalData.size(); i++) {
        m_membership[i].resize(m_clusters.size());
    }
    std::vector<std::vector<double>> clusters;
    srand(time(NULL));

    // Guess initial position of cluster centers (Pick random centers)
    for (int clusterIndex = 0; clusterIndex < m_clusters.size(); clusterIndex++) {
        // Set initial centroid to random values from the data set
        bool duplicate = true;
        int randomSignalIndex;
        std::vector<double> currentSignal;
        while (duplicate) {
            duplicate = false;
            randomSignalIndex = rand() % signalData.size();
            currentSignal = signalData[randomSignalIndex].getSignal();
            for (int i = 0; i < clusters.size(); i++) {
                if (clusters[i][currentSignal.size()-1] == currentSignal[currentSignal.size()-1]) {
                    duplicate = true;
                    break;
                }
            }
        }

        clusters.push_back(currentSignal);
        m_clusters[clusterIndex].setCentriod(signalData[randomSignalIndex]);
    }
}

void ClusterDataSet::setInitialClusters() {
    setInitialClusters(m_signalData);
}

void ClusterDataSet::reindexSignals() {

    for (int i = 0; i < m_signalData.size(); i++) {
        double max = -1;

        for (int j = 0; j < m_clusters.size(); j++) {
            if (max < m_membership[i][j]) {
                max = m_membership[i][j];
                m_signalData[i].setClusterIndex(j);
            }
        }
    }
}

double ClusterDataSet::calculateDistance(Signal point, Signal centroid) {
    double sum = 0;

    for (int i = 0; i < point.getSignal().size(); i++) {
        sum += pow(point.getSignal()[i] - centroid.getSignal()[i], 2);
    }
    return sqrt(sum);
}

double ClusterDataSet::calculateObjectiveFunction() {
    double jm = 0;

    for (int i = 0; i < m_signalData.size(); i++) {
        for (int j = 0; j < m_clusters.size(); j++) {
            jm += pow(m_membership[i][j], m_fuzzifier) * pow(calculateDistance(m_signalData[i], m_clusters[j].getCentriod()), 2);
        }
    }

    return jm;
}

void ClusterDataSet::calculateClusterCenters() {
    for (int j = 0; j < m_clusters.size(); j++) {
        double l = 0;
        std::vector<double> newCentroid;

        newCentroid.resize(m_clusters[j].getCentriod().getSignal().size());
        for (int i = 0; i < m_signalData.size(); i++) {
            double uu = pow(m_membership[i][j], m_fuzzifier);
            for (int k = 0; k < m_clusters[j].getCentriod().getSignal().size(); k++) {
                newCentroid[k] += uu * m_signalData[i].getSignal()[k];
            }
            l += uu;
        }

        for (int k = 0; k < m_clusters[j].getCentriod().getSignal().size(); k++) {
            newCentroid[k] = newCentroid[k] / l;
        }
        m_clusters[j].setCentriod(newCentroid);
    }
}

std::vector<std::vector<double>> ClusterDataSet::fuzzyCMeansClustering(std::vector<Signal> &signalData, bool intializeClusters) {
    // Run Fuzzy C-Means on entire data set to create clusters
    // Choose:
    // the number of clusters,
    m_numOfClusters;
    // weighting exponent,
    m_fuzzifier = 2;
    // iteration limit (iter),
    m_iterationLimit = 1000;
    // termination criterion and norm for error.
    m_epsilon = 0.15;
    // Signal Data
    m_signalData = signalData;


    if (intializeClusters) {
        m_membership.resize(m_signalData.size());
        for (int i = 0; i < m_signalData.size(); i++) {
            m_membership[i].resize(m_clusters.size());
        }

        setInitialClusters();

        double distance;

        for (int i = 0; i < m_signalData.size(); i++) {
            double sum = 0;
            double sum2 = 0;

            for (int j = 0; j < m_clusters.size(); j++) {
                distance = calculateDistance(signalData[i], m_clusters[j].getCentriod());
                m_membership[i][j] = (distance == 0) ? m_epsilon : distance;
                sum += m_membership[i][j];
            }

            for (int j = 0; j < m_clusters.size(); j++) {
                m_membership[i][j] = 1.0 / pow(m_membership[i][j] / sum, (2.0 / (m_fuzzifier - 1)));
                sum2 += m_membership[i][j];
            }

            for (int j = 0; j < m_clusters.size(); j++) {
                m_membership[i][j] = m_membership[i][j] / sum2;
            }
        }

        reindexSignals();
    }

    int iter = 0;
    while (iter <= m_iterationLimit) {
        iter++;
        m_objective_function = calculateObjectiveFunction();

        calculateClusterCenters();

        for (int j = 0; j < m_clusters.size(); j++) {
            for (int i = 0; i < m_signalData.size(); i++) {
                double top = calculateDistance(m_signalData[i], m_clusters[j].getCentriod());
                if (top < m_epsilon)
                    top = m_epsilon;

                double sumTerms = 0;
                for (int k = 0; k < m_clusters.size(); k++) {
                    double distance  = calculateDistance(m_signalData[i], m_clusters[k].getCentriod());
                    if (distance < m_epsilon)
                        distance = 1;
                    sumTerms += pow(top / distance, 2.0 / (m_fuzzifier - 1.0));
                }
                m_membership[i][j] = pow (sumTerms , -1.0) ;
            }
        }

        reindexSignals();

        double jkNew = calculateObjectiveFunction();

        std::cout << "Iteration: " << iter << std::endl;
        std::cout << "J: " << m_objective_function << std::endl;
        std::cout << "New J: " << jkNew << std::endl;
        std::cout << "Error: " << fabs(jkNew - m_objective_function) << std::endl;
        std::cout << "Epsilon: " << m_epsilon << std::endl;

        if (fabs(jkNew - m_objective_function) < m_epsilon) {
            break;
        }
    }

    signalData = m_signalData;

    return m_membership;
}

std::vector<Cluster> ClusterDataSet::getClusters(void) {
    return m_clusters;
}

ClusterDataSet::ClusterDataSet(const unsigned numOfClusters) {
    // Create numOfCluster new clusters for data set
    m_numOfClusters = numOfClusters;
    for (int i = 0; i < m_numOfClusters; i++) {
        m_clusters.push_back(Cluster());
    }
}

ClusterDataSet::ClusterDataSet(const std::vector<Cluster> &clusters) {
    // Set the vectors to the private variable for later use
    m_clusters = clusters;
}