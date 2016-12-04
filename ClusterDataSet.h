//
// Created by Nick Lofton on 11/15/16.
//

#ifndef NEURALNETWORK_CLUSTEREDDATASET_H
#define NEURALNETWORK_CLUSTEREDDATASET_H

#include <vector>
#include "Cluster.h"
#include "Signal.h"

class ClusterDataSet {
public:
    ClusterDataSet(const std::vector<Cluster> &clusters);
    ClusterDataSet(const unsigned numOfClusters);
    void addCluster(Cluster &newCluster);
    std::vector<std::vector<double>> fuzzyCMeansClustering(std::vector<Signal> &signalData, bool intializeClusters = true);
    std::vector<Cluster> getClusters(void);
    void setInitialClusters(std::vector<Signal> &signalData);
private:
    void setInitialClusters(void);
    void reindexSignals(void);
    double calculateDistance(Signal point, Signal centroid);
    double calculateObjectiveFunction(void);
    void calculateClusterCenters(void);
    std::vector<Cluster> m_clusters; // m_cluster[clusterNum]
    std::vector<Signal> m_signalData;
    std::vector<std::vector<double>> m_membership;
    int m_numOfClusters;
    int m_fuzzifier;
    double m_epsilon;
    int m_iterationLimit;
    double m_objective_function;


};


#endif //NEURALNETWORK_CLUSTEREDDATASET_H
