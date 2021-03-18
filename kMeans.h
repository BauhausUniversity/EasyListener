/*
 * EasyListener for Teensy Audio Library
 * (c) 2020 Clemens Wegener / Bauhaus-Universität Weimar
 *
 * This library uses machine learning to detect previously 
 * presented audio signals on the Teensy board. 
 *
 * BSD license
 */
#ifndef kMeans_h_
#define kMeans_h_
#include "PointND.h"
#include <vector>
#include <string>
#include "classifier.h"
using namespace std;

class KMeans : public Classifier 
{
  public:
    KMeans(int num_of_classes, vector<PointND> &pointVec, string initializeMethod);
    KMeans();
    
    void train(vector<PointND> &pointVec, vector<int> classLabels);
    bool classifierIsTrained(){ return _isTrained;}
    void cluster(int n_iterations);
    void clusterStep();
    int getClassLabel(int idx_to_point);
    int getCurrentClassLabel();
    float getCurrentScore();
    void classify(PointND p);
    int getNumberOfClasses() {return _n_classes;}
    PointND getCentroid(int class_label);
    void calculateScore(PointND p, int class_label);

  private:
    void initialize(int num_of_classes, vector<PointND> &pointVec, string initializeMethod = "spread");
    void initializeCentroidsNaiv();
    void initializeCentroidsMaxSpread();
    void assignClassLabels();
    void updateCentroids();
    void countNumOfClasses();
    void initializeClassesAndLabels(vector<PointND> &pointVec, vector<int> classLabels);

    vector<PointND> _points;
    int _n_classes;
    int _n_dim;
    int _n_points;
    int _current_class_label;
    float _current_score;
    vector<int> _n_class_instances;
    vector<int> _classLabels;
    vector<PointND> _centroids;
    bool _isTrained;
};

#endif
