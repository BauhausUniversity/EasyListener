/*
 * EasyListener for Teensy Audio Library
 * 
 * (c) 2020 Clemens Wegener 
 * Bauhaus-Universität Weimar
 * Department Interface Design
 *
 * This library uses machine learning to detect previously 
 * presented audio signals on the Teensy board. 
 *
 * BSD license
 */
#include "KMeansN.h"
#include <math.h>
#include <stdlib.h> // for random function "rand"

KMeansN::KMeansN(int num_of_classes, vector<PointND> &pointVec, string initializeMethod):
_points(pointVec), _n_classes(num_of_classes), _n_dim(pointVec[0].size()), _n_points(pointVec.size()){
  /* initialize random seed: */
  //srand (time(NULL));
    
  if (initializeMethod == "naiv") {
    initializeCentroidsNaiv();
  }
  else{
    initializeCentroidsMaxSpread();
  }

  for(int i=0; i<_n_classes; i++){
    _n_class_instances.push_back(0);
  }
  for(int i=0; i<_n_points; i++){
    _classLabels.push_back(0);
  }
}

KMeansN::KMeansN(){
  
}

void KMeansN::initialize(int num_of_classes, vector<PointND> &pointVec, string initializeMethod){
  _n_classes = num_of_classes;   
  _points = pointVec;
  _n_dim = pointVec[0].size(); 
  _n_points = pointVec.size();
  // initialize random seed:
  //srand (time(NULL));
  
  if (initializeMethod == "naiv"){
    initializeCentroidsNaiv();
  }
  else{
    initializeCentroidsMaxSpread();
  }

  for(int i=0; i<_n_classes; i++){
    _n_class_instances.push_back(0);
  }
  for(int i=0; i<_n_points; i++){
    _classLabels.push_back(-1);
  }
}

void KMeansN::initializeWithClassLabels(int num_of_classes, vector<PointND> &pointVec, vector<int> classLabels, string initializeMethod)
{
  // here we set the intial centroids to the center of the class labels that are given by the user 
  _n_classes = num_of_classes;   
  _points = pointVec;
  _n_dim = pointVec[0].size(); 
  _n_points = pointVec.size();

  for(int i=0; i<_n_classes; i++){
    // set to zero, they will be calculated by updateCentroids()
    _n_class_instances.push_back(0);
    _centroids.push_back(PointND(_n_dim));
  }

  for(int i=0; i<_n_points; i++)
  {
    _classLabels.push_back(classLabels[i]);
  }

  updateCentroids();
}

void KMeansN::initializeCentroidsNaiv(){
  // initialize with random controids
  for(int i=0; i<_n_classes; i++){
    // choose the centroids at random from the set of points
    //PointND centroid = _points[rand() % _n_points];
    PointND centroid = _points[i * (_points.size() / _n_classes)];
    _centroids.push_back(centroid); 
  }
}

void KMeansN::updateCentroids()
{
  // calculate centroids
  for (int c=0; c<_n_classes; c++)
  {
    // clear centroids
    _centroids[c] = PointND(_n_dim);
    
    // clear number of class instances
    _n_class_instances[c] = 0;

    for (int p=0; p<_n_points; p++)
    {
        if(c == _classLabels[p])
        {
            _centroids[c] = _centroids[c] + _points[p];
            _n_class_instances[c]++;
        }
    }
    _centroids[c] = _centroids[c] / (float)_n_class_instances[c];
  }
}

void KMeansN::assignClassLabels()
{
  for (int p=0; p<_n_points; p++)
  {
    float min_dist = 32767;

    for( int c=0; c<_n_classes; c++)
    {
      float dist_to_centroid = _centroids[c].getDistance(_points[p]);
      if (dist_to_centroid < min_dist)
      {
        min_dist = dist_to_centroid;
        _classLabels[p] = c;
      }
    }
  }
}

float KMeansN::calculateScore(PointND p, int class_label)
{
  int score_pickiness = 3;
  float distIntraClass = _centroids[class_label].getDistance(p);
  if (distIntraClass==0) 
  {
    return 1;
  }
  else
  {
    float distSumInterClass = 0;
    for( int c=0; c<_n_classes; c++)
    {
      distSumInterClass += powf(1.0/_centroids[c].getDistance(p),score_pickiness);
    }
    float score = powf(1.0 / distIntraClass, score_pickiness) / distSumInterClass;
    return score;
  }
}
int KMeansN::classify(PointND p)
{
  float min_dist = 32767;
  int classLabel = -1;
  float dist_to_centroid = min_dist;
  
  for( int c=0; c<_n_classes; c++)
  {
    dist_to_centroid = _centroids[c].getDistance(p);
    if (dist_to_centroid < min_dist)
    {    
      min_dist = dist_to_centroid;
      classLabel = c; 
    }
  }
//  if(min_dist > 100)
//  {
//    classLabel = min_dist;
//  }
  return classLabel;
}

void KMeansN::cluster(int n_iterations)
{
  for(int i=0; i<n_iterations; i++)
  {
      clusterStep();
  }
}

void KMeansN::clusterStep()
{
  assignClassLabels();
  updateCentroids();
}

int KMeansN::getClassLabel(int idx_to_point)
{
  return _classLabels[idx_to_point];
}

PointND KMeansN::getCentroid(int class_label)
{
  return _centroids[class_label];
}


void KMeansN::initializeCentroidsMaxSpread()
{
  // initialize with random class labels

  // choose first centroid at random
  PointND centroid = _points[rand() % _n_points]; // "%" is modulo operation! look it up if you need to!
  _centroids.push_back(centroid);

  // for every point we calculate the distance to the closest centroid
  // then we choose the point which is furthest away from all centroids
  // for this, we look for a point that maximizes the minimum distance to all other centroids!
  // each time we found a point like this, we make it a new centroid and push it back to our
  // vector of centroids 

  // for every class we want to choose a maximally spread initial centroid
  for(int i=0; i<_n_classes; i++)
  {
      // initialize max distance to be a small value
      float max_away = -32768;
      int max_p = 0; // chose initial max point (this is not a valid max point yet)
      // for every point we want to know the maximum min distance to all centroids we already chose
      for (int p=0; p<_n_points; p++)
      {
        // initialize min distance to be a big value
        float min_dist = 32767;
        // for every centroid that we already chose, we want to know the min distance to our current point
        // here "c" will be only among the centroids we already chose
        for(auto c : _centroids)
        {
          // this calculates the distance between the current point and current centroid
          float dist_to_centroid = c.getDistance(_points[p]);
          // here we ask if we found a centroid that is closer to our current point.
          // the closest centroid is crucial, to be sure that we found a point 
          // that is really far away from ALL centroids!
          if (dist_to_centroid < min_dist)
          {
            min_dist = dist_to_centroid;
          }
        }
        // now we know how close the next centroid to our current point is (min_dist)
        // so we can compare that distance to the centroid distances of the other points, we already found
        // we want to save the point, that is furthest away from all centroids!
        if(min_dist > max_away)
        {
          // save the furthest distance we found so far in max_away
          max_away = min_dist;
          // save the furthest away point from all centroids (until now) in max_p
          max_p = p;
        }
    }
    // we have been looking through all points, 
    // now, we know, we must have found the point, that is furthest away from all centroids!
    // so we push it back to our vector of centroids and then repeat all steps for the next centroid
    // until all centroids for all classes are set
    _centroids.push_back(_points[max_p]);
  }
}
