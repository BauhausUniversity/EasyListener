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
#ifndef analyze_easy_listener_h_
#define analyze_easy_listener_h_

#include "Arduino.h"
#include "AudioStream.h"
#include "event_detector.h"
#include "feature_extractor.h"
#include "fft256_f32.h"
#include "classifier.h"
#include <vector>
#include "PointND.h"


extern "C"{
  __attribute__((weak)) int __exidx_start(){ return -1;}
  __attribute__((weak)) int __exidx_end(){ return -1; }
}


class AudioAnalyzeEasyListener : public AudioStream
{
public:
  AudioAnalyzeEasyListener(float loudness_threshold = -30, float envelope_decay  = 0.95, int min_length_in_blocks = 60, bool logFFT = false);
  ~AudioAnalyzeEasyListener();

  virtual void update(void); // collect feature data for single frame, called automatically by system interupt
  
  int getSoundID(); // returns the class label of last detected sound
  bool available(); // check for new sound snippet, clears on read
  void listenToExample(int class_id); // collect example sound for the given class id
  void studyExamples(); // train the classifier
  void listenInBackground(); // listens to detected events from event detector and classifies them
  int getNumberOfClasses();
  float getScore(); // return classification score of current detected sound [0..1]
  float getFeature(int i);
  float getNumOfFeatures();
  uint16_t getFrameNum() {return _frame_num;}
  
  int16_t getNumOfFFTBins();
  float getFFTBin(unsigned int idx) ;

  FeatureExtractor* _featureExtractor;
  EventDetector* _eventDetector;
  Classifier* _classifier;
  FFT256F32* _FFT256F32;

private:
  audio_block_t *inputQueueArray[1];

  std::vector<PointND> _sound_database_features;
  std::vector<int>     _sound_database_labels;
  bool _new_sound_classified;
  uint16_t _frame_num;
};
#endif
