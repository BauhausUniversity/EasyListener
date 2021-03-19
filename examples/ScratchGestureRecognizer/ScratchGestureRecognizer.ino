/*
 * EasyListener for Teensy Audio Library
 * 
 * (c) 2021 Clemens Wegener 
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

float loudness_threshold = -45;
float envelope_decay_rate  = 0.95; 
int min_sound_length_in_blocks = 20;

String class_names[]
{
  "knock",
  "scratch"
};


AudioInputI2S            in;
AudioOutputI2S           out;
AudioControlSGTL5000     sgtl5000_1;
AudioFilterBiquad        highpass; // highpass filter to remove dc offset
AudioAnalyzeEasyListener easyListener(loudness_threshold, envelope_decay_rate, min_sound_length_in_blocks);
AudioConnection          patchCord1(in, 0, out, 0);
AudioConnection          patchCord2(in, 1, out, 1);
AudioConnection          patchCord3(in, 0, highpass, 0);
AudioConnection          patchCord4(highpass, 0, easyListener, 0);

void setup(){
  Serial.begin(9600);
  
  AudioMemory(16);
  sgtl5000_1.enable();
  sgtl5000_1.inputSelect(AUDIO_INPUT_MIC);
  sgtl5000_1.micGain(24);
  sgtl5000_1.volume(0.8);
  highpass.setHighpass(0, 40, 0.7071);
  highpass.setHighpass(1, 40, 0.7071);
  
  int number_of_examples = 3;
  int number_of_classes = 2;
  
  while(!easyListener._eventDetector->eventValid()) {} // reject first detected sound from switching on the audio codec
  
  Serial.println("Collecting data. \nTo train the algorithm please provide example sounds!\n");

  for (int class_id=0; class_id<number_of_classes; class_id++)
  {
    Serial.println("Provide "+ String(number_of_examples) + " sound examples for class " + class_names[class_id]);
    for (int i=0; i<number_of_examples; i++)
    {
      while(!easyListener._eventDetector->eventValid()) {} // wait for minimum sound length to be detected
      easyListener.listenToExample( class_id );
      Serial.println("New sound detected!");
    }
  }
  Serial.println("\nTraining the classifier\n");
  easyListener.studyExamples();
  Serial.println("Number of classes: " + String(easyListener.getNumberOfClasses()));
  Serial.println("\nSetup successful!\nTeensy listens to the trained sound gestures now.\n");
}


void loop()
{
  easyListener.listenInBackground();
  
  if(easyListener.available() )
  {
    if(easyListener.getScore()>0.90) 
    {
      Serial.println("Inferred sound was "+ class_names[easyListener.getSoundID()]+ ". Score: " + String(easyListener.getScore()));
    }
    else 
    {
      Serial.println("Sound could not be recognized. Score is too low!");
    }
  }
}