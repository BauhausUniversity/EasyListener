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

#ifndef fft_average_h_
#define fft_average_h_
#include <vector>
using namespace std;
#include "fft256_f32.h"
#include "PointND.h"

class FFTAverage : public FeatureExtractor
{
public:
	FFTAverage(): _new_feature_data_available(false), _frame_num(0)
	{
		for(int i=0; i<_fft.getNumOfBins(); i++)
    	{
      		_fft_sum.push_back(0);
    		_current_feature_vector.push_back(0);
    	}
	};

	float getFeature(unsigned int feature_idx) {
		return _fft_sum[feature_idx];
	}


	PointND getCurrentFeatureVector() {
		return _current_feature_vector;
	}

	void setCurrentFeatureVector();

	int16_t getNumOfFeatures() {
		return _fft.getNumOfBins();
	}
	void process(int16_t  *audio_data, uint16_t frame_num);

	void reset();

	bool newFeatureDataAvailable(); // check for new sound feature data
    
    uint16_t getFrameNum() {return _frame_num;}


	virtual ~FFTAverage(){}

private:
	FFT256F32 _fft;
	std::vector<float> _fft_sum;
  	int _block_count;
  	int _min_number_of_blocks;
  	volatile bool _new_feature_data_available;
  	PointND _current_feature_vector;
    uint16_t _frame_num;

};
#endif
