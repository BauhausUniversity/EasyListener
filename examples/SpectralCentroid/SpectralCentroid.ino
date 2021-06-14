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
 /* 
  *  This example uses a piezo disc connected to the 
  *  Teensy audio shield microphone input, which acts 
  *  as a sound gestural interface. You can record
  *  scratches and taps, e.g. on your desk and recognize
  *  them by the acoustic footprint of the structure
  *  borne sound.
  */
  
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <analyze_easy_listener.h>
#include "spectral_centroid.h"

float loudness_threshold = -45;
float envelope_decay_rate  = 0.95; 
int min_sound_length_in_blocks = 20;

int number_of_examples = 3;
int number_of_classes = 3;
float min_score = 0.60; // resembles probability of true classification between 0.0 and 1.0

String class_names[]
{
  "grumpy cat", 
  "knock",
  "scratch"
};

AudioInputI2S            in;
AudioOutputI2S           out;
AudioControlSGTL5000     sgtl5000_1;
AudioFilterBiquad        highpass; // highpass filter to remove dc offset
AudioAnalyzeEasyListener easyListener(loudness_threshold, envelope_decay_rate, min_sound_length_in_blocks, true);
AudioConnection          patchCord1(highpass, 0, out, 0);
AudioConnection          patchCord2(highpass, 0, out, 1);
AudioConnection          patchCord3(in, 0, highpass, 0);
AudioConnection          patchCord4(highpass, 0, easyListener, 0);

void setup()
{
  Serial.begin(115200);
  AudioMemory(128);
  sgtl5000_1.enable();
  sgtl5000_1.inputSelect(AUDIO_INPUT_MIC);
  sgtl5000_1.micGain(24);
  sgtl5000_1.volume(0.8);
  highpass.setHighpass(0, 200, 0.7071);
  highpass.setHighpass(1, 200, 0.7071);
  // delete the default feature extractor
  delete(easyListener._featureExtractor);
  // create a spectral centroid extractor and let easyListener point to it
  easyListener._featureExtractor = new SpectralCentroid();
  // wait for first event to be detected and "throw it away"
  // it is invalid and gets triggered due to noise when resetting the chip
  while(!easyListener._eventDetector->eventValid()) {};
}

void loop()
{
  easyListener.listenInBackground();
  if( easyListener._featureExtractor->newFeatureDataAvailable() )
  {
    float spectralCentroid = easyListener._featureExtractor->getFeature(0)*10;
    Serial.println(spectralCentroid);
  }
}
