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
#include "envelope_detector.h"
#include <stdlib.h>

void EnvelopeDetector::process(const int16_t *data)
{
  if( !data) return;
  if (_new_sound_detected) return;

  // detect peak of buffer
  for (int i=0; i < AUDIO_BLOCK_SAMPLES; i++)
  { 
    _envelope = abs(data[i]);
    if(_envelope > _peak){
      _peak = _envelope;
    }
  }

  // detect if event is "ongoing"
  if (_peak > _threshold){
    _threshold_crossed = true;
  }
  else _threshold_crossed = false;
  
  _peak = _peak * _decay;

  // detect if event is valid
  if (_threshold_crossed) _block_count++;

  if(_block_count > _min_number_of_blocks)
  {
    _sound_is_long_enough = true;
  } else 
  {
    _sound_is_long_enough = false;
  }

  if(!_threshold_crossed && _sound_is_long_enough)
  {
    _new_sound_detected = true;
  }
  // reset
  if (!_threshold_crossed) _block_count = 0;

  
}

void EnvelopeDetector::setDecayRate(float decay){
  _decay = decay;
}
void EnvelopeDetector::setThreshold(float threshold){
  _threshold = dbToa(threshold)*(float)32767.0;
}