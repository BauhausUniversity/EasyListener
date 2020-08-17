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
#include "fft_average.h"

bool FFTAverage::newFeatureDataAvailable()
{  
  __disable_irq();
  bool flag = _new_feature_data_available;
  if (flag){
    _new_feature_data_available = false;
  }
  __enable_irq();
  return flag;
}

void FFTAverage::process(int16_t  *audio_data)
{  
	_fft.process(audio_data);
	for(int i=0; i<_fft.getNumOfBins(); i++)
    {
      _fft_sum[i]+=_fft.getBin(i);
    }
    _new_feature_data_available = true;
}

void FFTAverage::setCurrentFeatureVector()
{
  for(int i=0; i<getNumOfFeatures(); i++)
  {
    _current_feature_vector.setDim(i, getFeature(i));
    _current_feature_vector.normalize();
  }
  reset();
}

void FFTAverage::reset()
{  
	std::fill(_fft_sum.begin(),_fft_sum.end(),0);
}