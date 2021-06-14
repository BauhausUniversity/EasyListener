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
#ifndef spectral_centroid_h_
#define spectral_centroid_h_
#include "PointND.h"
#include "fft256_f32.h"


class SpectralCentroid : public FeatureExtractor
{
 public:
    SpectralCentroid(){
      _spectral_centroid = 0;
      _frame_num = 0;
      _new_feature_data_available = false;
    }
    
    float getFeature(unsigned int feature_idx){
      return _spectral_centroid;
    }
    int16_t getNumOfFeatures(){
      return 1;
    }
    
    void process(int16_t  *audio_data, uint16_t frame_num)
    {
        _frame_num = frame_num;
        _fft.process(audio_data, frame_num);
        
        float sum_numerator = 0;
        float sum_denominator = 0;
        
        for(int i=0; i<_fft.getNumOfBins(); i++)
        {
            sum_numerator += (float) i / (float) _fft.getNumOfBins() * _fft.getBin(i);
        }
        
        for(int i=0; i<_fft.getNumOfBins(); i++)
        {
            sum_denominator += _fft.getBin(i);
        }
        _spectral_centroid = sum_numerator / sum_denominator;
        _new_feature_data_available = true;
        
    }
    bool newFeatureDataAvailable(){
    {  
        __disable_irq();
        bool flag = _new_feature_data_available;
        if (flag){
            _new_feature_data_available = false;
        }
        __enable_irq();
        return flag;
        }
    } // check for new sound feature data
    
    PointND getCurrentFeatureVector(){
        return PointND(0);
    }
    
    void setCurrentFeatureVector(){}
    
    uint16_t getFrameNum(){
        return _frame_num;
    }
    ~SpectralCentroid(){}

private:
  float _spectral_centroid;
  bool _new_feature_data_available;
  FFT256F32 _fft;
  uint16_t _frame_num;
};
#endif
