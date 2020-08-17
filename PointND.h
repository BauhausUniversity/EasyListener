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
#ifndef PointND_H
#define PointND_H

#include <vector>
using namespace std;

class PointND {
  public:
    PointND(){};
    PointND(int dimensions);
    PointND(const  vector<float>& newN);
    PointND(const PointND&);

    void push_back(float val);
    PointND operator+(const PointND& p2);
    PointND operator/(float f);
    float getDim(unsigned nth_dimension) const;
    void setDim(unsigned nth_dimension, float value);
    int size() const{
        return _pointN.size();
    }
    float getDistance(const PointND&);
    void normalize();
  
  private:
      vector<float> _pointN;
};
#endif
