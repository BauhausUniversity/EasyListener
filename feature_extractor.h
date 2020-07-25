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

class FeatureExtractor 
{
	public:
		virtual float read(unsigned int feature_idx);
		virtual void process(int16_t  *data);

		virtual ~FeatureExtractor(){}
};
#endif