# EasyListener
A simple and easily extendable machine learning toolset for the Teensy Audio Library.

This library does simple sound recognition with machine learning tools on the Teensy platform. 
It is intended for use with a piezo microphone to recognize acoustic gestures on surfaces, 
but might serve other purposes as well. 

It was tested only on the Teensy 4.0.


## Installation
Copy the folder ```EasyListener``` with the C++ source files to your Arduino Library path. Its in your sketchbook/libraries folder which defaults to:

- ```~/Arduino/libraries``` (linux)
- ```~/Documents/Arduino/libraries``` (osx)
- ```C:\Users\<User Name>\Documents\Arduino\libraries``` (windows)

## How to use
Check out the patch ```ScratchGestureRecognizer``` in the examples folder and take note of the following explanations.
- Include the library with ```#include <analyze_easy_listener.h>```.
- Add the EasyListner object with ```AudioAnalyzeEasyListener easyListener``` and 
- connect it to your audio input with a patchcord ```patchCord1(in, 0, easyListener, 0)```.
- Train a number of examples for each class of sounds with ```easyListener.listenToExample( class_id )```, 
where ```class_id``` refers to one of at least two classes of sounds. 
- With ```easyListener.studyExamples()``` the classifier is trained and then ready to use.
- The function ```easyListener.listenInBackground()``` should be called in the loop() function and is essential 
because it analyzes the sound stream in the backgound. 
- The ```easyListener.available()``` returns true if a new gesture is detected that meets the requirements. 
- Parameters for sound detection can be set in the constructor and have the following defaults:
```AudioAnalyzeEasyListener(float loudness_threshold = -30, float envelope_decay  = 0.95, int min_length_in_blocks = 60);```.

### Displaying Data in Processing
There is a patch called ```ScratchGestureRecognizerLog``` in the examples folder that sends spectral data, detection thresholds and recognition results over serial. 
You'll find the accompanying Processing patch here:

https://github.com/BauhausUniversity/EasyListenerVisualizer

## How it works
The library currently uses the very simple approach of averaging several spectral fft frames over the course of the recorded gesture. 
An envelope follower keeps track of the input loudness and enables the recording of features via FFT analysis. It then trains a KMeans++ classifier with the given feature vectors from the averaged FFT analysis frames. After training the classifier, EasyListener classifies newly recorded gestures and returns the most similar class label of the trained sounds.

The code is structured into three abstract classes:
1. EventDetector (currently implemented as envelope follower with threshold detection)
2. FeatureExtractor (implemented as FFT averaging over the course of a detected input gesture)
3. Classifier (implemented as KMeans clusterer)

These classes can be easily interchanged with your own code when you implement the corresponding abstract classes.
As a starting point, look at the  "envelope_detector.h" which implements the abstract class described in "event_detector.h".

## Notes
The magnitude resolution of the standard FFT algorithm in the Teensy Audio library proved to be not sufficient for high frequencies and low magnitude sounds. Therefore this library uses the higher resolution 32 bit floating point FFT from CMISIS-DSP. 

## License

Licensed under BSD 3-Clause License. See LICENSE.
