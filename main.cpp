#include <vector>
#include <iostream>
#include "TrainingData.h"
#include "Network.h"
#include "ClusterDataSet.h"

int main() {


    // There will be n = 6 clusters (One for each motion we'd like to identify)
    ClusterDataSet fuzzyDataSet(6);
    TrainingData trainData("generatedTrainingData.txt");
    std::vector<double> inputVals, targetVals, resultVals;
    std::vector<Signal> signalData;
    std::vector<std::vector<double>>  membership;
    std::vector<unsigned> topology;
    double learningRate;
    double momentum;

    std::cout << "Loaded Training File" << std::endl;
    std::cout << "Getting configurations..." << std::endl;

    fuzzyDataSet.setFuzzifier(trainData.getFuzzifier());
    fuzzyDataSet.setErrorMargin(trainData.getErrorMargin());
    fuzzyDataSet.setIterationLimit(trainData.getIterLimit());
    trainData.getTopology(topology);
    learningRate = trainData.getLearningRate();
    momentum = trainData.getMomentum();

    std::cout << "Setting Signal Data" << std::endl;

    while (!trainData.isEof()) {
        trainData.getNextInputs(inputVals);

        if (inputVals.size() > 0) {
            signalData.push_back(Signal(inputVals));
        }
    }

    std::cout << "Signal Data Set" << std::endl;


    std::cout << "Clustering Signal Data" << std::endl;

    // For debugging purposes
    fuzzyDataSet.setInitialClusters(signalData);
    for (int i = 0; i < fuzzyDataSet.getClusters().size(); i++) {
        trainData.showVectorVals("Initial Cluster Inputs :", fuzzyDataSet.getClusters()[i].getCentriod().getSignal());
    }

    membership = fuzzyDataSet.fuzzyCMeansClustering(signalData);

    std::cout << "Signal Data Clustered" << std::endl;

    // For debugging purposes
    for (int i = 0; i < fuzzyDataSet.getClusters().size(); i++) {
        trainData.showVectorVals("Current Cluster Inputs :", fuzzyDataSet.getClusters()[i].getCentriod().getSignal());
    }

    std::cout << "Press Enter to begin training" << std::endl;
    std::cin.ignore();

    Net myNet(topology, learningRate, momentum);
    int trainingPass = 0;

    std::cout << "Beginning Training...." << std::endl;

    for (int iter = 0; iter < 500; iter++) {
        trainData.resetEOF();
        double target = -1;

        for (int i = 0; i < signalData.size(); i++) {

            // Get new input data and feed it forward
            while (trainData.getNextInputs(inputVals) != topology[0]) {
                trainData.getNextInputs(inputVals);
            }

            int j = signalData[i].getClusterIndex();
            myNet.feedForward(fuzzyDataSet.getClusters()[j].getCentriod().getSignal());

            // Collect the net's actual results
            myNet.getResults(resultVals);

            // Train the net what the outputs should have been
            trainData.getTargetOutputs(targetVals);
            assert(targetVals.size() == topology.back());

            myNet.backProp(targetVals);
        }
        if (iter > 1 && iter % 100 == 0) {
            std::cout << "Iteration: " << iter << std::endl;
            // Report how the training is working, averaged
            std::cout << "Net recent average error: " << myNet.getRecentAverageError() << std::endl;
        }

    }

    std::cout << "Training Complete. Press Enter to start live test" << std::endl;
    std::cin.ignore();

    TrainingData liveData("liveData.txt");
    double success = 0;
    double pass = 0;

    while (!liveData.isEof()) {
        pass++;

        // Get new input data and feed it forward
        if (liveData.getNextInputs(inputVals) != topology[0]) {
            break;
        }


        signalData.push_back(Signal(inputVals));
        liveData.showVectorVals("Inputs :", signalData.back().getSignal());
        myNet.feedForward(fuzzyDataSet.findCluster(Signal(inputVals)).getSignal());

        // Collect the net's actual results
        myNet.getResults(resultVals);
        liveData.showVectorVals("Outputs :", resultVals);

        // Train the net what the outputs should have been
        liveData.getTargetOutputs(targetVals);
        liveData.showVectorVals("Targets :", targetVals);
        assert(targetVals.size() == topology.back());


        double target = -1;

        for (int i = 0; i < targetVals.size(); i++ ) {
            if (targetVals[i] != 0) {
                target = i;
                break;
            }
        }

        double networkOutput = -1;
        double result = -1;

        for (int i = 0; i < resultVals.size(); i++ ) {
            if (resultVals[i] > networkOutput) {
                networkOutput = resultVals[i];
                result = i;
            }
        }

        if (target == result && networkOutput >= 0.97) {
            success++;
        }
    }
    // Report how the training is worked, live success rate averaged
    std::cout << "Average error (Output >= 0.97): " << success / pass << std::endl;
}