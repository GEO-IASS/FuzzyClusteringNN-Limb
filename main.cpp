#include <vector>
#include <iostream>
#include "TrainingData.h"
#include "Network.h"
#include "ClusterDataSet.h"

int main() {

    // There will be n = 6 clusters (One for each motion we'd like to identify)
    ClusterDataSet fuzzyDataSet(6);

    TrainingData trainData("generatedTrainingData.txt");
    std::cout << "Loaded Training File" << std::endl;

    std::vector<double> inputVals, targetVals, resultVals;
    std::vector<Signal> signalData;

    std::cout << "Setting Signal Data" << std::endl;

    while (!trainData.isEof()) {
        trainData.getNextInputs(inputVals);

        if (inputVals.size() > 0) {
            signalData.push_back(Signal(inputVals));
        }
    }

    std::cout << "Signal Data Set" << std::endl;

    std::vector<std::vector<double>>  membership;

    std::cout << "Clustering Signal Data" << std::endl;

    /* For debugging purposes
    fuzzyDataSet.setInitialClusters(signalData);
    for (int i = 0; i < fuzzyDataSet.getClusters().size(); i++) {
        trainData.showVectorVals("Initial Cluster Inputs :", fuzzyDataSet.getClusters()[i].getCentriod().getSignal());
    }*/

    fuzzyDataSet.fuzzyCMeansClustering(signalData);

    std::cout << "Signal Data Clustered" << std::endl;

    std::cout << "Press Any Key to begin training" << std::endl;
    std::cin.ignore();

    std::cout << "Beginning Training...." << std::endl;

    /* For debugging purposes
    for (int i = 0; i < fuzzyDataSet.getClusters().size(); i++) {
        trainData.showVectorVals("Current Cluster Inputs :", fuzzyDataSet.getClusters()[i].getCentriod().getSignal());
    }*/

    std::vector<unsigned> topology;
    trainData.getTopology(topology);
    Net myNet(topology);
    int trainingPass = 0;

    for (int i = 0; i < signalData.size(); i++)
    {
        /* For debugging purposes
        std::cout << "Signal Value: " << signalData[i].toString() << " ";
        std::cout << "ClusterIndex: " << signalData[i].getClusterIndex() << std::endl;*/

        // Get new input data and feed it forward
        if (trainData.getNextInputs(inputVals) != topology[0]) {
            break;
        }

        int j = signalData[i].getClusterIndex();
        trainData.showVectorVals("Inputs :", signalData[i].getSignal());
        trainData.showVectorVals("Cluster Inputs :", fuzzyDataSet.getClusters()[j].getCentriod().getSignal());
        myNet.feedForward(fuzzyDataSet.getClusters()[j].getCentriod().getSignal());

        // Collect the net's actual results
        myNet.getResults(resultVals);
        trainData.showVectorVals("Outputs :", resultVals);

        // Train the net what the outputs should have been
        trainData.getTargetOutputs(targetVals);
        trainData.showVectorVals("Targets :", targetVals);
        assert(targetVals.size() == topology.back());

        myNet.backProp(targetVals);

        // Report how the training is working, averaged
        std::cout << "Net recent average error: " << myNet.getRecentAverageError() << std::endl;

        /* For debugging purposes
        for (int j = 0; j < 6; j++)
        {
            std::cout << "Cluster: " << fuzzyDataSet.getClusters()[j].getCentriod().toString() << " ";
            std::cout << "Membership Value: " << membership[i][j] << std::endl;
        }*/
    }
}