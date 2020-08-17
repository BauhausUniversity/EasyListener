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
#ifndef feature_extractor_h_
#define feature_extractor_h_
#include "PointND.h"

class FeatureExtractor 
{
	public:
		virtual float getFeature(unsigned int feature_idx);
		virtual int16_t getNumOfFeatures();
		virtual void process(int16_t  *audio_data);
		virtual bool newFeatureDataAvailable(); // check for new sound feature data
		virtual PointND getCurrentFeatureVector();
		virtual void setCurrentFeatureVector();

		virtual ~FeatureExtractor(){}
};
#endif