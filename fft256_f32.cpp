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
#include "fft256_f32.h"

static void copy_to_fft_buffer(int16_t *destination, const int16_t *source)
{
	for (int i=0; i < AUDIO_BLOCK_SAMPLES; i++) {
		*destination++ = *source++; 
	}
}

static void apply_window_to_fft_buffer(void *buffer, const void *window)
{
	int16_t *buf = (int16_t *)buffer;
	const int16_t *win = (int16_t *)window;;

	for (int i=0; i < 256; i++) {
		int32_t val = *buf * *win;
		*buf = val >> 15;
		buf++;
    win++;
	}
}

void FFT256F32::process(int16_t *data)
{
	if (!data) return;
	if (!prevdata) {
		prevdata = data;
		return;
	}
	copy_to_fft_buffer(buffer, prevdata);
	copy_to_fft_buffer(buffer+128, data);
	if (window) apply_window_to_fft_buffer(buffer, window);
  
	arm_q15_to_float(buffer, _float_buffer, FFT_SIZE); 
	/* Process the data through the CFFT/CIFFT module */
	arm_rfft_fast_f32(&_fft_inst, _float_buffer,  _fftOutput, 0);
	/* Process the data through the Complex Magnitude Module for
	calculating the magnitude at each bin */
	arm_cmplx_mag_f32(_fftOutput+2, _fftMag+1, FFT_SIZE/2 - 1);
	// Handle special cases
	_fftMag[0]          = fabs(_fftOutput[0]); // the DC bin is always real (no imaginary component), but can be negative
	_fftMag[FFT_SIZE/2] = fabs(_fftOutput[1]); // same for the nyquist component, read documentation of arm_rfft_fast_f32() for details
  	/* scale output to get correct amplitude factors */
  	arm_scale_f32(_fftMag, _scaleFactor , _fftMag, FFT_SIZE/2);
	prevdata = data;
}
