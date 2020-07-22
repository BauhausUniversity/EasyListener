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
#include "envelope_follower.h"
#include <stdlib.h>

void EnvelopeFollower::process(const int16_t *data)
{
  if( !data){
    return;
  }

  for (int i=0; i < AUDIO_BLOCK_SAMPLES; i++){
    _abs = abs(data[i]);
    if(_abs > _peak){
      _peak = _abs;
    }
  }
  _peak = _peak * _decay;
}

float EnvelopeFollower::read(){
  return (float)_peak / (float)32767.0;
}

void EnvelopeFollower::setDecayRate(float decay){
  _decay = decay;
}
