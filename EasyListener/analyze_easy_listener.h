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
#include "envelope_follower.h"
#include <vector>
#include "PointND.h"
#include "KMeansN.h"
#include "fft256_f32.h"

extern "C"{
  __attribute__((weak)) int __exidx_start(){ return -1;}
  __attribute__((weak)) int __exidx_end(){ return -1; }
}


class AudioAnalyzeEasyListener : public AudioStream
{
public:
  AudioAnalyzeEasyListener(float loudness_threshold = -40, float envelope_decay  = 0.95, int min_length_in_blocks = 60);
  
  virtual void update(void); // collect feature data for single frame, called automatically by system interupt
  
  int getSoundID(); // returns the class label of last detected sound
  bool available(); // check for new sound snippet, clears on read
  void listenToExample(int class_id); // collect example sound for the given class id
  void studyExamples(); // train the classifier
  void listenInBackground(); // analyzes and classifies the audio stream in the background, returns score of detected sound
  int getNumberOfClasses();
  float getScore(); // return classification score of current detected sound [0..1]

  // utility functions
  static float todb(const float a);
  
private:

  void detectSoundSnippet(); /* detect a sound snippet from a number of frames 
  and save its feature data in "Sound _current_sound" 
  called whenever _current_sound is read */

  bool feature_data_available(); // check for new sound feature data
  bool soundSnippetAvailable(); // check if new sound snipped is available
  
  audio_block_t *inputQueueArray[1];
  FFT256F32 _fft;
  EnvelopeFollower _envelopeFollower;
  float _loudness_threshold;
  float _envelope_decay;
  int _fft_size;
  int _n_features;
  std::vector<float> _fft_sum;
  int _block_count;
  int _min_number_of_blocks;
  PointND _current_feature_vector;
  int _number_of_classes;
  int _current_class_label;
  std::vector<PointND> _sound_database_features;
  std::vector<int>     _sound_database_labels;
  volatile bool _new_feature_data_available;
  bool _new_snippet_detected;
  bool _new_sound_classified;
  KMeansN _kMeans;
  float _score;
};


#endif
