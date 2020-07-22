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
#ifndef analyze_envelope_follower_h_
#define analyze_envelope_follower_h_

#include "core_pins.h"
#include "AudioStream.h"

class EnvelopeFollower{

  public:
    EnvelopeFollower(float decay = 0.95):
    _decay(decay)
    { };
      
    void process(const int16_t *data);
    float read();
    void setDecayRate(float decay);
          
  private:
    int16_t *data;
    int16_t _abs;
    int16_t _peak;
    float _decay;
};

#endif
