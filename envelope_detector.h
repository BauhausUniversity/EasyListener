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
#ifndef envelope_detector_h_
#define envelope_detector_h_

#include "core_pins.h"
#include "AudioStream.h"
#include "event_detector.h"
#include "Arduino.h"


class EnvelopeDetector : public EventDetector 
{
  public:
    EnvelopeDetector(float threshold_db, float decay, int16_t min_number_of_blocks):
    _envelope(0),
    _peak(0),
    _decay(decay),
    _threshold(dbToa(threshold_db)*(float)32767.0), // convert from db to 16bit fixed point amplitude
    _threshold_crossed(false),
    _sound_is_long_enough(false),
    _new_sound_detected(false),
    _block_count(0),
    _min_number_of_blocks(min_number_of_blocks)
    { };
      
    void process(const int16_t *data);
    void setDecayRate(float decay);
    void setThreshold(float threshold);

    bool eventOngoing() {
        return _threshold_crossed;
    }

    int16_t getEnvelope() {
        return _peak;
    }

    bool eventValid() {
        bool flag = false;
        if (_new_sound_detected)
            flag = true;

        _new_sound_detected = false;
        return flag;
    }

    static float aTodb(const float a) {
        return 20.0*log10f(a);
    }

    static float dbToa(const float db) {
        return powf(10,(db/20.0));
    }

    ~EnvelopeDetector(){}
          
  private:
    int16_t *data;
    int _envelope;
    int _peak;
    float _decay;
    float _threshold;
    bool _threshold_crossed;
    bool _sound_is_long_enough;
    bool _new_sound_detected;
    int16_t _block_count;
    int16_t _min_number_of_blocks;
};

#endif
