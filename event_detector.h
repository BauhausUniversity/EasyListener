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
#ifndef event_detector_h_
#define event_detector_h_

class EventDetector 
{
	public:
		virtual bool eventOngoing();
        virtual bool eventOngoingAvailable();
		virtual bool eventValid();
        virtual bool envelopeAvailable();
		virtual int16_t getEnvelope();
        virtual float getEnvelopeDB();
		virtual void process(const int16_t *data, uint16_t frame_num);
        virtual uint16_t getFrameNum();

		virtual ~EventDetector(){}
};
#endif
