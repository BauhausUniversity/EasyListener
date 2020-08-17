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
#ifndef analyze_fft256_f32_h_
#define analyze_fft256_f32_h_

#include <Arduino.h>
#include "AudioStream.h"
#include "arm_math.h"
#include "feature_extractor.h"

#define FFT_SIZE 256

// windows.c
extern "C" {
extern const int16_t AudioWindowHanning256[];
extern const int16_t AudioWindowBartlett256[];
extern const int16_t AudioWindowBlackman256[];
extern const int16_t AudioWindowFlattop256[];
extern const int16_t AudioWindowBlackmanHarris256[];
extern const int16_t AudioWindowNuttall256[];
extern const int16_t AudioWindowBlackmanNuttall256[];
extern const int16_t AudioWindowWelch256[];
extern const int16_t AudioWindowHamming256[];
extern const int16_t AudioWindowCosine256[];
extern const int16_t AudioWindowTukey256[];
}

class FFT256F32
{
public:
	FFT256F32() : window(AudioWindowHanning256)
	{
		prevdata = NULL;
    	memset(_float_buffer, 0, sizeof(_float_buffer));
    	memset(buffer, 0, sizeof(buffer));
    	arm_rfft_fast_init_f32(&_fft_inst, FFT_SIZE);
	}

	float getBin(unsigned int binNumber) {
		if (binNumber > 127) return 0.0;
		return _fftMag[binNumber];
	}

	int16_t getNumOfBins() 
	{
		return FFT_SIZE/2;
	}

	void windowFunction(const int16_t *w) {
		window = w;
	}
	void process(int16_t *data);

	~FFT256F32(){};

  
private:
	const int16_t *window;
	int16_t  *prevdata;

	int16_t buffer[FFT_SIZE] __attribute__ ((aligned (4)));
	float32_t _float_buffer[FFT_SIZE]__attribute__((aligned(4)));
	float32_t _fftOutput[FFT_SIZE];
	float32_t _fftMag[FFT_SIZE];
	arm_rfft_fast_instance_f32  _fft_inst;
	float _scaleFactor = 1.0/((float)FFT_SIZE/2.0);
};

#endif
