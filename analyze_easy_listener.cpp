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
#include "analyze_easy_listener.h"
#include "fft_average.h"
#include "envelope_detector.h"
#include "kMeans.h"
#include <string>
using namespace std;

AudioAnalyzeEasyListener::AudioAnalyzeEasyListener(float loudness_threshold, float envelope_decay, int min_length_in_blocks, bool logFFT):
    AudioStream(1, inputQueueArray),
    _new_sound_classified(false),
    _frame_num(128)
{
  _featureExtractor = new FFTAverage();
  _eventDetector = new EnvelopeDetector(loudness_threshold, envelope_decay, min_length_in_blocks);
  _classifier = new KMeans();
  if(logFFT)
  {
    _FFT256F32 = new FFT256F32();
  }
  else _FFT256F32 = nullptr;
}

AudioAnalyzeEasyListener::~AudioAnalyzeEasyListener()
{
  delete(_featureExtractor);
  delete(_eventDetector);
  delete(_classifier);
}

void AudioAnalyzeEasyListener::update(){

  //if(_featureExtractor->newFeatureDataAvailable()) return;

  audio_block_t *block = receiveReadOnly(0);
  
  if( !block)   return;
  _frame_num = (_frame_num+1)%128+128;
  _eventDetector->process(block->data, _frame_num);

  if(_eventDetector->eventOngoing())
  {
    _featureExtractor->process(block->data, _frame_num);
  }
  if(_FFT256F32)
  {
      _FFT256F32->process(block->data, _frame_num);
  }
  
  release(block);
}

float AudioAnalyzeEasyListener::getScore()
{
  return _classifier->getCurrentScore();
}


bool AudioAnalyzeEasyListener::available()
{  
  bool flag = _new_sound_classified;
  if (flag){
    _new_sound_classified = false;
  }
  return flag;
}

int AudioAnalyzeEasyListener::getNumberOfClasses()
{
  return _classifier->getNumberOfClasses();
}

void AudioAnalyzeEasyListener::listenToExample(int class_id)
{
  _featureExtractor->setCurrentFeatureVector();
    
  _sound_database_labels.push_back(class_id);
  _sound_database_features.push_back(_featureExtractor->getCurrentFeatureVector());
}

void AudioAnalyzeEasyListener::studyExamples()
{  
  _classifier->train(_sound_database_features, _sound_database_labels);
}

void AudioAnalyzeEasyListener::listenInBackground()
{
  if(_eventDetector->eventValid()) 
  {
    _featureExtractor->setCurrentFeatureVector();     // save collected features to current feature vector
    if(_classifier->classifierIsTrained())
    {
        _classifier->classify(_featureExtractor->getCurrentFeatureVector());
        _new_sound_classified = true;
    }
  }
}

int AudioAnalyzeEasyListener::getSoundID()
{
  return _classifier->getCurrentClassLabel();
}

float AudioAnalyzeEasyListener::getFeature(int i)
{
  return _featureExtractor->getFeature(i);
}

float AudioAnalyzeEasyListener::getNumOfFeatures()
{
  return _featureExtractor->getNumOfFeatures();
}
