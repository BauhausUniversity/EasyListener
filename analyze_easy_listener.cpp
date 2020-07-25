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
#include <string>
using namespace std;

AudioAnalyzeEasyListener::AudioAnalyzeEasyListener(float loudness_threshold, float envelope_decay, int min_length_in_blocks):
    AudioStream(1, inputQueueArray),
    _loudness_threshold(loudness_threshold),
    _envelope_decay(envelope_decay),
    _fft_size(256),
    _n_features(128),
    _block_count(0),
    _min_number_of_blocks(min_length_in_blocks),
    _current_feature_vector(_n_features),
    _number_of_classes(0),
    _current_class_label(-1),
    _new_feature_data_available(false),
    _new_snippet_detected(false),
    _new_sound_classified(false),
    _score(0)
{
  _envelopeFollower.setDecayRate(envelope_decay);
  for(int i=0; i<_n_features; i++)
  {
    _fft_sum.push_back(0);
  }
}


void AudioAnalyzeEasyListener::update(){

  if(_new_feature_data_available) return;

  audio_block_t *block = receiveReadOnly(0);
  
  if( !block)   return;
  
  _envelopeFollower.process(block->data);
  _fft.process(block->data);

  _new_feature_data_available = true;
  
  release(block);
}

float AudioAnalyzeEasyListener::getScore()
{
  return _score;
}
bool AudioAnalyzeEasyListener::soundSnippetAvailable()
{  
  bool flag = _new_snippet_detected;
  if (flag){
    _new_snippet_detected = false;
  }
  return flag;
}


bool AudioAnalyzeEasyListener::feature_data_available()
{  
  __disable_irq();
  bool flag = _new_feature_data_available;
  if (flag){
    _new_feature_data_available = false;
  }
  __enable_irq();
  return flag;
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
  return _number_of_classes;
}

void AudioAnalyzeEasyListener::listenToExample(int class_id)
{

  while(!soundSnippetAvailable()) 
  {
      detectSoundSnippet(); // wait for minimum sound length to be detected
  }

  bool new_class_found = true;
  for(unsigned int i=0; i<_sound_database_labels.size(); i++)
  {
    if(class_id == _sound_database_labels[i])
    {
      new_class_found = false;
      break;
    }
  }
  if(new_class_found) ++_number_of_classes;
    
  _sound_database_labels.push_back(class_id);
  _sound_database_features.push_back(_current_feature_vector);
}

void AudioAnalyzeEasyListener::studyExamples()
{  
  _kMeans.initializeWithClassLabels(_number_of_classes, _sound_database_features, _sound_database_labels, "spread");
  for(int i=0; i<5; i++)
  {    
    _kMeans.clusterStep();
  }
  for(int j=0; j<_number_of_classes; j++)
  {
    PointND centroid = _kMeans.getCentroid(j);
  }
}

float AudioAnalyzeEasyListener::todb(const float a)
{
  return 20.0*log10(a);
}

void AudioAnalyzeEasyListener::listenInBackground()
{
  detectSoundSnippet(); // wait for minimum sound length to be detected
  if(soundSnippetAvailable()) 
  {
    int inferred_class_label = _kMeans.classify(_current_feature_vector);
    _score = _kMeans.calculateScore(_current_feature_vector, inferred_class_label);
    _current_class_label = inferred_class_label;
    _new_sound_classified = true;
  }
}

int AudioAnalyzeEasyListener::getSoundID()
{
  return _current_class_label;
}
void AudioAnalyzeEasyListener::detectSoundSnippet()
{
  if(_new_snippet_detected) return; // previously detected sound was not read!
  if(!feature_data_available()) return; // wait for new sound feature data to arrive
  
  bool threshold_crossed = false;
  
  float amplitude = todb(_envelopeFollower.read());
  if ( amplitude > _loudness_threshold)
  {
    threshold_crossed = true;
  } else 
  {
    threshold_crossed = false;
  }
  
  if (threshold_crossed)
  {
    for(int i=0; i<_n_features; i++)
    {
      _fft_sum[i]+=_fft.read(i);
     }
    _block_count++;
  }
  
  bool sound_is_long_enough = false;
  
  if(_block_count > _min_number_of_blocks)
  {
    sound_is_long_enough = true;
  } else 
  {
    sound_is_long_enough = false;
  }
  
  if(!threshold_crossed) // sound is low amplitude (again)
  {
    if( sound_is_long_enough )
    {
      for(int i=0; i<_n_features; i++)
      {
        _current_feature_vector.setDim(i, todb((_fft_sum[i]+0.0000000000001)/_block_count)); // add a lower bound to loudness, to not take log(0)
      }
      _current_feature_vector.normalize();
      _new_snippet_detected = true;
      std::fill(_fft_sum.begin(),_fft_sum.end(),0);
      _block_count = 0;
    } 
    else
    {
      // reset point because sound is too short
      _new_snippet_detected = false;
      std::fill(_fft_sum.begin(),_fft_sum.end(),0);
      _block_count = 0;
    }
  }
}
