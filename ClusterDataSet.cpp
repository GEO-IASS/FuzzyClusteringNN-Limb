//
// Created by Nick Lofton on 11/15/16.
//

#include "ClusterDataSet.h"
#include <cmath>
#include <iostream>

void ClusterDataSet::setInitialClusters() {
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
            randomSignalIndex = rand() % m_signalData.size();
            currentSignal = m_signalData[randomSignalIndex].getSignal();
            for (int i = 0; i < clusters.size(); i++) {
               if (clusters[i] == currentSignal) {
                   duplicate = true;
                   break;
               }
            }
        }

        clusters.push_back(currentSignal);
        m_clusters[clusterIndex].setCentriod(m_signalData[randomSignalIndex]);
    }
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
    double jk = 0;

    for (int i = 0; i < m_signalData.size(); i++) {
        for (int j = 0; j < m_clusters.size(); j++) {
            jk += pow(m_membership[i][j], m_fuzzifier) * pow(calculateDistance(m_signalData[i], m_clusters[j].getCentriod()), 2);
        }
    }
    return jk;
}

void ClusterDataSet::calculateClusterCenters() {
    for (int j = 0; j < m_clusters.size(); j++) {
        double l = 1;
        std::vector<double> newCentroid;

        newCentroid.resize(m_clusters[j].getCentriod().getSignal().size());
        for (int i = 0; i < m_signalData.size(); i++) {
            double uu = pow(m_membership[i][j], m_fuzzifier);
            for (int k = 0; k < m_clusters[j].getCentriod().getSignal().size(); k++) {
                newCentroid[k] += uu * m_clusters[j].getCentriod().getSignal()[k];
            }
            l += uu;
        }

        for (int k = 0; k < m_clusters[j].getCentriod().getSignal().size(); k++) {
            m_clusters[j].getCentriod().getSignal()[k] = newCentroid[k] / l;
        }
    }
}

std::vector<std::vector<double>> ClusterDataSet::fuzzyCMeansClustering(std::vector<Signal> &signalData) {
    // Run Fuzzy C-Means on entire data set to create clusters
    // Choose:
    // the number of clusters,
    m_numOfClusters;
    // weighting exponent,
    m_fuzzifier = 2;
    // iteration limit (iter),
    m_iterationLimit = 20;
    // termination criterion and norm for error.
    m_epsilon = 0.15;
    // Signal Data
    m_signalData = signalData;


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

    int iter = 0;
    while (iter <= m_iterationLimit) {
        iter++;
        m_objective_function = calculateObjectiveFunction();

        calculateClusterCenters();

        for (int c = 0; c < m_clusters.size(); c++) {
            for (int h = 0; h < m_signalData.size(); h++) {
                double top = calculateDistance(m_signalData[h], m_clusters[c].getCentriod());
                if (top < 0.05)
                    top = 0.05;

                double sumTerms = 0;
                for (int ck = 0; ck < m_clusters.size(); ck++) {
                    double distance  = calculateDistance(m_signalData[h], m_clusters[ck].getCentriod());
                    if (distance < 0.05)
                        distance = 0.05;
                    sumTerms += pow(top / distance, 2.0 / (m_fuzzifier - 1.0));
                }
                m_membership[h][c] = 1.0 / sumTerms;
            }
        }

        reindexSignals();

        double jkNew = calculateObjectiveFunction();

        if (fabs(m_objective_function - jkNew) < m_epsilon) {
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