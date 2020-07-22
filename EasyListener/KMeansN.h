/*
 * EasyListener for Teensy Audio Library
 * (c) 2020 Clemens Wegener / Bauhaus-Universität Weimar
 *
 * This library uses machine learning to detect previously 
 * presented audio signals on the Teensy board. 
 *
 * BSD license
 */
#ifndef KMeansN_H
#define KMeansN_H
#include "PointND.h"
#include <vector>
#include <string>
using namespace std;

class KMeansN{
  public:
    KMeansN(int num_of_classes, vector<PointND> &pointVec, string initializeMethod);
    KMeansN();
    
    void initialize(int num_of_classes, vector<PointND> &pointVec, string initializeMethod = "spread");
    void initializeWithClassLabels(int num_of_classes, vector<PointND> &pointVec, vector<int> classLabels, string initializeMethod = "spread");
    void cluster(int n_iterations);
    void clusterStep();
    int getClassLabel(int idx_to_point);
    int classify(PointND p);
    int getNumberOfClasses() {return _n_classes;}
    PointND getCentroid(int class_label);
    float calculateScore(PointND p, int class_label);

  private:
    void initializeCentroidsNaiv();
    void initializeCentroidsMaxSpread();
    void assignClassLabels();
    void updateCentroids();
    vector<PointND> _points;
    int _n_classes;
    int _n_dim;
    int _n_points;
    vector<int> _n_class_instances;
    vector<int> _classLabels;
    vector<PointND> _centroids;
};

#endif
