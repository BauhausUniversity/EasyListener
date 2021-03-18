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
#ifndef classifier_h_
#define classifier_h_
#include <vector>
#include <string>
using namespace std;
#include "PointND.h"

class Classifier 
{
	public:

	    virtual void train(vector<PointND> &pointVec, vector<int> classLabels);
	    virtual void classify(PointND p);
	    virtual int getCurrentClassLabel();
	    virtual float getCurrentScore();
	    virtual int getNumberOfClasses();
        virtual bool classifierIsTrained();

		virtual ~Classifier(){}
};
#endif
