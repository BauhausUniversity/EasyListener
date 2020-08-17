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
  AudioAnalyzeEasyListener(float loudness_threshold = -30, float envelope_decay  = 0.95, int min_length_in_blocks = 60);
  ~AudioAnalyzeEasyListener();

  virtual void update(void); // collect feature data for single frame, called automatically by system interupt
  
  int getSoundID(); // returns the class label of last detected sound
  bool available(); // check for new sound snippet, clears on read
  void listenToExample(int class_id); // collect example sound for the given class id
  void studyExamples(); // train the classifier
  void listenInBackground(); // analyzes and classifies the audio stream in the background, returns score of detected sound
  int getNumberOfClasses();
  float getScore(); // return classification score of current detected sound [0..1]

  FeatureExtractor* _featureExtractor;
  EventDetector* _eventDetector;
  Classifier* _classifier;

private:
  audio_block_t *inputQueueArray[1];

  std::vector<PointND> _sound_database_features;
  std::vector<int>     _sound_database_labels;
  bool _new_sound_classified;
};
#endif