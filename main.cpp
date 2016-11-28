#include <vector>
#include <iostream>
#include <string>
#include "TrainingData.h"
#include "Network.h"
#include "ClusterDataSet.h"
#include "Signal.h"


int main() {

    // There will be n = 6 clusters (One for each motion we'd like to identify)
    ClusterDataSet fuzzyDataSet(6);

    TrainingData trainData("/Users/nick.lofton/Documents/CIS 667/trainingDataFCNNAll.txt");
    std::vector<double> inputVals, targetVals, resultVals;
    std::vector<Signal> signalData;

    while (!trainData.isEof()) {
        trainData.getNextInputs(inputVals);
        if (inputVals.size() > 0)
            signalData.push_back(Signal(inputVals));
    }

    std::vector<std::vector<double>>  membership;

    fuzzyDataSet.fuzzyCMeansClustering(signalData);

    std::vector<unsigned> topology;
    trainData.getTopology(topology);
    Net myNet(topology);
    int trainingPass = 0;

    for (int i = 0; i < signalData.size(); i++)
    {
        //std::cout << "Signal Value: " << signalData[i].toString() << " ";
        //std::cout << "ClusterIndex: " << signalData[i].getClusterIndex() << std::endl;

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

        // std::cout << std::endl << "Training network" << std::endl;
        myNet.backProp(targetVals);

        // Report how the training is working, averaged
        std::cout << "Net recent average error: " << myNet.getRecentAverageError() << std::endl;
        /*for (int j = 0; j < 6; j++)
        {
            std::cout << "Cluster: " << fuzzyDataSet.getClusters()[j].getCentriod().toString() << " ";
            std::cout << "Membership Value: " << membership[i][j] << std::endl;
        }*/
    }

    std::cout << std::endl << "Done: Training Complete";
    std::cout << std::endl << "We can not begin testing....";
    bool running = true;

    while (running) {

        std::cout << std::endl << "Please enter 5 double values (spaced):" << std::endl;
        std::string line;
        std::getline(std::cin, line);
        std::istringstream buf(line);
        std::istream_iterator<std::string> beg(buf), end;

        std::vector<std::string> tokens(beg, end); // done!
        inputVals.clear();
        for(auto& s: tokens) {
            std::cout << s << std::endl;
            if (s == "done") {
                running = false;
            } else {
                inputVals.push_back(std::atof(s.c_str()));
            }
        }

        if (running) {
            // recluster
            signalData.push_back(Signal(inputVals));
            fuzzyDataSet.fuzzyCMeansClustering(signalData);

            int i = signalData.size() - 1;
            int j = signalData[i].getClusterIndex();
            trainData.showVectorVals("Inputs :", signalData[i].getSignal());
            trainData.showVectorVals("Cluster Inputs :", fuzzyDataSet.getClusters()[j].getCentriod().getSignal());
            myNet.feedForward(inputVals);

            // Collect the net's actual results
            myNet.getResults(resultVals);
            trainData.showVectorVals("Outputs :", resultVals);
        }
    }
}