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
#include "PointND.h"
#include <cmath>
using namespace std;

PointND::PointND(int dimensions)
{
  for(int i=0; i<dimensions; i++)
  {
    _pointN.push_back(0.0);
  }
}

PointND::PointND(const  vector<float>& newN){
  _pointN = newN;
}

PointND::PointND(const PointND& p2){
  _pointN = p2._pointN;
}

void PointND::push_back(float val)
{
  _pointN.push_back(val);
}

float PointND::getDim(unsigned n_dimension) const
{
  //assert(n_dimension<_pointN.size() && n_dimension>=0)
  if(n_dimension<_pointN.size() && n_dimension>=0)
  {
    return _pointN[n_dimension]; 
  }else
  {
    return -1;
  }
}

void PointND::setDim(unsigned nth_dimension, float value){
  _pointN[nth_dimension] = value;
}


float PointND::getDistance(const PointND &otherPoint){
  if(size() == otherPoint.size()){
    float distance = 0;
    for(unsigned i=0; i<_pointN.size(); i++){
      distance  += (getDim(i) - otherPoint.getDim(i) ) * (getDim(i) - otherPoint.getDim(i) ) ;
    }
    distance = sqrt(distance);

    return distance;
  }else{
    return -1;
  }
}

PointND PointND::operator+(const PointND &p2){
  //asssert(size() == p2.size())
  
  vector<float> vres;
  for(unsigned i=0; i<_pointN.size(); i++)
  {
    vres.push_back( getDim(i) + p2.getDim(i) );
  }
  PointND res(vres);
  return res;
}

PointND PointND::operator/(float f){
  vector<float> vres;
  for(unsigned i=0; i<_pointN.size(); i++){
    vres.push_back(_pointN[i]/f);
  }
  PointND res(vres);
  return res;
}

void PointND::normalize(){
  float max = -32768;
  for(int i=0; i<size(); i++){
    float dim = getDim(i);
    if(max<dim){
      max = dim;
    }
  }
  
  for(int i=0; i<size(); i++){
    setDim(i, getDim(i)-max);
  }
}
