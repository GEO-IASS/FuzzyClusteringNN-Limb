# FuzzyClusteringNN-Limb

This project is an implementation of an Multi-Layered Perceptron Neural Network discussed in following publication:
- https://www.researchgate.net/profile/Bekir_Karlik/publication/9006747_A_Fuzzy_Clustering_Neural_Network_Architecture_for_Multifunction_Upper-Limb_Prosthesis/links/5667f45208ae34c89a027fad.pdf

The publication goes into detail of how to build and train a neural network as well as how to implement fuzzy clustering on  a data set.

## How it works
Upon execution, a file is loaded containing training data. An example of the file can be found in the `build/Debug` directory.
The program then executes as follows:
- Parses the file for all input values and creates signal objects based on the data
- Fuzzy Clustering is then executed on the signals created until the margin of error is below threshold.
- The neural network is constructed based on a topology provided in the file loaded.
- The network is then trained with all the data provided
  - Each Signal contains an index to the Signal's cluster
  - The identified cluster is fed to the neural network for training

## Neural Network

The neural network is fed 5 inputs:
  - A four-order AR Model
  - Signal power

The neural network returns 6 outputs:
  - The probability that the Signal provided corresponds with one of six upper-limb motions
    - Resting
    - Wrist Supination
    - Grasp
    - Wrist Pronation 
    - Elbow extension
    - Elbow flexion 

The network is fed this information through a single hidden layer (You can use multiple hidden layers by adding another layer to the `topology` line in the training file).
More information on the network's cost and activation functions can be found in the linked article.
 
## Clustering
The clustering algorithm used is the Fuzzy C-Means algorithm.
- The fuzzifier coefficient is set to 2.
- The margin of error is set to 0.1

More information about the Fuzzy C-Means algorithm can be found:
- https://en.wikipedia.org/wiki/Fuzzy_clustering#Fuzzy_C-means_Clustering

## How to execute
This repository contains the executable along with a sample training data file.
If you would like to build yourself, ensure that the training file is located in the same directory as the executable.
I have also created a supplement program that generates input for this program that can be found:
- https://github.com/rinichulous/NNSampleDataGenerator
