#ifndef __UltraschallSensor__
#define __UltraschallSensor__

#include <Arduino.h>

class UltraschallSensor {
  private: int trigger;
		   int echo;
		   double dauer;
		   double entfernung;

  public:
          UltraschallSensor (int trigger, int echo);
          void setMode();
          double getEntfernung();
};

#endif
