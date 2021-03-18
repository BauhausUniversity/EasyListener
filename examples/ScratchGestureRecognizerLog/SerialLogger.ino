void logFFTDatatoSerial()
{
  if (easyListener._FFT256F32->available())
  {
     // 1: signifies FFT data to processing sketch
    Serial.write((byte) 1);
    Serial.write((byte) easyListener._FFT256F32->getFrameNum());
    for (int i= 0; i<easyListener._FFT256F32->getNumOfBins(); i++)
    {
      int fftval = 20*log10(easyListener._FFT256F32->getBin(i));
      fftval = constrain(fftval, -120, -11);
      byte val = abs(fftval);
      Serial.write(val);
    }
    
    Serial.write('\n');
  }
}

void logThresholdCrossedToSerial()
{
  if (easyListener._eventDetector->eventOngoingAvailable())
  {
    if(easyListener._eventDetector->eventOngoing())
    {
    // 3: signifies threshold crossed to processing sketch
      Serial.write((byte) 3);
      Serial.write((byte) easyListener._eventDetector->getFrameNum());
      Serial.write((byte) abs(loudness_threshold));
      Serial.write('\n');
    }
  }
}

void logEnvelopeToSerial()
{
  if (easyListener._eventDetector->envelopeAvailable())
  {
    // 2: signifies envelope data to processing sketch
    Serial.write((byte) 2);
    Serial.write((byte) easyListener._eventDetector->getFrameNum());
    int env_val = easyListener._eventDetector->getEnvelopeDB();
    env_val = constrain(env_val, -60, -11);
    byte val = abs(env_val);
    Serial.write(val);
    Serial.write('\n');
  }
}
void logSoundIDToSerial( float min_score )
{
  if(easyListener.available() )
  {
    if(easyListener.getScore()>min_score) 
    {
       // 0: signifies sound id data
      Serial.write((byte) 0);
      Serial.write((byte) easyListener.getFrameNum());
      Serial.write((byte) easyListener.getSoundID());
      Serial.write((byte) easyListener.getScore());
      Serial.print( class_names[easyListener.getSoundID()] );
      Serial.write('\n');
      
    }
  }
}
void logGreetingToSerial()
{
   // 4: signifies greeting message
  Serial.write((byte) 4);
  Serial.print("Collecting data. \nTo train the algorithm please provide example sounds!");
  Serial.write('\n');
}
