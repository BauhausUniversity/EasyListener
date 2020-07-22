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
  *  Teensy audio shield microphone input which acts 
  *  as a sound gestural interface. 
  */

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <analyze_easy_listener.h>

AudioInputI2S            in;
AudioOutputI2S           out;
AudioControlSGTL5000     sgtl5000_1;
AudioAnalyzeEasyListener easyListener;
AudioConnection          patchCord1(in, 0, out, 0);
AudioConnection          patchCord2(in, 1, out, 1);
AudioConnection          patchCord4(in, 0, easyListener, 0);

void setup(){
  Serial.begin(9600);
  
  AudioMemory(16);
  sgtl5000_1.enable();
  sgtl5000_1.inputSelect(AUDIO_INPUT_MIC);
  sgtl5000_1.micGain(24);
  sgtl5000_1.volume(0.8);
  
  int number_of_examples = 3;
  int number_of_classes = 2;
  
  Serial.println("Collecting data. \nTo train the algorithm please provide example sounds!\n");

  for (int class_id=0; class_id<number_of_classes; class_id++)
  {
    Serial.println("Provide "+ String(number_of_examples) + " sound examples for class " + String(class_id));
    for (int i=0; i<number_of_examples; i++)
    {
      easyListener.listenToExample( class_id );
      Serial.println("New sound detected!");
    }
  }
  Serial.println();
  
  Serial.println("Calculating Centroids\nNumber of classes: " + String(easyListener.getNumberOfClasses()));
  easyListener.studyExamples();
  Serial.println("\nSetup successful!\nTeensy listens to the trained sound gestures now.\n");
}


void loop()
{
  easyListener.listenInBackground();
  
  if(easyListener.available() && easyListener.getScore()>0.80)
  {
    Serial.println("Inferred class is "+ String(easyListener.getSoundID())+ ". Score: " + String(easyListener.getScore()));
  }
}
